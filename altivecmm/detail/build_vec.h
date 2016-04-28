#pragma once

#include "../typetable.h"

namespace altivecmm {

	namespace detail {

		// C++ doesn't want me to partially specialize functions. So wrap a struct around it
		template<std::size_t elem_count, typename elemtype>
		struct build_vec;

		template<typename elemtype>
		struct build_vec<2, elemtype> {
			typename typetable<elemtype>::vectype operator() (const elemtype & r0, const elemtype & r1)
			{ return (typename typetable<elemtype>::vectype) {r0, r1}; }
		};

		template<typename elemtype>
		struct build_vec<4, elemtype> {
			typename typetable<elemtype>::vectype operator() (const elemtype & r0, const elemtype & r1, const elemtype & r2, const elemtype & r3)
			{ return (typename typetable<elemtype>::vectype) {r0, r1, r2, r3}; }
		};

		template<typename elemtype>
		struct build_vec<8, elemtype> {
			typename typetable<elemtype>::vectype operator() (const elemtype & r0, const elemtype & r1, const elemtype & r2, const elemtype & r3, const elemtype & r4, const elemtype & r5, const elemtype & r6,  const elemtype & r7)
			{ return (typename typetable<elemtype>::vectype) {r0, r1, r2, r3, r4, r5, r6, r7}; }
		};

		template<typename elemtype>
		struct build_vec<16, elemtype> {
			typename typetable<elemtype>::vectype operator() (const elemtype & r0, const elemtype & r1, const elemtype & r2, const elemtype & r3, const elemtype & r4, const elemtype & r5, const elemtype & r6,  const elemtype & r7,  const elemtype & r8,  const elemtype & r9,  const elemtype & r10,  const elemtype & r11,  const elemtype & r12,  const elemtype & r13,  const elemtype & r14,  const elemtype & r15)
			{ return (typename typetable<elemtype>::vectype) {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15}; }
		};
	 }

}
