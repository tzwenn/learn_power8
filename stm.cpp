#include <iostream>
#include <chrono>

#include "stm4pwr.h"

const uint64_t repeats = 1 << 14;

static void busy_wait(const int slowdown_repeats = 1 << 20)
{
	for (volatile int i = 0; i < slowdown_repeats; i++);
}

using Lock = stm4pwr::STMLock;

#define __transaction_atomic

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

#ifndef ENABLE_TIMER
#define ENABLE_TIMER 1
#endif

int main()
{
	volatile uint64_t test_d = 0;

#if ENABLE_TIMER
	auto lastTime = std::chrono::high_resolution_clock::now();
#endif

	#pragma omp parallel for
	for (uint64_t i = 0; i < repeats; i++) {
		busy_wait();
		slow_inc(&test_d);

		busy_wait();
		slow_inc(&test_d);

		busy_wait();
		slow_dec(&test_d);
	}

#if ENABLE_TIMER
	std::chrono::duration<float, std::chrono::milliseconds::period> duration = std::chrono::high_resolution_clock::now() - lastTime;

	std::cout << "Needed " << duration.count() << " milliseconds" << std::endl;
	std::cout << test_d << " " << repeats << " (" << repeats - test_d << " off)" << std::endl;
	std::cout << (test_d == repeats ? "[OK]" : "[FAIL]") << std::endl;
#endif

	return test_d != repeats;
}


