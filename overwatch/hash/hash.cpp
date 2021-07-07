#include "../hash/hash.hpp"

#include <string>

bool Hash::compareHash(const Hash::SHA256& s1, const Hash::SHA256& s2) noexcept {
    short rtn = std::strncmp(s1, s2, sizeof s1);
    return (std::abs(rtn) == 0 ? true : false);
}

void Hash::calcHash(const char* data, Hash::SHA256 digest) {
    std::memset(digest, 0, sizeof digest);
    SHA256_get((uint8_t*)data, sizeof data, digest);
    digest[SHA256_HASH_LENGTH - 1] = '\0';
}