#include "stm4pwr.h"

namespace stm4pwr {

std::mutex MutexLock::s_mutex;

MutexLock::MutexLock() :
	std::lock_guard<std::mutex>(s_mutex)
{
	;;
}

STMLock::STMLock()
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

void STMLock::abort()
{
	// asm volatile ("tabort.");
}

STMLock::~STMLock()
{
	asm volatile ("tend.");
}


}

