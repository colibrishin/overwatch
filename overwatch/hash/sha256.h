#ifndef SHA256_H
#define SHA256_H

#ifndef _cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define SHA256_BLOCK_LENGTH 8
#define SHA256_DIGEST_SIZE 32

typedef struct {
    // 256 bits
    union {
        uint32_t h[SHA256_BLOCK_LENGTH];
        uint8_t digest[SHA256_DIGEST_SIZE];
    };
    // 512 bits
    union {
        uint32_t w[SHA256_BLOCK_LENGTH * 2];
        uint8_t chunks[SHA256_DIGEST_SIZE * 2];
    };
    size_t sz;
    uint64_t totalsz;
} SHA256ctx;

#define XOR2(x,y) (x ^ y)
#define XOR3(x,y,z) (x ^ y ^ z)
#define NOT(x) ~x
#define AND(x,y) (x & y)

#define LLS(x,y) (x << y)
#define RLS(x,y) (x >> y)
#define LCS(x,y) (LLS(x, y) | RLS(x, (32 - y)))
#define RCS(x,y) (RLS(x, y) | LLS(x, (32 - y)))

#define CH(x,y,z) ( XOR2(AND(x, y), AND(NOT(x), z)) )
#define MAJ(x,y,z) ( XOR3(AND(x,y), AND(x,z), AND(y,z)) )
#define SIG0(x) ( XOR3(RCS(x, 2), RCS(x, 13), RCS(x, 22)) )
#define SIG1(x) ( XOR3(RCS(x, 6), RCS(x, 11), RCS(x,25)) )
#define sig0(x) ( XOR3(RCS(x, 7), RCS(x, 18), RLS(x, 3)) )
#define sig1(x) ( XOR3(RCS(x, 17), RCS(x, 19), RLS(x, 10)) )

#define MIN(x,y) (x < y ? x : y)
#define W(x) w[(x) & 0x0f] // Cycling, AND -> Masking.
#define BYTEN2WORDS(a,b,c,d) (LLS(a, 24) | LLS(b, 16) | LLS(c, 8) | d)

static const char hexTable[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static const uint8_t padding[64] = {
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// Initialize array of round constants:
// (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):
static const uint32_t k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void SHA256_init(SHA256ctx *const ctx);
void SHA256_transform(SHA256ctx *const ctx);
void SHA256_update(SHA256ctx *const ctx, const uint8_t *data, size_t sz);
void SHA256_finial(SHA256ctx *const ctx, char *const digest);
void SHA256_get(const uint8_t *data, size_t sz, char *digest);

#ifndef _cplusplus
}
#endif

#endif