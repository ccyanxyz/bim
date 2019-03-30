#ifndef PUBKEY_H
#define PUBKEY_H

#include "uint256.h"
#include "sha256.h"
#include <vector>

class CPubKey {
public:
	static constexpr unsigned int PUBLIE_KEY_SIZE = 65;
	static constexpr unsigned int COMPRESSED_PUBLIC_KEY_SIZE = 33;
	static constexpr unsigned int SIGNATURE_SIZE = 72;
	static constexpr unsigned int COMPACT_SIGNATURE_SIZE = 65;

	static_assert(PUBLIC_KEY_SIZE >= COMPRESSED_PUBLIC_KEY_SIZE,
			"COMPREESSED_PUBLIC_KEY_SIZE is larger than PUBLIC_KEY_SIZE");

private:
	unsigned char pubkey[PUBLIC_KEY_SIZE];

	unsigned int static get_len(unsigned char header)
	{
		if (header == 2 || header == 3) {
			return COMPREESSED_PUBLIC_KEY_SIZE;
		}

		if(header == 4 || header == 6 || header == 7) {
			return PUBLIC_KEY_SIZE;
		}

		return 0;
	}

	void invalidate()
	{
		pubkey[0] = 0xFF;
	}

public:
	// construct an invalid public key
	CPubKey()
	{
		invalidate();
	}

	// initialize a public key with iterators
	template <typename T>
	void set(const T begin, const T end)
	{
		int len = end == begin ? 0 : int(end - begin);
		if(len && len == get_len(begin[0])) {
			memcpy(pubkey, (unsigned char *) &begin[0], len);
		} else {
			invalidate();
		}
	}

	// construct public key using iterators
	template <typename T>
	CPubKey(const T begin, const T end)
	{
		set(begin, end);
	}

	// construct public key from a vector
	explicit CPubKey(const std::vector<unsigned char> &v)
	{
		set(v.begin(), v.end());
	}

	// get size
	unsigned int size() {
		return get_len(pubkey[0]);
	}

	// get pubkey data
	const unsigned char *data() const
	{
		return pubkey;
	}

	// iterators
	const unsigned char *begin() const
	{
		return pubkey;
	}

	const unsigned char *end() const
	{
		return pubkey + size();
	}

	const unsigned char &operator[](unsigned int pos) const 
	{
		return pubkey[pos];
	}

	// comparator
	friend bool operator==(const CPubKey &a, const CPubKey &b)
	{
		return a.pubkey[0] == b.pubkey[0] && 
			memcmp(a.pubkey, b.pubkey, a.size()) == 0;
	}

	friend bool operator!=(const CPubKey &a, const CPubKey &b)
	{
		return !(a == b);
	}

	friend bool operator<(const CPubKey &a, const CPubKey &b)
	{
		return a.pubkey[0] < b.pubkey[0] || 
			(a.pubkey[0] == b.pubkey[0] && memcmp(a.pubkey, b.pubkey, a.size()) < 0);
	}

	// get hash of pubkey
	uint256 get_hash() const
	{
		return sha256(pubkey, size());
	}

	bool is_valid() const
	{
		return size() > 0;
	}

	bool is_fully_valid() const;

	bool is_compressed() const
	{
		return size() == COMPRESSED_PUBLIC_KEY_SIZE;
	}

	// verify signature
	bool verify(const uint256 &hash, const std::vector<unsigned char> &sig) const;

	// recover a pubkey from compact signature
	bool recover_compact(const uint256 &hash, const std::vector<unsigned char> &sig);

	// decompress pubkey
	bool decompress();
};

#endif
