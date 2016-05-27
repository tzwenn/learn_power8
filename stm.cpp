#include <thread>
#include <chrono>
#include <iostream>

#include "HazardList.h"

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


