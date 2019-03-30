#include "pubkey.h"
#include "secp256k1.h"
#include "secp256k1_recovery.h"

static secp256k1_context *secp256k1_context_verify = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);

bool CPubKey::is_fully_valid() const
{
	if(!is_valid()) {
		return false;
	}
	secp256k1_pubkey pub_key;
	return secp256k1_ec_pubkey_parse(secp256k1_context_verify, &pub_key, pubkey, size());
}

bool CPubKey::verify(const uint256 &hash, const std::vector<unsigned char> &v_sig) const
{
	if(!is_valid())
		return false;
	secp256k1_pubkey pub_key;
	secp256k1_ecdsa_signature sig;
	if(!secp256k1_ec_pubkey_parse(secp256k1_context_verify, &pub_key, pubkey, size())) {
		return false;
	}

	memcpy(sig.data, v_sig.data(), v_sig.size());

	return secp256k1_ecdsa_verify(secp256k1_context_verify, &sig, hash.begin(), &pub_key);
}

bool CPubKey::recover_compact(const uint256 &hash, const std::vector<unsigned char> &v_sig)
{
	if(v_sig.size() != COMPACT_SIGNATURE_SIZE) {
		return false;
	}
	int recid = (v_sig[0] - 27) & 3;
	bool comp = ((v_sig[0] - 27) & 4) != 0;

	secp256k1_pubkey pub_key;
	secp256k1_ecdsa_recoverable_signature sig;
	if(!secp256k1_ecdsa_recoverable_signature_parse_compact(secp256k1_context_verify, &sig, &v_sig[1], recid)) {
		return false;
	}
	if(!secp256k1_ecdsa_recover(secp256k1_context_verify, &pub_key, &sig, hash.begin())) {
		return false;
	}

	unsigned char pub[PUBLIC_KEY_SIZE];
	size_t publen = PUBLIC_KEY_SIZE;
	secp256k1_ec_pubkey_serialize(secp256k1_context_verify, pub, &publen, &pub_key, comp ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
	set(pub, pub + publen);
	return true;
}

bool CPubKey::decompress()
{
	if(!is_valid()) {
		return false;
	}

	secp256k1_pubkey pub_key;
	if(!secp256k1_ec_pubkey_parse(secp256k1_context_verify, &pub_key, pubkey, size())) {
		return false;
	}

	unsigned char pub[PUBLIC_KEY_SIZE];
	size_t publen = PUBLIC_KEY_SIZE;
	secp256k1_ec_pubkey_serialize(secp256k1_context_verify, pub, &publen, &pub_key, SECP256K1_EC_UNCOMPRESSED);
	set(pub, pub + publen);
	return true;
}
