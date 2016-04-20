#pragma once

#include <cstdint>
#include <altivec.h>
#include <ostream>
#include <iostream>

#include "altivecmm_typetable.h"

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
			auto *data = (const elemtype *) &m_d;
			stream << "Vec<>(" << m_d[0];
			for (int i = 1; i < typeinfo::elem_count; i++)
				stream << ", " << m_d[i];
			stream << ")";
			return stream;
		}

	protected:
		vectype m_d;

	};

	class Vec2_uint64: public Vec<unsigned long>
	{
	public:

		Vec2_uint64(const uint64_t r0 = 0, const uint64_t r1 = 0) :
			Vec<unsigned long>((vectype) {r0, r1})
		{
			;;
		}
	};

}

