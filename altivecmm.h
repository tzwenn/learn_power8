#pragma once

#include <cstdint>
#include <altivec.h>
#include <ostream>

namespace altivecmm {
	
	template<typename _VecT>
	class Vec {
	public:
		using vectype = _VecT;

		Vec()
		{
			;;
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

		////////////////////////////////////

	protected:
		vectype m_d;

	};

	class Vec2_uint64 {
		
	public:
		using vectype = __vector unsigned long long;

		Vec2_uint64(const uint64_t r0 = 0, const uint64_t r1 = 0) :
			m_d((vectype) {r0, r1})
		{
			;;
		}

		Vec2_uint64(const Vec2_uint64 & other) :
			m_d(other.m_d)
		{
			;;
		}

		Vec2_uint64(const vectype & d) :
			m_d(d)
		{
			;;
		}

		//////////////////////////////////////////////////

		Vec2_uint64 & operator +=(const Vec2_uint64 & other)
		{
			m_d = vec_add(m_d, other.m_d);
			return *this;
		}

		Vec2_uint64 & operator -=(const Vec2_uint64 & other)
		{
			m_d = vec_sub(m_d, other.m_d);
			return *this;
		}


		Vec2_uint64 & operator *=(const Vec2_uint64 & other)
		{
			m_d = vec_mul(m_d, other.m_d);
			return *this;
		}

		//////////////////////////////////////////////////
		
		operator vectype() const
		{
			return m_d;
		}
	
		//////////////////////////////////////////////////

		std::ostream& print(std::ostream & stream) const
		{
			const unsigned long long *data = (const unsigned long long *) &m_d;
			// vec_st(m_d, 0, data);
			stream << "Vec2_uint64(" << data[0] << ", " << data[1] << ")";
			return stream;
		}

		friend std::ostream& operator<< (std::ostream &out, const altivecmm::Vec2_uint64 & vec);

	protected:
		vectype m_d;
	};

}

std::ostream& operator<< (std::ostream &out, const altivecmm::Vec2_uint64 & vec)
{
	/*unsigned long long data[2];
	//vec_st(vec.m_d, 0, data);
	stream << "Vec2_uint64(" << data[0] << ", " << data[1] << ")";
	return stream;*/
}
