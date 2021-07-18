#include "../hash/hash.hpp"

#include <string>

bool Hash::SHA256::compareHash(const char* s1, const char* s2) const noexcept {
    return (std::abs(std::strncmp(s1, s2, SHA256_HASH_LENGTH)) == 0 ? true : false);
}

void Hash::SHA256::calcHash(const char* data, char* digest) {
    std::memset(digest, 0, SHA256_HASH_LENGTH);
    SHA256_get((uint8_t*)data, sizeof data, digest);
    digest[SHA256_HASH_LENGTH - 1] = '\0';
}