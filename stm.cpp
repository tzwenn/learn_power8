#include <iostream>
#include <chrono>

#include <condition_variable>
#include <thread>

#include "stm4pwr.h"

const uint64_t thread_count = 1 << 14;

static void busy_wait(const int slowdown_repeats = 1 << 20)
{
	for (volatile int i = 0; i < slowdown_repeats; i++);
}

class Barrier
{
public:
	explicit Barrier(std::size_t count) :
		m_count(count)
	{
		;;
	}

	void wait()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (--m_count == 0) {
			m_cv.notify_all();
		} else {
			m_cv.wait(lock, [this]{ return m_count == 0; });
		}
	}
private:
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::size_t m_count;
};

using Lock = stm4pwr::MutexLock;

template<typename T>
void slow_inc(T *data)
{
	{
		Lock lock;
		*data *= 2;
		*data += 2;
		*data /= 2;
	}
}

template<typename T>
void slow_dec(T *data)
{
	{
		Lock lock;
		*data *= 2;
		*data -= 2;
		*data /= 2;
	}
}

class DeferredTestThread
{
	enum State {VALID, INVALID};
public:
	DeferredTestThread():
		m_state(INVALID)
	{
		;;
	}

	DeferredTestThread(Barrier & startBarrier, Barrier & endBarrier, std::function<void()> run):
		m_state(VALID)
	{
		m_thread = std::thread{[&]{
			startBarrier.wait();
			run();
			endBarrier.wait();
		}};
	}

	~DeferredTestThread()
	{
		if (m_state == VALID)
			m_thread.join();
	}

private:
	State m_state;
	std::thread m_thread;

};

#ifndef ENABLE_TIMER
#define ENABLE_TIMER 1
#endif

int main()
{
	volatile uint64_t test_d = 0;
	auto testFunction = [&]{
		busy_wait();
		slow_inc(&test_d);

		busy_wait();
		slow_inc(&test_d);

		busy_wait();
		slow_dec(&test_d);
	};

#if ENABLE_TIMER
	auto lastTime = std::chrono::high_resolution_clock::now();
#endif

	#pragma omp parallel for
	for (uint64_t i = 0; i < thread_count; i++) {
		testFunction();
	}

#if ENABLE_TIMER
	std::chrono::duration<float, std::chrono::milliseconds::period> duration = std::chrono::high_resolution_clock::now() - lastTime;

	std::cout << "Needed " << duration.count() << " milliseconds" << std::endl;
	std::cout << test_d << " " << thread_count << " (" << thread_count - test_d << " off)" << std::endl;
	std::cout << (test_d == thread_count ? "[OK]" : "[FAIL]") << std::endl;
#endif

	return test_d != thread_count;
}


