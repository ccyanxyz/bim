#ifndef PRIMITIVES_BLOCK_H
#define PRIMITIVES_BLOCK_H

#include "primitives/transaction.h"
#include "uint256.h"

class CBlockHeader {
public:
	uint256 hash_prevblock;
	uint256 hash_merkleroot;
	uint32_t n_time;
	uint32_t n_bits; // difficulty target
	uint32_t nonce;

	CBlockHeader()
	{
		set_null();
	}

	void set_null()
	{
		hash_prevblock.set_null();
		hahs_merkleroot.set_null();
		n_time = 0;
		n_bits = 0;
		nonce = 0;
	}

	bool is_null() const
	{
		return n_bits == 0;
	}

	uint256 get_hash() const;
	std::string to_string() const;

	int64_t get_block_time() const
	{
		return (int64_t) n_time;
	}
};

class CBlock: public CBlockHeader {
public:
	std::vector<CTransaction> vtx;

	bool checked;

	CBlock()
	{
		set_null();
	}

	CBlock(const CBlockHeader &header)
	{
		set_null();
		*(static_cast<CBlockHeader *>(this)) = header;
	}

	void set_null()
	{
		CBlockHeader::set_null();
		vtx.clear();
		checked = false;
	}

	CBlockHeader get_block_header() const
	{
		CBlockHeader block;
		block.hash_prevblock = hash_prevblock;
		block.hash_merkleroot = hash_merkleroot;
		block.n_time = n_time;
		block.n_bits = n_bits;
		block.nonce = nonce;
		return block;
	}
	
	std::string to_string() const;
};

#endif
