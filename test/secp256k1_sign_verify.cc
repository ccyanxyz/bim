#include <iostream>
#include "strutils.h"
#include "secp256k1.h"
#include "uint256.h"
#include "sha256.h"

using namespace std;

int main()
{
	secp256k1_context *secp256k1_context_sign = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

	unsigned char privkey[32];
	unsigned char pubkey[33];
	string key = "d0fe22f32190bd234a05190be4cc5cf763c4794807ac1303650faee1bc5577f3";
	string pub = "021c3d1fd4bb8f34572fdb43e4a0ef1e02c092858c25a1e3b066d9ac0dd2108130";

	for(int i = 0, j = 0; i < key.length(); i += 2, ++j) {
		privkey[j] = (hexdigit[key[i]] << 4) | hexdigit[key[i + 1]];
	}

	for(int i = 0, j = 0; i < pub.length(); i += 2, ++j) {
		pubkey[j] = (hexdigit[pub[i]] << 4) | hexdigit[pub[i + 1]];
	}

	string msg = "hello";
	uint256 hash = sha256(msg);

	secp256k1_ecdsa_signature sig;
	int ret = secp256k1_ecdsa_sign(secp256k1_context_sign, &sig, hash.begin(), privkey, secp256k1_nonce_function_rfc6979, nullptr);

	cout << "ret:" << ret << endl;
	cout << "sig:";
	cout << hex_str(sig.data, sig.data + 64) << endl;

	secp256k1_context *secp256k1_context_verify = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY);
	
	secp256k1_pubkey pub_key;
	int ret1 = secp256k1_ec_pubkey_parse(secp256k1_context_verify, &pub_key, pubkey, 33);
	
	cout << "ret1:" << ret1 << endl;
	
	int ret2 = secp256k1_ecdsa_verify(secp256k1_context_verify, &sig, hash.begin(), &pub_key);
	cout << "result:" << ret2 << endl;

	if(secp256k1_context_sign) {
		secp256k1_context_destroy(secp256k1_context_sign);
	}

	if(secp256k1_context_verify) {
		secp256k1_context_destroy(secp256k1_context_verify);
	}

	return 0;
}
