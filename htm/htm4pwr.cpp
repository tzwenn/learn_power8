#include "htm4pwr.h"

namespace htm4pwr {

std::mutex MutexLock::s_mutex;

MutexLock::MutexLock() :
	std::lock_guard<std::mutex>(s_mutex)
{
	;;
}

void HTMLock::abort()
{
	// asm volatile ("tabort.");
}

}

