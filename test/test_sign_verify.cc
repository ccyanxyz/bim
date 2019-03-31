#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "sha256.h"
#include "pubkey.h"
#include "key.h"
#include "strutils.h"

using namespace std;

TEST(keytest, compact)
{
	CKey privkey;
	privkey.new_key(true);

	EXPECT_EQ(privkey.is_valid(), true);

	CPubKey pubkey = privkey.get_public_key();
	EXPECT_EQ(pubkey.is_valid(), true);
	EXPECT_EQ(pubkey.is_fully_valid(), true);
	EXPECT_EQ(pubkey.is_compressed(), true);

	EXPECT_EQ(privkey.verify_pubkey(pubkey), true);

	string msg = "hello";
	uint256 hash = sha256(msg);
	vector<unsigned char> v_sig;
	EXPECT_EQ(privkey.sign_compact(hash, v_sig), true);
	
	CPubKey pubkey1;
	EXPECT_EQ(pubkey1.recover_compact(hash, v_sig), true);
	EXPECT_EQ(pubkey, pubkey1);
}

TEST(keytest, noncompact)
{
	CKey privkey;
	privkey.new_key(false);

	EXPECT_EQ(privkey.is_valid(), true);

	CPubKey pubkey = privkey.get_public_key();
	EXPECT_EQ(pubkey.is_valid(), true);
	EXPECT_EQ(pubkey.is_fully_valid(), true);
	EXPECT_EQ(pubkey.is_compressed(), false);

	EXPECT_EQ(privkey.verify_pubkey(pubkey), true);

	string msg = "hello";
	uint256 hash = sha256(msg);
	vector<unsigned char> v_sig;
	EXPECT_EQ(privkey.sign_compact(hash, v_sig), true);
	
	CPubKey pubkey1;
	EXPECT_EQ(pubkey1.recover_compact(hash, v_sig), true);
	EXPECT_EQ(pubkey, pubkey1);
}

int main(int argc, char *argv[])
{	
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
