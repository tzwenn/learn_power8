#include <thread>
#include <chrono>
#include <iostream>

#include "HazardList.h"

void repeatedAppend(HazardList *list, int repeats, HazardList::Parity parity)
{
	while (repeats--) {
		list->appendOn(parity);
	}
	list->openForAll();
}

int main()
{
	HazardList list;
	const int repeats = 4;

	std::thread evenThread(repeatedAppend, &list, repeats, HazardList::Even);
	std::thread  oddThread(repeatedAppend, &list, repeats, HazardList::Odd );

	evenThread.join();
	oddThread.join();

	//list.print();
	std::cout << (list.verify() ? "[OK]" : "[FAIL]") << std::endl;

	return 0;
}


