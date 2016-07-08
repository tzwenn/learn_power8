#include <cstdio>
#include <altivec.h>

void printVec(const vector float v)
{
	float *pv = (float *)&v;
	printf("vector float <%f, %f, %f, %f>\n", pv[0], pv[1], pv[2], pv[3]);
}

using native = vector float;

//using type_w = altivecmm::Vec<float>;
class wrapper
{
public:
	wrapper(native d)
	{
		_d = d;
	}

	operator native() const
	{
		return _d;
	}

	native _d;
};

native operator+(const wrapper & lhs, const wrapper & rhs)
{
	return vec_add(lhs._d, rhs._d);
}

native test1_n(native a, native b)
{
	return vec_add(a, b);
}

native test1_wn(wrapper a, wrapper b)
{
	return a + b;
}

native test1_w(const wrapper & a, const wrapper & b)
{
	return a + b;
}

int main(int argc, char *argv[])
{
	vector float a = {1, 2, 3, 4};
	vector float b = {4, 3, 2, 1};

	asm volatile ("building_vec:");
	wrapper A(a);
	asm volatile ("endbuilding_vec:");

	printf("sizeof(A) = %lu\n", sizeof(A));

	asm volatile ("calling_test1_n:");
	printVec(test1_n(a, b));

	asm volatile ("calling_test1_wn:");
	printVec(test1_wn(a, b));

	asm volatile ("calling_test1_w:");
	printVec(test1_w(a, b));


	printVec(A._d);

	return 0;
}
