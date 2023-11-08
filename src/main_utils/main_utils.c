#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "main_utils.h"

char* find_pattern_in_path(const char* pattern, const char* path) {
    // Search path for files that has substr pattern and return first occurrence, return NULL if not find.
    // Doesnt search recursivelly.
    struct dirent *entry = NULL;
    struct stat file_stat;
    char* client_toml = NULL;
    DIR *dp = opendir(path);
    
    if (dp == NULL) {
        perror("Failed to open directory.\n");
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

    perror("Error: Unknown argument.\n");
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
            perror("Error: Unknown argument format.\n");
            code_return = 1;
        }

        if (code_return == 1 || code_return == 2) {
            return code_return;
        }
    }
    return code_return;
}
