#ifndef KEY_H
#define KEY_H

#include "uint256.h"
#include "pubkey.h"
#include <vector>
#include <stdexcept>

typedef std::vector<unsigned char, std::alloctor<unsigned char> > CPrivKey;

class CKey {
public:
	static const unsigned int PRIVATE_KEY_SIZE = 256;

private:
	// whether private key is valid
	bool valid;
	// whether public key compressed
	bool compressed;
	
	// private key data
	std::vector<unsigned char, std::allocator<unsigned char> > key_data;

	// check if data in v is valid key data
	bool static check(const unsigned char *v);

public:
	// construct an invalid private key
	CKey(): valid(false), compressed(false)
	{
		key_data.resize(32);
	}

	friend bool operator==(const CKey &a, const CKey &b)
	{
		return a.compressed == b.compressed && a.size() == b.size() &&
			memcmp(a.key_data.data(), b.key_data.data(), a.size()) == 0;
	}

	// initialize with iterators
	template <typename T>
	void set(const T begin, const T end, bool compressed_in) {
		if(size_t(end - begin) != key_data.size()) {
			valid = false;
		} else if (check(&begin[0])) {
			memcpy(key_data.data(), (unsigned char *)&begin[0], key_data.size());
			valid = true;
			compressed = compressed_in;
		} else {
			valid = false;
		}
	}

	// get size
	unsigned int size() const {
		return valid ? key_data.size() : 0;
	}

	// iterators
	const unsigned char *begin()
	{
		return key_data.data();
	}
	
	const unsigned char *end()
	{
		return key_data.data() + size();
	}

	// check if this private is valid
	bool is_valid() const
	{
		return valid;
	}

	// generate a new private key
	void new_key();

	CPrivKey get_private_key() const;
	CPubKey get_public_key() const;

	// sign
	bool sign(const uint256 &hash, std::vector<unsigned char> &v_sig, bool grind = true) const;

	// sign compact
	bool sign_compact(const uint256 &hash, std::vector<unsigned char> &v_sig) const;

	// verify if this private key match the public key
	bool verify_pubkey(const CPubKey &pubkey) const;
};


#endif
