#include "HazardList.h"

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
	m_first(new ListEl(0))
{
	m_last = m_first;
}

HazardList::~HazardList()
{
	#if !HAZARD_LIST_SHARED_PTR
	ListElPtr next = m_first->next;
	while (m_first) {
		delete m_first;
		next = m_first = next->next;
	}
	#endif
}

void HazardList::appendOn(Parity parity)
{
	while (true) {
		 __transaction_atomic {
			//STMLock lock;
			if (oddOrEven(m_last) == parity) {
				m_last = ListEl::appendTo(m_last->value + 1, m_last);
				return;
			}
		}
	}
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
