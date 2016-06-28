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
	Vecmm sigma(Vecmm x)
	{
		return __builtin_crypto_vshasigmaw(x.d(), case_select, func_select);
	}

	static Vecmm Ch(Vecmm x, Vecmm y, Vecmm z)
	{
		return (x & y) ^ ((~x) & z);
	}

	static Vecmm Maj(Vecmm x, Vecmm y, Vecmm z)
	{
		return (x & y) ^ (x & z) ^ (y & z);
	}

#if !ENABLE_QUAD
	static uint32_t ROTL(uint32_t x, uint32_t n)
	{
		return (x << n) | (x >> (sizeof(x)*8 - n));
	}

	static uint32_t ROTR(uint32_t x, uint32_t n)
	{
		return (x >> n) | (x << (sizeof(x)*8 - n));
	}

	template<int case_select, int func_select>
	uint32_t sigma(const uint32_t x);

	template<>
	uint32_t sigma<uppercase, sigma0>(const uint32_t x)
	{
		return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
	}

	template<>
	uint32_t sigma<uppercase, sigma1>(const uint32_t x)
	{
		return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
	}

	template<>
	uint32_t sigma<lowercase, sigma0>(const uint32_t x)
	{
		return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
	}

	template<>
	uint32_t sigma<lowercase, sigma1>(const uint32_t x)
	{
		return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
	}

	static uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
	{
		return (x & y) ^ ((~x) & z);
	}

	static uint32_t Maj(uint32_t x, uint32_t y, uint32_t z)
	{
		return (x & y) ^ (x & z) ^ (y & z);
	}
#endif
}
