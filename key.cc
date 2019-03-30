#include "key.h"
#include "secp256k1.h"
#include "secp256k1_recovery.h"
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iostream>

static secp256k1_context *secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

void random_bytes(unsigned char *arr, int size)
{
	assert(arr);
	srand(time(nullptr));
	for(int i = 0; i < size; ++i) {
		arr[i] = rand() % 256;
	}
}

bool CKey::check(const unsigned char *arr)
{
	return secp256k1_ec_seckey_verify(secp256k1_context_sign, arr);
}

void CKey::new_key(bool compressed_in)
{
	do {
		random_bytes(key_data.data(), key_data.size());
	} while(!check(key_data.data()));
	valid = true;
	compressed = compressed_in;
}

CPrivKey CKey::get_private_key() const
{
	assert(valid);
	return key_data;
}

CPubKey CKey::get_public_key() const
{
	assert(valid);
	secp256k1_pubkey pubkey;
	size_t len = CPubKey::PUBLIC_KEY_SIZE;
	CPubKey result;
	int ret = secp256k1_ec_pubkey_create(secp256k1_context_sign, &pubkey, begin());
	assert(ret);
	secp256k1_ec_pubkey_serialize(secp256k1_context_sign, (unsigned char *)result.begin(), &len, &pubkey, compressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
	assert(result.size() == len);
	assert(result.is_valid());
	return result;
}

bool CKey::sign(const uint256 &hash, std::vector<unsigned char> &v_sig) const
{
	if(!valid) {
		return false;
	}
	v_sig.resize(CPubKey::SIGNATURE_SIZE);
	size_t siglen = CPubKey::SIGNATURE_SIZE;

	secp256k1_ecdsa_signature sig;
	int ret = secp256k1_ecdsa_sign(secp256k1_context_sign, &sig, hash.begin(), begin(), secp256k1_nonce_function_rfc6979, nullptr);
	assert(ret);
	memcpy(v_sig.data(), sig.data, siglen);
	return true;
}

bool CKey::sign_compact(const uint256 &hash, std::vector<unsigned char> &v_sig) const
{
	if(!valid) {
		return false;
	}
	v_sig.resize(CPubKey::COMPACT_SIGNATURE_SIZE);
	int rec = -1;
	secp256k1_ecdsa_recoverable_signature sig;
	int ret = secp256k1_ecdsa_sign_recoverable(secp256k1_context_sign, &sig, hash.begin(), begin(), secp256k1_nonce_function_rfc6979, nullptr);
	assert(ret);
	secp256k1_ecdsa_recoverable_signature_serialize_compact(secp256k1_context_sign, &v_sig[1], &rec, &sig);
	assert(rec != -1);
	v_sig[0] = 27 + rec + (compressed ? 4 : 0);
	return true;
}

bool CKey::verify_pubkey(const CPubKey &pubkey) const
{
	if(pubkey.is_compressed() != compressed) {
		return false;
	}
	std::string str = "public key verification";
	uint256 hash = sha256(str);
	std::vector<unsigned char> sig;
	sign(hash, sig);
	std::cout << "msg hash:";
	std::cout << hash.get_hex() << std::endl;
	std::cout << "sig:";
	std::cout << hex_str(sig.begin(), sig.end()) << std::endl;
	return pubkey.verify(hash, sig);
}
