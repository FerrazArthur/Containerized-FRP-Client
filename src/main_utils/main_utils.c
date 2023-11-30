#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "main_utils.h"


int read_server_configuration(const char* path, const char* server_addr_env, const char* server_port_env) {
    char line[256];
    char *token = NULL;
    FILE *server_file = fopen(path, "r");
    if (server_file == NULL) {
        // fprintf(stderr, "Error opening %s: File not found. Configuration will \
proceed with default value for server's ip and port\n", path);
        fprintf(stderr, "Error opening %s\n", path);
        return 1;
    }

    if (fgets(line, sizeof(line), server_file) != NULL) {
        token = strtok(line, ":");
        if (token != NULL) {
            setenv(server_addr_env, token, 1);
        }

        token = strtok(NULL, ":");
        if (token != NULL) {
            setenv(server_port_env, token, 1);
        }
        else {
            fprintf(stderr, "Error reading %s: server host not properly defined.\n", path);
            fclose(server_file);
            return 1;
        }
    }
    else {
        fprintf(stderr, "Error reading %s: file is empty.\n", path);
        fclose(server_file);
        return 1;
        // exit(EXIT_FAILURE);
    }
    fclose(server_file);
    return 0;
}

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

char* find_pattern_in_path(const char* pattern, const char* path) {
    // Search path for files that has substr pattern and return first occurrence, return 
    // NULL if not find.
    // Doesnt search recursivelly.
    struct dirent *entry = NULL;
    struct stat file_stat;
    char* client_toml = NULL;
    DIR *dp = opendir(path);
    
    if (dp == NULL) {
        fprintf(stderr, "Failed to open directory: Path: %s\n", path);
        return NULL;
    }

    while ((entry = readdir(dp))) {
        // Check if the entry is a regular file
        if (stat(entry->d_name, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            // Check if the file name matches the pattern "*_client.toml"
            if (strstr(entry->d_name, pattern) != NULL) {
                client_toml = malloc(strlen(entry->d_name) + 1);
                strcpy(client_toml, entry->d_name);

                closedir(dp);
                return client_toml;
            }
        }
    }
    closedir(dp);
    return NULL;
}

void remove_extension_from_string(char* name, char* extension) {
    // Search name for extension substring. If present, exclude it by placing a '\0'
    // in its place.
    char* found = strstr(name, extension);
    if (found) {
        *found = '\0';
    }
}

int process_arg(char* arg, int *interactive) {
    char *message = "Uso: quant1_frpc [OPÇÕES]\n"
    "Autentica o usuário, configura um cliente de proxy reverso rápido e o inicia.\n\n"
    "Opções:\n"
    "  --help, -h          Exibe esta mensagem de ajuda\n"
    "  --interactive, -i   Executa interativamente, solicitando informações \
conforme configura o arquivo e realiza a autenticação\n";
    if (strcmp(arg, "help") == 0 || strcmp(arg, "h") == 0) {
        printf("%s", message);
        return 2;
    }
    else if (strcmp(arg, "interactive") == 0 || strcmp(arg, "i") == 0) {
        *interactive = 1;
        return 0;
    }

    fprintf(stderr, "Error: Unknown argument: %s\n", arg);
    return 1;
}

int read_args(int argc, char* argv[], int* interactive) {
    // Receive the args passed to the program and return codes.
    int code_return = 0;
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--", 2) == 0) {
            code_return = process_arg(argv[i]+2, interactive);
        }
        else if (strncmp(argv[i], "-", 1) == 0) {
            code_return = process_arg(argv[i]+1, interactive);
        }
        else {
            fprintf(stderr, "Error: Unknown argument format: %s\n", argv[i]);
            code_return = 1;
        }

        if (code_return == 1 || code_return == 2) {
            return code_return;
        }
    }
    return code_return;
}
