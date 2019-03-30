#include "uint256.h"
#include "strutils.h"
#include <vector>
#include <string>
#include <cassert>
#include <cctype>

template <uint32_t BITS>
blob<BITS>::blob(const std::vector<unsigned char> &v)
{
	assert(v.size() == sizeof(data));
	memcpy(data, v.data(), sizeof(data));
}

template <uint32_t BITS>
std::string blob<BITS>::get_hex() const
{
	return hex_str(std::reverse_iterator<const uint8_t*>(data + sizeof(data)), std::reverse_iterator<const uint8_t*>(data));
}

template <uint32_t BITS>
void blob<BITS>::set_hex(const char *pchar)
{
	memset(data, 0, sizeof(data));

	// skip leading spaces
	while(*pchar++ == ' ');

	// skip 0x
	if(pchar[0] == '0' && tolower(pchar[1]) == 'x') {
		pchar += 2;
	}

	// convert hex string to uint array
	const char *pbegin = pchar;
	while(hexdigit[*pchar++] != -1);
	--pchar;
	// digits in data are in reverse order
	unsigned char *p1 = (unsigned char *)data;
	unsigned char *pend = p1 + width;
	while(pchar >= pbegin && p1 < pend) {
		*p1 = hexdigit[*pchar--];
		if(pchar >= pbegin) {
			*p1 |= ((unsigned char)hexdigit[*pchar--] << 4);
			p1++;
		}
	}
}

template <uint32_t BITS>
void blob<BITS>::set_hex(const std::string &str)
{
	set_hex(str.c_str());
}

template <uint32_t BITS>
std::string blob<BITS>::to_string() const
{
	return get_hex();
}

template blob<256>::blob(const std::vector<unsigned char> &);
template std::string blob<256>::get_hex() const;
template std::string blob<256>::to_string() const;
template void blob<256>::set_hex(const char *);
template void blob<256>::set_hex(const std::string &);
