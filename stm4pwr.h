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
		STMLock()
		{
			begin:
				asm goto ("tbegin.\n\t"
						  "beq- %l[failure_hdlr]"
						   : : : : failure_hdlr);
				return;

			failure_hdlr:
				goto begin;
		}

		STMLock(std::function<bool()> failure_handler)
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

		~STMLock()
		{
			asm volatile ("tend.");
		}
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
