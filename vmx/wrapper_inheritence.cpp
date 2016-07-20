#include <cstdio>
#include <altivec.h>

template<typename T>
struct typetable;

template<>
struct typetable<float>
{
	using vectype = __vector float;
};

/////////////////////////////////////////

template<typename elemtype>
class BasicWrapper
{
public:
	using typeinfo = typetable<elemtype>;
	using vectype = typename typeinfo::vectype;

	BasicWrapper(vectype d) :
		value(d)
	{
		;;
	}

	template<typename S>
	BasicWrapper(const BasicWrapper<S> & other) :
		value(other.value)
	{
		;;
	}

	operator vectype() const
	{
		return value;
	}

	//////////

	vectype value;
};

//template<typename elemtype>
template<typename elemtype>
class Vec: public BasicWrapper<elemtype>
{
	using wrapper = BasicWrapper<elemtype>;
public:
	using wrapper::value;

	using vectype = typename wrapper::vectype;
	using typeinfo = typename wrapper::typeinfo;

	Vec(const wrapper & base) :
		wrapper(base)
	{
		;;
	}

	wrapper operator+(const Vec & other)
	{
		return vec_add(value, other.value);
	}
};

/////////////////////////////////////////

void printVec(const vector float v)
{
	float *pv = (float *)&v;
	printf("vector float <%f, %f, %f, %f>\n", pv[0], pv[1], pv[2], pv[3]);
}

int main(int argc, char *argv[])
{
	__vector float vdata1 = {1, 2, 3, 4};
	__vector float vdata2 = {4, 3, 2, 1};
	Vec<float> v1(vdata1);
	Vec<float> v2(vdata2);

	Vec<float> vsum = v1 + v2;

	printVec(vsum);

	v1.value = (__vector float){0, 8, 1, 5};

	printVec(v1);

	return 0;
}
