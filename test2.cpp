#include <stdio.h>
#include <iostream>

#include "altivecmm/altivecmm.h"

int main(int argc, char *argv[])
{
	using vti = altivecmm::Vec<int>;
	int values[] = {1337, 1748, 1, 2};

	altivecmm::Vec2_int64 v1(1, 2);
	altivecmm::Vec2_int64 v2(3, 4);

	altivecmm::Vec<int> v3(values);

	std::cout << v3 << std::endl;
	std::cout << (v1 - v2).abs() << std::endl;

	vti vc = vti::construct(-1, -2, -3, -4);

	std::cout << vc << std::endl;

	return 0;
}
