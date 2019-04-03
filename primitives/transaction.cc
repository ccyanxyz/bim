#include "primitives/transaction.h"
#include "sha256.h"
#include "strutils.h"
#include <string>

std::string COutPoint::to_string() const
{
	return "COutPoint(" + hash.to_string().substr(0, 10) + ", " + std::to_string(index) + ")";
}

CTxIn::CTxIn(COutPoint prevout_in, CScript script_sig_in)
{
	prevout = prevout_in;
	script_sig = script_sig_in;
}

CTxIn::CTxIn(uint256 hash_prevtx, uint32_t index, CScript script_sig_in)
{
	prevout = COutPoint(hash_prevtx, index);
	script_sig = script_sig_in;
}

std::string to_string() const
{
	std::string str;
	str += "CTxIn(";
	str += prevout.to_string();
	if(prevout.is_null()) {
		str += ", coinbase " + hex_str(script_sig);
	} else {
		str += ", script_sig=", hex_str(script_sig).substr(0, 24);
	}
	str += ")";
	return str;
}

CTxOut::CTxOut(const int64_t value_in, CScript script_pubkey_in)
{
	value = value_in;
	script_pubkey = script_pubkey_in;
}

std::string CTxOut::to_string()
{
	return "CTxOut(value=" + std::to_string(value) + ", script_pubkey=" + hex_str(script_pubkey).substr(0, 30); 
}

uint256 CTransaction::compute_hash() const
{
	return sha256(to_string());
}

CTransaction::CTransaction(): vin(), vout() {  }

int64_t CTransaction::get_value_out() const
{
	int64_t value_out = 0;
	for(const auto &tx_out: vout) {
		value_out += tx_out.value;
	}
	return value_out;
}

std::string CTransaction::to_string() const
{
	std::string str;
	str += "CTransaction(vin.size=" + std::to_string(vin.size()) + ", vout.size=" + std::to_string(vout.size());
	for(const auto &tx_in: vin) {
		str += " " + tx_in.to_string() + "\n";
	}
	for(const auto &tx_out: vout) {
		str += " " + tx_out.to_string() + "\n";
	}
	return str;
}
