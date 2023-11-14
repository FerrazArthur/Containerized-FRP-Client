#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <unistd.h>

void md5_hash(const char *data, size_t len, unsigned char *md) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *mdtype = EVP_md5(); // Use the MD5 algorithm

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, mdtype, NULL);
    EVP_DigestUpdate(mdctx, data, len);
    EVP_DigestFinal_ex(mdctx, md, NULL);
    EVP_MD_CTX_free(mdctx);
}

int main() {
    int n;
    char buf[256];
    ssize_t bytes;

    printf("insert the string: ");
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        fprintf(stderr, "Something gone wrong while reading input.\n");
        return 1;
    }

    size_t md_size = EVP_MD_size(EVP_md5());
    unsigned char *out = malloc(md_size);

    if (out == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return 1;
    }

    md5_hash(buf, strlen(buf), out);
    // Allocate space for the hexadecimal representation of the hash
    char mystr[2 * md_size + 1];

    for (size_t n = 0; n < md_size; n++) {
        // Use sprintf to convert each byte to two hexadecimal characters
        sprintf(&mystr[n * 2], "%02x", out[n]);
    }

    mystr[2 * md_size] = '\0'; // Null-terminate the string

    printf("MD5 hash: %s\n", mystr);

    free(out);

    return 0;
}
