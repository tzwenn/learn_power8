#include <iostream>
#include <iomanip>

#include "altivecmm/altivecmm.h"

namespace gccfix {

	constexpr unsigned char extract_field(int a, int oct_field, int offset)
	{
		return (unsigned char)(((a >> (oct_field * 3)) & 7) * 4 + offset);
	}

	__vector unsigned char gpci(int a)
	{
		__vector unsigned char mask = {
			extract_field(a, 3, 0), extract_field(a, 3, 1), extract_field(a, 3, 2), extract_field(a, 3, 3),
			extract_field(a, 2, 0), extract_field(a, 2, 1), extract_field(a, 2, 2), extract_field(a, 2, 3),
			extract_field(a, 1, 0), extract_field(a, 1, 1), extract_field(a, 1, 2), extract_field(a, 1, 3),
			extract_field(a, 0, 0), extract_field(a, 0, 1), extract_field(a, 0, 2), extract_field(a, 0, 3)
		};
		return mask;
	}

}

template<typename T>
struct Foo
{
	T d;

	void print()
	{
		printf("Hi %d\n", sizeof(d));
	}
};


template<typename VecT>
typename VecT::vectype slo(VecT v, int shift)
{
	return vec_slo(v, altivecmm::Vec<signed char>(shift << 3).d());
}

int main(int argc, char *argv[])
{
	altivecmm::Vec2_double v1{-1, 2};
	altivecmm::Vec2_double v2{3, 4};

	std::cout << (v1 / v2) << std::endl;

	using V32t = altivecmm::Vec<uint32_t>;
	__vector unsigned int v3 = {0xff000001, 2, 3, 4};
	asm volatile ("startlabel: ");
	__vector unsigned int permuted = vec_slo(v3, altivecmm::Vec<signed char>(1 << 3).d());
	//__vector unsigned int permuted = vec_sld(v3, v4, 1);
	asm volatile ("endlabel: ");

	//std::cout << std::hex << altivecmm::Vec<signed char>(4 << 3) << std::endl;
	std::cout << std::hex << V32t(permuted) << std::endl;

	return 0;
}
