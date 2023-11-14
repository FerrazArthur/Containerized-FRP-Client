#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

#include "config_utils.h"

int md5_hash(const char* input_str, char** output_str) {
    // Allocate memory to store the output hash
    size_t md_size = EVP_MD_size(EVP_md5());
    unsigned char *out = malloc(md_size);
    *output_str = malloc(md_size * 2 + 1);
    
    if (out == NULL || *output_str == NULL) {
        fprintf(stderr, "Memory allocation for md5 hash error.\n");
        return 1;
    }

    // Hash the input string
    EVP_MD_CTX *mdctx;
    const EVP_MD *md_type = EVP_md5(); // md5 hash type
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md_type, NULL);
    EVP_DigestUpdate(mdctx, input_str, strlen(input_str));
    EVP_DigestFinal_ex(mdctx, out, NULL);
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a string

    for (size_t i = 0; i < md_size; i++) {
        sprintf(*output_str + i * 2, "%02x", out[i]);
    }
    free(out);
    
    return 0;
}

void set_default_if_empty(char* input, const char* default_value) {
    // If input is empty, set it to default_value
    if (input[0] == '\0') {
        snprintf(input, 100, "%s", default_value);
    }
}

void set_default_or_env(char* destination, const char* env_value, const char* default_value) {
    // If env_value is null or empty, copy default_value to destination. Else, copy env_value
    if (env_value == NULL || env_value[0] == '\0') {
        strcpy(destination, default_value);
    }
    else {
        strcpy(destination, env_value);
    }
}
int get_config_input(char* destination, size_t size_of_dest, int interactive, char* message\
, const char* env_variable_name) {
    if (interactive == 1) {
        printf("%s", message);
        if (fgets(destination, size_of_dest, stdin) == NULL) {
            fprintf(stderr, "Error reading input.\n");
            return 1;
        }
    }
    else if (interactive == 0) {
        char* env_value = getenv(env_variable_name);
        if (env_value != NULL) {
            strncpy(destination, env_value, size_of_dest);
        }
        else {
            strncpy(destination, "", size_of_dest);
        }
    }

    destination[strcspn(destination, "\n")] = '\0';  // remove the newline character
    return 0;
}
