/* SHA-256 implementation from
 *		https://github.com/svenstucki/sha256
 */

#include <cstdio>
#include <iostream>

#include <vector>
#include <array>
#include <tuple>

#include <functional>
#include <chrono>

#define ENABLE_QUAD 0
#include "powersha.h"

class SHA256Hasher
{
public:
	using msg_element_t = uint8_t;
	using field_t = uint32_t;
	using msg_t = std::vector<msg_element_t>;

	using block_t = std::array<field_t, 64>;
	using hash_t = std::array<field_t, 8>;

	SHA256Hasher(const msg_t & initialbuffer = msg_t()) :
		m_buffer(initialbuffer)
	{
		init();
	}

	void printhex()
	{
		for (std::size_t j = 0; j < m_hash.size(); j++) {
			printf("%08x", m_hash[j]);
		}
		puts("");
	}

	void calc_final()
	{
		for (std::size_t i = 0; i + 64 < m_buffer.size(); i += 64) {
			process_block(&m_buffer.data()[i]);
		}

		std::size_t i, len = m_buffer.size() % 64;

		msg_t buffer(64, 0x00);
		auto lastSlice = m_buffer.begin() + 64 * (m_buffer.size() / 64);
		std::copy(lastSlice, lastSlice + len, buffer.begin());

		if (len < 56) {
			buffer[len] = 0x80;
			i = len + 1;
		} else {
			buffer[len] = 0x80;
			for (i = len + 1; i < 64; i++) {
				buffer[i] = 0x00;
			}
			process_block(buffer.data());
			i = 0;
		}
		for (; i < 56; i++) {
			buffer[i] = 0x00;
		}

		const auto bits = m_buffer.size() * 8;
		for (i = 0; i < 8; i++) {
			buffer[63 - i] = bits >> (i * 8);
		}
		process_block(buffer.data());
	}

protected:
	static const field_t K[64];

	msg_t m_buffer;
	hash_t m_hash __attribute__((aligned(16)));

	void init()
	{
		m_hash[0] = 0x6a09e667;
		m_hash[1] = 0xbb67ae85;
		m_hash[2] = 0x3c6ef372;
		m_hash[3] = 0xa54ff53a;
		m_hash[4] = 0x510e527f;
		m_hash[5] = 0x9b05688c;
		m_hash[6] = 0x1f83d9ab;
		m_hash[7] = 0x5be0cd19;
	}

	void loadMsgIntoW(const msg_element_t *msg, block_t & W)
	{
		for (std::size_t i = 0; i < 16; i++) {
			W[i] = msg[i*4 + 3] | (msg[i*4 + 2] << 8) | (msg[i*4 + 1] << 16) | (msg[i*4 + 0] << 24);
		}
	}

	void process_block(const msg_element_t *msg)
	{
		field_t a, b, c, d, e, f, g, h;
		field_t T1, T2;
		block_t W __attribute__((aligned(16)));

		unsigned int i;

		loadMsgIntoW(msg, W);

		for (i = 16; i < 64; i++) {
			W[i] = pwr::sigma<pwr::lowercase, pwr::index1>(W[i-2]) + W[i-7] + pwr::sigma<pwr::lowercase, pwr::index0>(W[i-15]) + W[i-16];
		}

		a = m_hash[0];
		b = m_hash[1];
		c = m_hash[2];
		d = m_hash[3];
		e = m_hash[4];
		f = m_hash[5];
		g = m_hash[6];
		h = m_hash[7];
		pwr::vecu32_t h0123 = vec_ld(0, m_hash.data());
		pwr::vecu32_t h4567 = vec_ld(4 * sizeof(uint32_t), m_hash.data());
		pwr::vecu32_t nullvec = {0, 0, 0, 0};

		pwr::vecu32_t abcd = h0123;
		pwr::vecu32_t efgh = h4567;

		static bool has_printed = false;
		if (!has_printed) {
			has_printed = true;
			std::cout << std::hex << pwr::Vecmm(abcd) << ", " << pwr::Vecmm(efgh) << std::dec << std::endl;
		}

		for (i = 0; i < 64; i++) {
			T1 = h + pwr::sigma<pwr::uppercase, pwr::index1>(e) + pwr::Ch(e, f, g) + K[i] + W[i];
			T2 = pwr::sigma<pwr::uppercase, pwr::index0>(a) + pwr::Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;

			// Left shift does actually right shift. How nice.
			efgh = vec_sld(efgh, abcd, 4);
			abcd = vec_sld(abcd, nullvec, 4);

			pwr::vecu32_t _t1t2_000 = {T1 + T2, 0, 0, 0};
			pwr::vecu32_t   _t1_000 = {T1     , 0, 0, 0};

			//pwr::vecu32_t t2xxx = __builtin_crypto_vshasigmaw(abcd, 1, 0);

			abcd = vec_add(abcd, _t1t2_000);
			efgh = vec_add(efgh,   _t1_000);

			static bool has_printed2 = false;
			if (!has_printed2) {
				has_printed2 = true;
				std::cout << std::hex << pwr::Vecmm(abcd) << ", " << pwr::Vecmm(efgh) << std::dec << std::endl;
			}

		}
#if 1
		h0123 = vec_add(abcd, h0123);
		h4567 = vec_add(efgh, h4567);

		vec_st(h0123, 0, m_hash.data());
		vec_st(h4567, 4 * sizeof(uint32_t), m_hash.data());
#else
		// compute intermediate hash
		m_hash[0] = a + m_hash[0];
		m_hash[1] = b + m_hash[1];
		m_hash[2] = c + m_hash[2];
		m_hash[3] = d + m_hash[3];
		m_hash[4] = e + m_hash[4];
		m_hash[5] = f + m_hash[5];
		m_hash[6] = g + m_hash[6];
		m_hash[7] = h + m_hash[7];
#endif
	}
};

// SHA-224/256 constants
const SHA256Hasher::field_t SHA256Hasher::K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

auto time_it = [](std::function<void()>  f){
	auto lastTime = std::chrono::high_resolution_clock::now();
	f();
	std::chrono::duration<float> d(std::chrono::high_resolution_clock::now() - lastTime);
	return d.count();
};

int main(int argc, char *argv[])
{
	SHA256Hasher::msg_t input(767, 42);

	input[0] = 0;
	SHA256Hasher h0(input);

	std::cout << time_it([&]{
		h0.calc_final();
	}) * 1000 * 1000 << " µs" << std::endl;

	h0.printhex(); // f59ff41a8c3ddf325b1c0789a15aae114838c2d95a9294642562796e616482da
	return 0;
}
