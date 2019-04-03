#include "primitives/block.h"
#include "sha256.h"
#include "strutils.h"
#include <string>

uint256 CBlockHeader::get_hash() const
{
	return sha256(to_string());
}

std::string CBlockHeader::to_string() const
{
	return "CBlockHeader(" + hex_str(hash_prevblock) + ", " + hex_str(hash_merkleroot) + ", " + std::to_string(n_time) + ", " + std::to_string(n_bits) + ", " + std::to_string(nonce) + ")";
}

std::string CBlock::ToString() const
{
	std::string str;
	str += "CBlock(" + CBlockHeader::to_string() + ")";
	for(const auto &tx: vtx) {
		str += " " + tx.to_string() + "\n";
	}
	return str;
}
