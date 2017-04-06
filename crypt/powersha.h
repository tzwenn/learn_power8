#pragma once

#include <cstdint>
#include <altivecmm/altivecmm.h>

#ifndef ENABLE_QUAD
#define ENABLE_QUAD 1
#endif

namespace pwr {

	using Vecmm = altivecmm::Vec<uint32_t>;
	using vecu32_t = Vecmm::vectype;

	enum sigma_index {
		index0 = 0,
		index1 = 0xf
	};

	enum sigma_case {
		lowercase = 0,
		uppercase = 1
	};

	template<sigma_case case_select, sigma_index index_select>
	altivecmm::Vec<uint32_t> sigma(altivecmm::Vec<uint32_t> x)
	{
		return __builtin_crypto_vshasigmaw(x.d(), case_select, index_select);
	}

	template<sigma_case case_select, sigma_index index_select>
	altivecmm::Vec<uint64_t> sigma(altivecmm::Vec<uint64_t> x)
	{
		return __builtin_crypto_vshasigmad(x.d(), case_select, index_select);
	}

	static inline Vecmm Ch(Vecmm b, Vecmm c, Vecmm d)
	{
		return vec_sel(d.d(), c.d(), b.d());
	}

	static inline Vecmm Maj(Vecmm x, Vecmm y, Vecmm z)
	{
		return vec_sel(x.d(), y.d(), vec_xor(x.d(), z.d()));
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

	template<sigma_case case_select, sigma_index index_select>
	uint32_t sigma(const uint32_t x);

	template<>
	uint32_t sigma<uppercase, index0>(const uint32_t x)
	{
		return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
	}

	template<>
	uint32_t sigma<uppercase, index1>(const uint32_t x)
	{
		return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
	}

	template<>
	uint32_t sigma<lowercase, index0>(const uint32_t x)
	{
		return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
	}

	template<>
	uint32_t sigma<lowercase, index1>(const uint32_t x)
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
