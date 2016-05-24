#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <functional>

#include <mutex>

static std::mutex s_mutex;

struct MutexLock: public std::lock_guard<std::mutex>
{
	MutexLock() : std::lock_guard<std::mutex>(s_mutex)
	{
		;;
	}
};

struct STMLock
{
	STMLock()
	{
		asm volatile ("tbegin.");
#if 0
		asm goto ("tbegin.\n\t"
					  "beq- %l[failure_hdlr]"
					  :
					  :
					  :
					  : failure_hdlr);
		return;

		failure_hdlr:
			throw 1;
#endif
	}

	void abort()
	{
		asm volatile ("tabort.");
	}

	~STMLock()
	{
		asm volatile ("tend.");
	}
};

const uint64_t repeats = 1 << 10;
const std::chrono::microseconds waittime(100);

#define __transaction_atomic

namespace stm4pwr {

	class AtomicTransaction
	{
	public:
		AtomicTransaction(std::function<void()> f)
		{
			STMLock lock;
			(void)lock;
			f();
		}
	};
}

void f1(double *a, double *b, double step)
{
	{
		STMLock lock;
		*b = *a - repeats * step;
		*a = *b - repeats * step;
	}

	for (uint64_t i = 0; i < repeats; i++) {
		stm4pwr::AtomicTransaction([&]{
			*b = *a + step;
			std::this_thread::sleep_for(waittime);
			*a = *b + step;
		});
	}

	stm4pwr::AtomicTransaction([&]{
		*b = *a - repeats * step;
		*a = *b - repeats * step;
	});
}

void f2(double *glob, double *local)
{
	for (uint64_t i = 0; i < repeats; i++) {
		stm4pwr::AtomicTransaction([&]{
			*local = *glob * 5;
			std::this_thread::sleep_for(waittime);
			*glob = *local / 5;
		});
	}
}

int main()
{
	double a = 13.37, b = 0;
	double step1;

	return 0;
}
