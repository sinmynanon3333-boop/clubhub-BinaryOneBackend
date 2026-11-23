#pragma once
#include <sodium.h>
#include <string>

std::string generateToken() {
    unsigned char bytes[32];

    randombytes_buf(bytes, sizeof(bytes));

    char encoded[crypto_generichash_BYTES * 2 + 1];

    sodium_bin2hex(
        encoded,
        sizeof(encoded),
        bytes,
        sizeof(bytes)
    );

    return std::string(encoded);
}