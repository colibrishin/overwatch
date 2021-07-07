#include "../hash/sha256.h"

void SHA256_init(SHA256ctx *const ctx){
    memset(ctx, 0, sizeof(*ctx));
    ctx->h[0] = 0x6a09e667UL;
    ctx->h[1] = 0xbb67ae85UL;
    ctx->h[2] = 0x3c6ef372UL;
    ctx->h[3] = 0xa54ff53aUL;
    ctx->h[4] = 0x510e527fUL;
    ctx->h[5] = 0x9b05688cUL;
    ctx->h[6] = 0x1f83d9abUL;
    ctx->h[7] = 0x5be0cd19UL;
}

void SHA256_transform(SHA256ctx *const ctx){
    uint8_t t;
    uint32_t tmp1, tmp2, *w = ctx->w;

    uint32_t a = ctx->h[0];
    uint32_t b = ctx->h[1];
    uint32_t c = ctx->h[2];
    uint32_t d = ctx->h[3];
    uint32_t e = ctx->h[4];
    uint32_t f = ctx->h[5];
    uint32_t g = ctx->h[6];
    uint32_t h = ctx->h[7];

    for(t = 0; t < 64; ++t){
        if(t >= 16)
            W(t) += sig0(W(t + 14)) + W(t + 9) + sig1(W(t + 1));
        tmp1 = h + SIG0(e) + CH(e,f, g) + k[t] + W(t);
        tmp2 = SIG1(a) + MAJ(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + tmp1;
        d = c;
        c = b;
        b = a;
        a = tmp1 + tmp2;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;
}

void SHA256_update(SHA256ctx *const ctx, const uint8_t *data, size_t sz) {
    size_t n;

    while(sz > 0) {
        n = MIN(sz, 64 - ctx->sz);
        memcpy(ctx->chunks + ctx->sz, data, n);

        ctx->sz += n;
        ctx->totalsz += n;

        data = (uint8_t*) data + n;
        sz -= n;

        if(ctx->sz == 64) {
            SHA256_transform(ctx);
            ctx->sz = 0;
        }
    }
}

void SHA256_finial(SHA256ctx *const ctx, char *const digest) {
    size_t paddingsz;
    uint64_t totalsz = ctx->totalsz * 8;

    paddingsz = 56 - ctx->sz;
    if(ctx->sz >= 56) paddingsz += 64;

    SHA256_update(ctx, padding, paddingsz);
    
    ctx->w[14] = LLS(totalsz, 32);
    ctx->w[15] = totalsz;

    SHA256_transform(ctx);

    // memcpy(digest, ctx->digest, SHA256_DIGEST_SIZE);
    
    // Big-Endian Copy, unsigned char to char;
    for(int i = 0, j = 0; i < SHA256_DIGEST_SIZE; ++i, ++j){
        digest[j] = hexTable[ctx->digest[i] >> 4];
        digest[++j] = hexTable[ctx->digest[i] & 0x0f];
    }
}

void SHA256_get(const uint8_t *data, size_t sz, char *digest) {
    SHA256ctx ctx;

    SHA256_init(&ctx);
    SHA256_update(&ctx, data, sz);
    SHA256_finial(&ctx, digest);
}