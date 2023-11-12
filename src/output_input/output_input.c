#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "output_input.h"

// Read credentials from file and store them in a structure
void read_store_credentials(struct Credentials *credentials, const char* path) {
    char line[256];
    char *token = NULL;
    FILE *cred_file = fopen(path, "r");
    if (cred_file == NULL) {
        fprintf(stderr, "Error opening %s\n", path);
        exit(EXIT_FAILURE);
    }

    if (fgets(line, sizeof(line), cred_file) != NULL) {
        token = strtok(line, " ");
        if (token != NULL) {
            strncpy(credentials->user, token, sizeof(credentials->user));
        }

        token = strtok(NULL, " ");
        if (token != NULL) {
            strncpy(credentials->password, token, sizeof(credentials->password));
        }
        else {
            fprintf(stderr, "Error reading %s: password not properly defined.\n", path);
            fclose(cred_file);
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Error reading %s: file is empty.\n", path);
        fclose(cred_file);
        exit(EXIT_FAILURE);
    }

    fclose(cred_file);
}

void output_config_input(const struct Credentials *credentials, FILE *output_stream) {
    // Output the configuration input
    const char *proxy_name = getenv("PROXY_NAME");
    const char *proxy_type = getenv("PROXY_TYPE");
    const char *proxy_local_ip = getenv("PROXY_LOCAL_IP");
    const char *proxy_local_port = getenv("PROXY_LOCAL_PORT");

    // put out the password
    fprintf(output_stream, "%s", credentials->password);

    if (proxy_name != NULL) {
        fprintf(output_stream, "%s", proxy_name);
    }
    else {
        fprintf(output_stream, "\n");
    }
    if (proxy_type != NULL) {
        fprintf(output_stream, "%s", proxy_type);
    }
    else {
        fprintf(output_stream, "\n");
    }
    if (proxy_local_ip != NULL) {
        fprintf(output_stream, "%s", proxy_local_ip);
    }
    else {
        fprintf(output_stream, "\n");
    }
    if (proxy_local_port != NULL) {
        fprintf(output_stream, "%s", proxy_local_port);
    }
    else {
        fprintf(output_stream, "\n");
    }
}

void output_main_input(FILE *output_stream, const char *config_file_suffix, const char *credentials_path) {
    // This function reproduce the input prompt from the main function and
    // fill the output_stream with the requested input, in the same order
    // Read credentials once and store them in a structure
    struct Credentials credentials;
    DIR *dir;
    struct dirent *entry;
    int found = 0;

    if (output_stream == NULL) {
        fprintf(stderr, "Error in output stream\n");
        exit(EXIT_FAILURE);
    }

    read_store_credentials(&credentials, credentials_path);

    // Search for a config file
    dir = opendir(".");
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory\n");
        exit(EXIT_FAILURE);
    }

    // Search for the config file with the specified config_file_suffix
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, config_file_suffix) != NULL) {
            // Config file with the specified config_file_suffix found
            found = 1;
            break;
        }
    }

    if (found == 0) {
        // Config file doesn't exist
        // It's the first execution, and we configure the client

        // If QUANT1_USER is not already set as an environment variable
        if (getenv("QUANT1_USER") == NULL) {
            fprintf(output_stream, "%s\n", credentials.user);
        }
    }

    output_config_input(&credentials, output_stream);
}