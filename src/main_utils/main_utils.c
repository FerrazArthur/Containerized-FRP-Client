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
        perror("Failed to open pathectory.");
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
