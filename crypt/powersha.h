#pragma once

#include <altivecmm/altivecmm.h>

namespace pwr {

	enum {
		sigma0 = 0,
		sigma1 = 0xf
	};

	enum {
		lowercase = 0,
		uppercase = 1
	};

	template<int case_select, int func_select>
	inline __vector unsigned int sigma(__vector unsigned int x)
	{
		return __builtin_crypto_vshasigmaw(x, case_select, func_select);
	}

	template<int case_select, int func_select>
	uint32_t sigma(uint32_t x)
	{
		__vector unsigned int input = {x, 0, 0, 0};
		uint32_t output[4] __attribute__((aligned(16)));

		vec_st(sigma<case_select, func_select>(input), 0, output);

		return output[0];
	}

}
