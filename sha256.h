#ifndef SHA256_H
#define SHA256_H

#include "uint256.h"

uint256 sha256(const std::string &);
uint256 sha256(const unsigned char *, const int);

#endif
