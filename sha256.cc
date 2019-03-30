#include <openssl/sha.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include "sha256.h"

uint256 sha256(const std::string &str) 
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    return uint256(hash, SHA256_DIGEST_LENGTH);
}

uint256 sha256(const unsigned char *arr, const int size)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, arr, size);
	SHA256_Final(hash, &sha256);
	return uint256(hash, SHA256_DIGEST_LENGTH);
}
