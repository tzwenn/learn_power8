#include <cstdio>
#include <iostream>
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
		m_d(d)
	{
		;;
	}

	template<typename S>
	BasicWrapper(const BasicWrapper<S> & other) :
		m_d(other.m_d)
	{
		;;
	}

	operator vectype() const
	{
		return m_d;
	}

	vectype d() const
	{
		return m_d;
	}

	//////////

protected:
	vectype m_d;
};

//template<typename elemtype>
template<typename elemtype>
class Vec: public BasicWrapper<elemtype>
{
	using baseclass = BasicWrapper<elemtype>;
public:
	using baseclass::m_d;

	using vectype = typename baseclass::vectype;
	using typeinfo = typename baseclass::typeinfo;

	Vec(vectype v) :
		baseclass(v)
	{
		;;
	}

	Vec(const baseclass & base) :
		baseclass(base)
	{
		;;
	}

	Vec operator+(const Vec & other)
	{
		return vec_add(m_d, other.m_d);
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

#if 0
	Vec<float> vsum = v1 + v2;

	printVec(vsum);
#endif

	//v1.m_d = (__vector float){0, 8, 1, 5};

	Vec<float> eier = static_cast<Vec<float>>(vec_sqrt(v1.d()));

	printVec(v1);

	return 0;
}
