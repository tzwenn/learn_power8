#pragma once

#define USE_SSE 0
#define USE_ALTIVEC 1

#if USE_SSE
#include <emmintrin.h>
#endif

#if USE_ALTIVEC
#include <altivec.h>
#endif

#include "uint128_t.h"

////////////////////////////////////////////////////////////////////////////////
// Vector sum
////////////////////////////////////////////////////////////////////////////////

#if USE_SSE
static const __m128i vector2 = _mm_set_epi32(0, 2, 0, 2);

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE(__m128i &vectorCurrent, __m128i &vectorSum)
{
    vectorCurrent = _mm_add_epi64(vectorCurrent, vector2);
    vectorSum = _mm_add_epi64(vectorSum, vectorCurrent);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE2(__m128i &vectorCurrent, __m128i &vectorSum)
{
    sumSSE(vectorCurrent, vectorSum);
    sumSSE(vectorCurrent, vectorSum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE4(__m128i &vectorCurrent, __m128i &vectorSum)
{
    sumSSE2(vectorCurrent, vectorSum);
    sumSSE2(vectorCurrent, vectorSum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE8(__m128i &vectorCurrent, __m128i &vectorSum)
{
    sumSSE4(vectorCurrent, vectorSum);
    sumSSE4(vectorCurrent, vectorSum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE16(__m128i &vectorCurrent, __m128i &vectorSum)
{
    sumSSE8(vectorCurrent, vectorSum);
    sumSSE8(vectorCurrent, vectorSum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE32(__m128i &vectorCurrent, __m128i &vectorSum)
{
    sumSSE16(vectorCurrent, vectorSum);
    sumSSE16(vectorCurrent, vectorSum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumSSE64(__m128i &vectorCurrent, __m128i &vectorSum)
{
    sumSSE32(vectorCurrent, vectorSum);
    sumSSE32(vectorCurrent, vectorSum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sumBlock256Vector_SSE(uint64_t &start, uint128_t &result)
{
    // Risk of overflow: end >~ UINT64_MAX / 256 - 255

    uint64_t sum[2];

    __m128i vectorSum = _mm_set_epi64((__m64) start, (__m64) (start + 1));
    __m128i vectorCurrent = vectorSum;

    sumSSE(vectorCurrent, vectorSum);
    sumSSE2(vectorCurrent, vectorSum);
    sumSSE4(vectorCurrent, vectorSum);
    sumSSE8(vectorCurrent, vectorSum);
    sumSSE16(vectorCurrent, vectorSum);
    sumSSE32(vectorCurrent, vectorSum);
    sumSSE64(vectorCurrent, vectorSum);

    _mm_storeu_si128((__m128i *) sum, vectorSum);

    result += sum[0] + sum[1];
}

#endif // USE_SSE

#if USE_ALTIVEC

#endif // USE_ALTIVEC

////////////////////////////////////////////////////////////////////////////////
// Common sum
////////////////////////////////////////////////////////////////////////////////

inline void sum2(uint64_t &current, uint128_t &sum)
{
    sum += current;
    sum += ++current;
}

////////////////////////////////////////////////////////////////////////////////

inline void sum4(uint64_t &current, uint128_t &sum)
{
    sum2(current, sum);
    sum2(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sum8(uint64_t &current, uint128_t &sum)
{
    sum4(current, sum);
    sum4(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sum16(uint64_t &current, uint128_t &sum)
{
    sum8(current, sum);
    sum8(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sum32(uint64_t &current, uint128_t &sum)
{
    sum16(current, sum);
    sum16(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sum64(uint64_t &current, uint128_t &sum)
{
    sum32(current, sum);
    sum32(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sum128(uint64_t &current, uint128_t &sum)
{
    sum64(current, sum);
    sum64(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////

inline void sum256(uint64_t &current, uint128_t &sum)
{
    sum128(current, sum);
    sum128(++current, sum);
}

////////////////////////////////////////////////////////////////////////////////
// sumBlock256
////////////////////////////////////////////////////////////////////////////////

inline void sumBlock256(uint64_t &start, uint128_t &result)
{
    // Check for possible overflow in sumBlock256Vector() ...
#if USE_SSE
    if (256 * (start + 255) > start)
        sumBlock256Vector_SSE(start, result);
    // ... otherwise use the common way
    else
    {
#endif
        uint64_t current = start;
        sum256(current, result);
#if USE_SSE
    }
#endif
}
