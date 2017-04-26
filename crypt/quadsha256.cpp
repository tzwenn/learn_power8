/* Originally based upon the SHA-256 implementation from
 *		https://github.com/svenstucki/sha256
 */

#include <cstdio>
#include <iostream>

#include <vector>
#include <array>
#include <tuple>

#include <functional>
#include <chrono>

#include <omp.h>

#include "powersha.h"

class msg_tuple: public std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>
{
	using basetype = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;

public:
	msg_tuple(const uint8_t data) :
		basetype(data, data, data, data)
	{ }

	msg_tuple(const uint8_t r0, const uint8_t r1, const uint8_t r2, const uint8_t r3) :
		basetype(r0, r1, r2, r3)
	{ }
};

class SHA256Hasher
{
public:
#if ENABLE_QUAD
	using msg_element_t = msg_tuple;
	using field_t = pwr::Vecmm;
#else
	using msg_element_t = uint8_t;
	using field_t = uint32_t;
#endif
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

#if ENABLE_QUAD
		for (int i = 0; i < 4; i++) {
			for (std::size_t j = 0; j < m_hash.size(); j++) {
				printf("%08x", m_hash[j].d()[i]);
			}
			puts("");
		}
#else
		for (std::size_t i = 0; i < m_hash.size(); i++) {
			printf("%08x", m_hash[i]);
		}
		puts("");
#endif
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
	hash_t m_hash;

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
#if ENABLE_QUAD
			field_t::vectype wi = {
				(uint32_t)(std::get<0>(msg[i*4 + 3]) | (std::get<0>(msg[i*4 + 2]) << 8) | (std::get<0>(msg[i*4 + 1]) << 16) | (std::get<0>(msg[i*4 + 0]) << 24)),
				(uint32_t)(std::get<1>(msg[i*4 + 3]) | (std::get<1>(msg[i*4 + 2]) << 8) | (std::get<1>(msg[i*4 + 1]) << 16) | (std::get<1>(msg[i*4 + 0]) << 24)),
				(uint32_t)(std::get<2>(msg[i*4 + 3]) | (std::get<2>(msg[i*4 + 2]) << 8) | (std::get<2>(msg[i*4 + 1]) << 16) | (std::get<2>(msg[i*4 + 0]) << 24)),
				(uint32_t)(std::get<3>(msg[i*4 + 3]) | (std::get<3>(msg[i*4 + 2]) << 8) | (std::get<3>(msg[i*4 + 1]) << 16) | (std::get<3>(msg[i*4 + 0]) << 24))
			};
			W[i] = wi;
#else
			W[i] = msg[i*4 + 3] | (msg[i*4 + 2] << 8) | (msg[i*4 + 1] << 16) | (msg[i*4 + 0] << 24);
#endif
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
	SHA256Hasher::msg_t input(80, 42);
	const int repeats = 10000000;
	const double simd_width = sizeof(SHA256Hasher::msg_element_t);

	auto calcKHash = [&](double secs) {
		return (simd_width * repeats * 0.001) / secs;
	};

#if ENABLE_QUAD
	input[0] = msg_tuple(0, 1, 2, 3);
#else
	input[0] = 0;
#endif

	// Create thread pool to remove overhead in timing measurement below
	#pragma omp parallel for
	for (int i = 0; i < omp_get_max_threads(); i++) {
		asm volatile ("":::"memory");
	}
	std::cout << "threads: " << omp_get_max_threads() << std::endl;
	std::cout << "repeats: " << repeats << std::endl;
	std::cout << "simd_wd: " << simd_width << std::endl;

	std::vector<SHA256Hasher> hashers(omp_get_max_threads(), SHA256Hasher(input));

	std::cout << calcKHash(time_it([&]{
		#pragma omp parallel for
		for (int i = 0; i < repeats; i++) {
			hashers[omp_get_thread_num()].calc_final();
		}
	})) << " khash/s" << std::endl;

	return 0;
}

