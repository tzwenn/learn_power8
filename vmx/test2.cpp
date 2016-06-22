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

int main(int argc, char *argv[])
{
	altivecmm::Vec2_double v1{-1, 2};
	altivecmm::Vec2_double v2{3, 4};

	std::cout << (v1 / v2) << std::endl;

	using V32t = altivecmm::Vec<uint32_t>;
	V32t v3{0x01234567, 2, 0x89abcdef, 3};

	V32t permuted = vec_perm(v3.d(), v3.d(), gccfix::gpci(01032));

	std::cout << std::hex << permuted << std::endl;

	return 0;
}
