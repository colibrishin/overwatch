#ifndef HASH_H
#define HASH_H

#include "sha256.h"
#define SHA256_HASH_LENGTH 65

namespace Hash {
	using SHA256 = char[SHA256_HASH_LENGTH];

	bool compareHash(const Hash::SHA256& s1, const Hash::SHA256& s2) noexcept;
	void calcHash(const char* data, Hash::SHA256 digest);
}

#endif