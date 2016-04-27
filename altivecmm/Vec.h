#pragma once

#include <altivec.h>
#include <ostream>

#include "typetable.h"

namespace altivecmm {

	template<typename elemtype>
	class Vec
	{
	public:
		using typeinfo = typetable<elemtype>;
		using vectype = typename typeinfo::vectype;

		Vec(const elemtype * data)
		{
			auto d = vec_ld(typeinfo::elem_count, data);
			m_d = d;
		}

		template<typename S>
		Vec(const Vec<S> & other) :
			m_d(other.m_d)
		{
			;;
		}

		Vec(const vectype & d) :
			m_d(d)
		{
			;;
		}

		////////////////////////////////////

		Vec & operator +=(const Vec & other)
		{
			m_d = vec_add(m_d, other.m_d);
			return *this;
		}

		Vec & operator -=(const Vec & other)
		{
			m_d = vec_sub(m_d, other.m_d);
			return *this;
		}

		Vec & operator *=(const Vec & other)
		{
			m_d = vec_mul(m_d, other.m_d);
			return *this;
		}

		////////////////////////////////////

		Vec operator +(const Vec & other) const
		{
			return vec_add(m_d, other.m_d);
		}

		Vec operator -(const Vec & other) const
		{
			return vec_sub(m_d, other.m_d);
		}

		Vec operator *(const Vec & other) const
		{
			return vec_mul(m_d, other.m_d);
		}

		////////////////////////////////////

		operator vectype() const
		{
			return m_d;
		}

		////////////////////////////////////

		std::ostream& print(std::ostream & stream) const
		{
			stream << "Vec<" << typeinfo::typestr << ">(" << m_d[0];
			for (int i = 1; i < typeinfo::elem_count; i++)
				stream << ", " << m_d[i];
			stream << ")";
			return stream;
		}

	protected:
		vectype m_d;

	};

}

template<typename elemtype>
std::ostream &operator<<(std::ostream & stream, const altivecmm::Vec<elemtype> & vec)
{
	return vec.print(stream);
}
