/* SHA-256 implementation from
 *		https://github.com/svenstucki/sha256
 */

#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>

#include <vector>
#include <array>

#include "powersha.h"

// SHA-224/256 constants
const uint32_t K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

class SHA256Hasher
{
public:
	using bytes_t = std::vector<uint8_t>;
	using buffer_block_t = std::array<uint8_t, 64>;

	using field_t = uint32_t;
	using block_t = std::array<field_t, 64>;
	using hash_t = std::array<field_t, 8>;

	SHA256Hasher(const bytes_t & initialbuffer = bytes_t()) :
		m_buffer(initialbuffer)
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

	void update(const bytes_t & buffer)
	{
		m_buffer.reserve(m_buffer.size() + buffer.size());
		m_buffer.insert(m_buffer.end(), buffer.begin(), buffer.end());
	}

	void printhex()
	{
		for (std::size_t i = 0; i < m_hash.size(); i++) {
			printf("%08x", m_hash[i]);
		}
		puts("");
	}

	void calc_final()
	{
		uint8_t *buf = m_buffer.data();
		for (std::size_t i = 0; i + 64 < m_buffer.size(); i += 64) {
			process_block(&buf[i]);
		}

		std::size_t i, len = m_buffer.size() % 64;

		buffer_block_t buffer;
		memset(buffer.data(), 0, buffer.size());
		memcpy(buffer.data(), m_buffer.data() + 64 * (m_buffer.size() / 64), len);

		if (len < 56) {
			buffer[len] = 0x80;
			i = len + 1;
		} else {
			buffer[len] = 0x80;
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

private:
	bytes_t m_buffer;
	hash_t m_hash;

	void process_block(const uint8_t *msg) {
		field_t a, b, c, d, e, f, g, h;
		field_t T1, T2;
		block_t W; // __attribute__((aligned(16)));

		unsigned int i;

		// prepare W
		for (i = 0; i < 16; i++) {
			W[i] = msg[i*4 + 3] | (msg[i*4 + 2] << 8) | (msg[i*4 + 1] << 16) | (msg[i*4 + 0] << 24);
		}


		for (i = 16; i < 64; i++) {
			W[i] = pwr::sigma<pwr::lowercase, pwr::sigma1>(W[i-2]) + W[i-7] + pwr::sigma<pwr::lowercase, pwr::sigma0>(W[i-15]) + W[i-16];
		}

		a = m_hash[0];
		b = m_hash[1];
		c = m_hash[2];
		d = m_hash[3];
		e = m_hash[4];
		f = m_hash[5];
		g = m_hash[6];
		h = m_hash[7];

		for (i = 0; i < 64; i++) {
			T1 = h + pwr::sigma<pwr::uppercase, pwr::sigma1>(e) + pwr::Ch(e, f, g) + K[i] + W[i];
			T2 = pwr::sigma<pwr::uppercase, pwr::sigma0>(a) + pwr::Maj(a, b, c);
			h = g;
			g = f;
			f = e;
			e = d + T1;
			d = c;
			c = b;
			b = a;
			a = T1 + T2;
		}

		// compute intermediate hash
		m_hash[0] = a + m_hash[0];
		m_hash[1] = b + m_hash[1];
		m_hash[2] = c + m_hash[2];
		m_hash[3] = d + m_hash[3];
		m_hash[4] = e + m_hash[4];
		m_hash[5] = f + m_hash[5];
		m_hash[6] = g + m_hash[6];
		m_hash[7] = h + m_hash[7];
	}
};


int main(int argc, char *argv[])
{
	// Make input fields
	SHA256Hasher::bytes_t input(767, 42);

	input[0] = 0;
	SHA256Hasher h0(input);
	input[0] = 1;
	SHA256Hasher h1(input);
	input[0] = 2;
	SHA256Hasher h2(input);
	input[0] = 3;
	SHA256Hasher h3(input);

	h0.calc_final();
	h1.calc_final();
	h2.calc_final();
	h3.calc_final();

	h0.printhex(); // f59ff41a8c3ddf325b1c0789a15aae114838c2d95a9294642562796e616482da
	h1.printhex(); // 6a50420aeafcf735705c55ad0776a496db056b2fb78228aa67ce1c8ed8cb918e
	h2.printhex(); // 6f0bdedb20589983c9f5ae33a4f4e7cc66e617c013eb042961c8d802f0e183c2
	h3.printhex(); // 101ce26ffd75f4bca024fc565798716c2f768b311221182b440e4c8ce4cbd78e
}

