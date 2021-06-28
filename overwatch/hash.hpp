#ifndef HASH_H
#define HASH_H

#include <filesystem>

namespace SHA256 {
    constexpr unsigned char HASH_LENGTH = 64 + 1;
    using Hash = char[HASH_LENGTH];

    constexpr short ABS(short& x) { return (x < 0 ? -x : x); };

    void calc_hash(const char*, char*);
    // You have to initalized with null before use calc_hash()
    void calc_hash(const std::filesystem::path&, char*);
    bool compare_hash(const char*, const char*) noexcept;

    /*
    void get_hash(const FS::path &dir, char digest[cosnt_hash_length]);
    void get_hash(const char* data, char digest[cosnt_hash_length]);
    */

}

#endif