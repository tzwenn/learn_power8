#pragma once

#include <cstdint>
#include <altivecmm/altivecmm.h>

#ifndef ENABLE_QUAD
#define ENABLE_QUAD 1
#endif

namespace pwr {

	using Vecmm = altivecmm::Vec<uint32_t>;
	using vecu32_t = Vecmm::vectype;

	enum {
		sigma0 = 0,
		sigma1 = 0xf
	};

	enum {
		lowercase = 0,
		uppercase = 1
	};

	template<int case_select, int func_select>
	inline vecu32_t vsigma(vecu32_t x)
	{
		return __builtin_crypto_vshasigmaw(x, case_select, func_select);
	}

	static Vecmm Ch(Vecmm x, Vecmm y, Vecmm z)
	{
		return (x & y) ^ ((~x) & z);
	}

	static Vecmm Maj(Vecmm x, Vecmm y, Vecmm z)
	{
		return (x & y) ^ (x & z) ^ (y & z);
	}

	//////////////////////////////

	static vecu32_t pack(uint32_t input)
	{
		vecu32_t output = {input, 0, 0, 0};
		return output;
	}

	static uint32_t unpack(vecu32_t input)
	{
		uint32_t output[4] __attribute__((aligned(16)));
		vec_st(input, 0, output);
		return output[0];
	}

	template<int case_select, int func_select>
	Vecmm sigma(Vecmm x)
	{
		return __builtin_crypto_vshasigmaw(x.d(), case_select, func_select);
	}

	template<int case_select, int func_select>
	uint32_t sigma(uint32_t x)
	{
		return unpack(vsigma<case_select, func_select>(pack(x)));
	}

#if !ENABLE_QUAD
	uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
	{
		return unpack(Ch(pack(x), pack(y), pack(z)));
	}

	uint32_t Maj(uint32_t x, uint32_t y, uint32_t z)
	{
		return unpack(Maj(pack(x), pack(y), pack(z)));
	}
#endif
}
