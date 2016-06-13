#include <stdio.h>
#include <iostream>

#include "altivecmm/altivecmm.h"

int main(int argc, char *argv[])
{
	altivecmm::Vec2_double v1{1, 2};
	altivecmm::Vec2_double v2{3, 4};

	std::cout << (v1 / v2).abs() << std::endl;

	return 0;
}
