#pragma once

#include <inttypes.h>
#include <string.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////
//
// uint128_t
//
////////////////////////////////////////////////////////////////////////////////

class uint128
{
	public:
		uint128()
		{
			m_data[0] = 0;
			m_data[1] = 0;
		}
		
        uint128(uint64_t low)
		{
			m_data[0] = low;
			m_data[1] = 0;
		}

		uint128 &operator +=(const uint128 &other)
		{
			m_data[0] += other.m_data[0];
			m_data[1] += other.m_data[1];
			
			m_data[1] += (m_data[0] < other.m_data[0]);
			
			return *this;
		}
		
		uint128 &operator +=(const uint64_t &other)
		{
		    m_data[0] += other;
			m_data[1] += (m_data[0] < other);
			
			return *this;
		}

		std::string str() const
		{
			// log10(x) = log2(x) / log2(10) ~= log2(x) / 3.322
			static char s[128 / 3 + 1 + 1];
			
			uint64_t n[2];
			
			char *p = s;
			int i;

			memset(s, '0', sizeof(s) - 1);
			s[sizeof(s) - 1] = '\0';

			memcpy(n, m_data, sizeof(n));

			for (i = 0; i < 128; i++)
			{
				int carry = (n[1] >= 0x8000000000000000);
				
				// Shift n[] left, doubling it
				n[1] = (n[1] << 1) + (n[0] >= 0x8000000000000000);
				n[0] = n[0] << 1;

				// Add s[] to itself in decimal, doubling it
				for (int j = sizeof(s) - 2; j >= 0; j--)
				{
					s[j] += s[j] - '0' + carry;

					carry = (s[j] > '9');

					if (carry)
						s[j] -= 10;
				}
			}

			while ((p[0] == '0') && (p < &s[sizeof(s) - 2]))
				p++;
			
			return p;
		}
		
		uint64_t m_data[2];
};

std::ostream & operator<<(std::ostream &ostream, const uint128 &number)
{
	ostream << number.str();
	
	return ostream;
}

typedef uint128 uint128_t;
