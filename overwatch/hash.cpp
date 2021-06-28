#include <fstream>
#include <memory>
#include <filesystem>

#include "hash.hpp"
#include "sha256.h"

bool SHA256::compare_hash(const char* s1, const char* s2) noexcept {
    short rtn = std::strncmp(s1, s2, sizeof s1);
    return (SHA256::ABS(rtn) == 0 ? true : false);
}

void SHA256::calc_hash(const char* data, char* digest){
    std::memset(digest, 0, sizeof digest);
    SHA256_get((uint8_t*)data, sizeof data, digest);
    digest[SHA256::HASH_LENGTH - 1] = '\0';
}

void SHA256::calc_hash(const std::filesystem::path &path, char* digest){
    std::ifstream f;
    std::streamsize sz;
    std::unique_ptr<char[]> buf;
    memset(digest, 0, sizeof digest);

    f.open(path, std::ios::binary);
    f.seekg(0, std::ios::end);
    sz = f.tellg();
    f.seekg(0, std::ios::beg);
    buf = std::unique_ptr<char[]>(new char[sz]);
    f.read(buf.get(), sz);

    SHA256_get((uint8_t*)buf.get(), sz, digest);
    buf.reset();
}
