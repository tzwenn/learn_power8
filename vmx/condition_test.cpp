#include <iostream>

#include "altivecmm/altivecmm.h"
#include "altivecmm/cond.h"

int main(int argc, char *argv[])
{
	altivecmm::Vec<int> vec{1, -2, 3, -4};

	auto abs0 = altivecmm::ifthenelse(
		vec > 0,
			vec,
			-vec
		);

	auto abs1 = altivecmm::_if(vec > 0)(
				vec
			)(
				-vec
			);



	std::cout << "Abs0 = " << abs0 << std::endl;
	std::cout << "Abs1 = " << abs1 << std::endl;

	altivecmm::Vec<int> data{13, 8, 5, 3};
	altivecmm::Vec<int> modeSelect{3, 2, 1, 0};

	auto res = altivecmm::_switch(modeSelect)
			(1, 2 + 3)
			(2, 2 * 3)
			(3, data)
			(abs0);


	std::cout << "Switch-Case: " << res << std::endl;
}
