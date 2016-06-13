#include "stm4pwr.h"

namespace stm4pwr {

std::mutex MutexLock::s_mutex;

MutexLock::MutexLock() :
	std::lock_guard<std::mutex>(s_mutex)
{
	;;
}

void STMLock::abort()
{
	// asm volatile ("tabort.");
}

}

