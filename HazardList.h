#pragma once

#ifndef HAZARD_LIST_SHARED_PTR
#    define HAZARD_LIST_SHARED_PTR 0
#endif

struct ListEl;
#if HAZARD_LIST_SHARED_PTR
	typedef std::shared_ptr<ListEl> ListElPtr;
#else
	typedef ListEl* ListElPtr;
#endif

class HazardList
{
public:
	enum Parity {
		Odd = 1,
		Even = 0
	};

	static Parity oddOrEven(const ListElPtr & el);

	HazardList();
	~HazardList();

	bool itIsTimeToAppend(const Parity parity) const;
	void appendOn(const Parity parity);

	bool verify() const;
	void print() const;

	void openForAll()
	{ m_always_append = true; }

private:
	ListElPtr m_first, m_last;
	bool m_always_append;
};
