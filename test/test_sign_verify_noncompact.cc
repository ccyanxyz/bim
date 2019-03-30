#include <vector>
#include <iostream>
#include "sha256.h"
#include "pubkey.h"
#include "key.h"
#include "strutils.h"

using namespace std;

int main()
{
	// get a new private key
	CKey privkey;

	privkey.new_key(false);

	CPrivKey key = privkey.get_private_key();
	CPubKey pubkey = privkey.get_public_key();

	cout << "private key:";
	cout << hex_str(key.begin(), key.end()) << endl;

	cout << "public key:";
	cout << hex_str(pubkey.begin(), pubkey.end()) << endl;
	cout << endl;

	cout << "verify_pubkey:";
	cout << privkey.verify_pubkey(pubkey) << endl;

	string msg = "hello";
	uint256 hash = sha256(msg);
	vector<unsigned char> v_sig;
	bool ret1 = privkey.sign_compact(hash, v_sig);
	cout << "sign compact ret1:" << ret1 << endl;

	CPubKey pubkey1;
	bool ret2 = pubkey1.recover_compact(hash, v_sig);
	cout << "recover compact ret2:" << ret2 << endl;
	cout << "recovered pubkey:" << hex_str(pubkey1.begin(), pubkey1.end()) << endl;

	return 0;
}
