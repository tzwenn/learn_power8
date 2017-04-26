#include <stdio.h>
#include <stdint.h>

#include <altivec.h>

typedef vector unsigned int vu32_t;


// Scalar ////////////////////////////////////////

static inline uint32_t ROTR_32_s(const uint32_t x, int n)
{
	return (x >> n) | (x << (sizeof(x)*8 - n));
}


static inline uint32_t Sigma0_32_s(const uint32_t x)
{
	return ROTR_32_s(x, 2) ^ ROTR_32_s(x, 13) ^ ROTR_32_s(x, 22);
}

static inline uint32_t Ch_32_s(const uint32_t x, const uint32_t y, const uint32_t z)
{
	return (x & y) ^ ((~x) & z);
}

static inline uint32_t Maj_32_s(const uint32_t x, const uint32_t y, const uint32_t z)
{
	return (x & y) ^ (x & z) ^ (y & z);
}


// Vector - unenhanded  //////////////////////////

static inline vu32_t vROTR_32_v(const vu32_t x, int n)
{
	// Due to the absence of a vec_rr function: vec_rl(x, 32 - n)
	// And because vec_splat_s32 can only go up to 16: memory load

	const int rotater = 32 - n;
	vu32_t vrotater = {rotater, rotater, rotater, rotater};

	return vec_rl(x, vrotater);
}

static inline vu32_t Sigma0_32_v(const vu32_t x)
{
	return vec_xor(vROTR_32_v(x, 2), vec_xor(vROTR_32_v(x, 13), vROTR_32_v(x, 22)));
}

static inline vu32_t Ch_32_v(const vu32_t x, const vu32_t y, const vu32_t z)
{
	// Due to the absence of vec_not: vec_nand(x, x);
	return vec_xor(vec_and(x, y),
				   vec_and(vec_nand(x, x), z));
}

static inline vu32_t Maj_32_v(const vu32_t x, const vu32_t y, const vu32_t z)
{
	return vec_xor(vec_and(x, y),
		   vec_xor(vec_and(x, z),
				   vec_and(y, z)));
}


// Vector - POWER8 enhanced ///////////////////////

static inline vu32_t Sigma0_32_p(const vu32_t x)
{
	return __builtin_crypto_vshasigmaw(x, 1, 0);
}

static inline vu32_t Ch_32_p(const vu32_t x, const vu32_t y, const vu32_t z)
{
	return vec_sel(z, y, x);
}

static inline vu32_t Maj_32_p(const vu32_t x, const vu32_t y, const vu32_t z)
{
	return vec_sel(vec_xor(z, y), y, x);
}


// Main ///////////////////////////////////////////


void functionality_test()
{
	const uint32_t data = 42;
	vu32_t vdata = {data, data, data, data};

	printf("scalar: %x\n", Sigma0_32_s(data));
	printf("vector: %x\n", Sigma0_32_v(vdata)[0]);
	printf("power8: %x\n", Sigma0_32_p(vdata)[0]);
}

#ifndef MODE // 1 = scalar, 2 = vector, 3 = enhanced
#define MODE 3
#endif

#define _OP(x, sfx) Maj_32_##sfx  (x, x, x)

#if MODE == 1
#  define OP(x) _OP((x), s)
#elif MODE == 2
#  define OP(x) _OP((x), v)
#elif MODE == 3
#  define OP(x) _OP((x), p)
#endif

void timing_test()
{
	const int reps = 1 << 30;
	const int data = 42;

#if MODE == 1
	volatile uint32_t x = data;
#else
	volatile vu32_t x = {data, data, data, data};
#endif



	for (volatile int i = 0; i < reps; i++) {
		 x = OP(x);
	}


	printf("%d\n", x
#if MODE != 1
		   [0]
#endif
		   ); // = 42 :)
}

int main()
{
	timing_test();
	return 0;
}
