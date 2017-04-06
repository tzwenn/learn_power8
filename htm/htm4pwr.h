#pragma once

#include <mutex>
#include <functional>
#include <type_traits>

#include <iostream>

namespace htm4pwr {

	class MutexLock: public std::lock_guard<std::mutex>
	{
		static std::mutex s_mutex;

	public:
		MutexLock();
	};

	class HTMLock
	{
	public:
		HTMLock()
		{
			begin:
				asm goto ("tbegin.\n\t"
						  "beq- %l[failure_hdlr]"
						   : : : : failure_hdlr);
				return;

			failure_hdlr:
				goto begin;
		}

		HTMLock(std::function<bool()> failure_handler)
		{
			begin:
				asm goto ("tbegin.\n\t"
							  "beq- %l[failure_hdlr]"
							   : : : : failure_hdlr);
				return;

			failure_hdlr:
				if (failure_handler())
					goto begin;
		}

		void abort();

		~HTMLock()
		{
			asm volatile ("tend.");
		}
	};

	class AtomicTransaction
	{
	public:
		AtomicTransaction(std::function<void()> f)
		{
			HTMLock lock;
			(void)lock;
			f();
		}
	};

	template<class Function, class... Args>
	typename std::result_of<Function(Args...)>::type atomic(Function&& f, Args&&... args)
	{
		HTMLock lock;
		return f(args...);
	}
}
