#pragma once

#define HAZARD_LIST_SHARED_PTR 0

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

	void appendOn(Parity parity);
	bool verify() const;

private:
	ListElPtr m_first, m_last;
};
