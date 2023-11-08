#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "config_utils.h"

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
int get_config_input(char* destination,size_t size_of_dest, int interactive, char* message) {
    if (interactive == 1) {
        printf("%s", message);
    }
    if (fgets(destination, size_of_dest, stdin) == NULL) {
        perror("Error reading input.\n");
        return 1;
    }

    destination[strcspn(destination, "\n")] = '\0';  // remove the newline character
    return 0;
}
