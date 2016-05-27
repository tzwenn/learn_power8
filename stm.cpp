#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <functional>

#include <mutex>


static std::mutex s_mutex;

struct MutexLock: public std::lock_guard<std::mutex>
{
	MutexLock() : std::lock_guard<std::mutex>(s_mutex)
	{
		;;
	}
};

struct STMLock
{
	STMLock()
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

	void abort()
	{
		asm volatile ("tabort.");
	}

	~STMLock()
	{
		asm volatile ("tend.");
	}
};

const std::chrono::microseconds waittime(100);

namespace stm4pwr {

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

/////////////////////////////////////////////////////////////////


struct ListEl;
typedef std::shared_ptr<ListEl> ListElPtr;

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


class List
{
public:
	enum Parity {
		Odd = 1,
		Even = 0
	};


	static Parity oddOrEven(const ListElPtr & el)
	{
		return (Parity) (el->value & 1);
	}

	////////////////////////////////////////////

	List() :
		first(new ListEl(0))
	{
		last = first;
	}

	~List()
	{
		;;
	}

	void appendOn(Parity parity)
	{
		while (true) {
			MutexLock lock;
			if (oddOrEven(last) == parity) {
				last = ListEl::appendTo(last->value + 1, last);
				return;
			}
		}
	}

	bool verify()
	{
		int prevVal = first->value;
		for (ListElPtr cur = first->next; cur; cur = cur->next, prevVal++) {
			if (cur->value != prevVal + 1) {
				std::cerr << cur->value << " followed " << prevVal << std::endl;
				return false;
			}
		}
		return true;
	}

private:
	ListElPtr first, last;
};

/////////////////////////////////////////////////////////////////

void repeatedAppend(List *list, int repeats, List::Parity parity)
{
	while (repeats--) {
		list->appendOn(parity);
	}
}

int main()
{
	List list;
	const int repeats = 10;

	std::thread evenThread(repeatedAppend, &list, repeats, List::Even);
	std::thread  oddThread(repeatedAppend, &list, repeats, List::Odd );

	//repeatedAppend(&list, repeats, List::Odd);

	evenThread.join();
	oddThread.join();

	std::cout << "verify: " << list.verify() << std::endl;

	return 0;
}


