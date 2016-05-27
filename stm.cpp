#include <thread>
#include <chrono>
#include <iostream>
#include <functional>

#include <mutex>

#include "HazardList.h"

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

namespace stm4pwr {

	class AtomicTransaction
	{
	public:
		AtomicTransaction(std::function<void()> f)
		{
			MutexLock lock;
			(void)lock;
			f();
		}
	};
}

void repeatedAppend(HazardList *list, int repeats, HazardList::Parity parity)
{
	while (repeats--) {
		list->appendOn(parity);
	}
}

int main()
{
	HazardList list;
	const int repeats = 100;

	std::thread evenThread(repeatedAppend, &list, repeats, HazardList::Even);
	std::thread  oddThread(repeatedAppend, &list, repeats, HazardList::Odd );

	evenThread.join();
	oddThread.join();

	std::cout << "verify: " << list.verify() << std::endl;

	return 0;
}


