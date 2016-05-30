#include "HazardList.h"

#include "stm4pwr.h"

#include <iostream>

struct ListEl
{
	typedef int ValT;

	ValT value;
	ListElPtr next, prev;

	ListEl(const ValT & _value) :
		value(_value)
	{
		;;
	}

	static ListElPtr appendTo(ValT _value, ListElPtr old)
	{
		ListElPtr el(new ListEl(_value));
		if (old)
			old->next = el;
		el->prev = old;
		return el;
	}
};

///////////////////////////////////////////////////////////////

HazardList::Parity HazardList::oddOrEven(const ListElPtr & el)
{
	return (Parity) (el->value & 1);
}

HazardList::HazardList() :
	m_first(new ListEl(0)),
	m_always_append(false)
{
	m_last = m_first;
}

HazardList::~HazardList()
{
	#if !HAZARD_LIST_SHARED_PTR
	while (m_first->next) {
		ListElPtr next = m_first->next;
		delete m_first;
		m_first = next;
	}
	#endif
}

#include <thread>

bool HazardList::itIsTimeToAppend(const Parity parity) const
{
	return oddOrEven(m_last) == parity;
}

void HazardList::appendOn(const Parity parity)
{
	//stm4pwr::MutexLock lock;
	while (!(m_always_append || itIsTimeToAppend(parity))) {
		// std::cout << std::this_thread::get_id() << ": " << !m_always_append << oddOrEven(m_last) << parity << std::endl;
		;;
	}
	m_last = ListEl::appendTo(m_last->value + 1, m_last);
}

bool HazardList::verify() const
{
	int prevVal = m_first->value;
	for (ListElPtr cur = m_first->next; cur; cur = cur->next, prevVal++) {
		if (cur->value != prevVal + 1) {
			std::cerr << cur->value << " followed " << prevVal << std::endl;
			return false;
		}
	}
	return true;
}

void HazardList::print() const
{
	std::cout << "HazardList:";
	for (ListElPtr cur = m_first; cur; cur = cur->next) {
		std::cout << " " << cur->value;
	}
	std::cout << std::endl;
}

