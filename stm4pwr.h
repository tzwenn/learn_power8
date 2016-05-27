#pragma once

#include <mutex>
#include <functional>

namespace stm4pwr {

	class MutexLock: public std::lock_guard<std::mutex>
	{
		static std::mutex s_mutex;

	public:
		MutexLock();
	};

	class STMLock
	{
	public:
		STMLock();

		void abort();
		~STMLock();
	};

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
