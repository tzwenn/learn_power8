#include <iostream>

#include "altivecmm/altivecmm.h"
#include "altivecmm/cond.h"

int main(int argc, char *argv[])
{
	altivecmm::Vec<signed int> vec{1, -2, 3, -4}, zero;

	auto res = altivecmm::_if(
		vec < zero,
			zero - vec,
			vec
		);

	std::cout << res << std::endl;
}
