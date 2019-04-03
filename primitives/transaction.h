#ifndef PRIMITIVES_TRANSACTION_H
#define PRIMITIVES_TRANSACTION_H

#include <vector>
#include "uint256.h"
#include "script/script.h"

class COutPoint {
public:
	// transaction hash
	uint256 hash;
	// vout index
	uint32_t index;

	// -1 overflow
	COutPoint(): n((uint32_t) -1) {  }
	COutPoint(const uint256 &hash_in, uint32_t idx_in): hash(hash_in), index(idx_in) {  }

	void set_null()
	{
		hash.set_null();
		n = (uint32_t) -1;
	}

	bool is_null()
	{
		return (hash.is_null && n == (uint32_t) -1);
	}

	friend bool operator<(const COutPoint &a, const COutPoint &b)
	{
		int cmp = a.hash.compare(b.hash);
		return cmp < 0 || (cmp == 0 && a.n < b.n);
	}

	friend bool operator==(const COutPoint &a, const COutPoint &b)
	{
		return a.hash == b.hash && a.n == b.n;
	}

	friend bool operator!=(const COutPoint &a, const COutPoint &b)
	{
		return !(a == b);
	}

	std::string to_string() const;
};

class CTxIn {
public:
	COutPoint prevout;
	CScript script_sig;

	CTxIn() {  }
	explicit CTxIn(COutPoint prevout_in, CScript script_sig_in = CScript());
	CTxIn(uint256 hash_prevtx, uint32_t index, CScript script_sig_in = CScript());

	friend bool operator==(const CTxIn &a, const CTxIn &b)
	{
		return a.prevout == b.prevout && a.script_sig == b.script_sig;
	}

	friend bool operator!=(const CTxIn &a, const CTxIn &b)
	{
		return !(a == b);
	}
	
	std::string to_string() const;
};

class CTxOut {
public:
	int64_t value;
	CScript script_pubkey;

	CTxOut()
	{
		set_null();
	}

	CTxOut(const int64_t &value_in, CScript script_pubkey_in);

	void set_null()
	{
		value = -1;
		script_pubkey.clear();
	}

	bool is_null() const
	{
		return value == -1;
	}

	friend bool operator==(const CTxOut &a, const CTxOut &b)
	{
		return a.value == b.value && a.script_pubkey == b.script_pubkey;
	}

	friend bool operator!=(const CTxOut &a, const CTxOut &b)
	{
		return !(a == b);
	}

	std::string to_string() const;
};

class CTransaction {
public:
	const std::vector<CTxIn> vin;
	const std::vector<CTxOut> vout;

private:
	// memory only
	const uint256 hash;

	uint256 compute_hash() const;

public:
	CTransaction();

	bool is_null() const
	{
		return vin.empty() && vout.empty();
	}

	const uint256 &get_hash() const
	{
		return hash;
	}

	// return sum of txouts
	int64_t get_value_out() const;

	bool is_coinbase() const
	{
		// why???
		return vin.size() == 1 && vin[0].prevout.is_null();
	}

	friend bool operator==(const CTransaction &a, const CTransaction &b)
	{
		return a.hash == b.hash;
	}

	friend bool operator!=(const CTransaction &a, const CTransaction &b)
	{
		return !(a == b);
	}

	std::string to_string() const;
};

#endif
