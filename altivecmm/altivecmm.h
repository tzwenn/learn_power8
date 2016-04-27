#pragma once

#include <cstdint>

#include "Vec.h"

namespace altivecmm {

	class Vec2_uint64: public Vec<unsigned long>
	{
	public:

		Vec2_uint64(const uint64_t r0 = 0, const uint64_t r1 = 0) :
			Vec<unsigned long>((vectype) {r0, r1})
		{
			;;
		}
	};

	class Vec2_int64: public Vec<long>
	{
	public:

		Vec2_int64(const int64_t r0 = 0, const int64_t r1 = 0) :
			Vec<long>((vectype) {r0, r1})
		{
			;;
		}
	};
}

