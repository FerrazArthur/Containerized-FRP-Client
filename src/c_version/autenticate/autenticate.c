#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "autenticate.h"

int autenticate_quant1_user(char* username, int interactive) {
    // Authenticate user by calling ldapwhoami api.
    // If quant1 ldap server recognize user and password, then user is autenticated.
    // Interactive calls the ldapwhoami input prompt, while non interactive doest prompt user.
    int output = 0;
    char ldap_command[356];
    if (interactive == 1) {
        strcpy(ldap_command, "ldapwhoami -x -W -H " LDAP_HOST " -D \"cn=");
        strcat(ldap_command, username);
        strcat(ldap_command, ",ou=users,dc=ldap,dc=quant1,dc=com,dc=br\" > /dev/null");
    }
    else {
        char password[200];

        if (fgets(password, sizeof(password), stdin) == NULL) {
            printf("Error reading password.\n");
            return 1;
        }
        password[strcspn(password, "\n")] = '\0';  // remove the newline character

        strcpy(ldap_command, "ldapwhoami -x -H " LDAP_HOST " -D \"cn=");
        strcat(ldap_command, username);
        strcat(ldap_command, ",ou=users,dc=ldap,dc=quant1,dc=com,dc=br\" -w \"");
        strcat(ldap_command, password);
        strcat(ldap_command, "\" > /dev/null");
        // Clear the password from memory
        memset(password, 0, sizeof(password));

    }

    // Do a system call with the command.
    output = system(ldap_command);

    if (output != 0) {
        perror("Error: User was not properly authenticated.\n");
        return 1;
    }
    else {
        printf("User authenticated successfully!\n");
    }
    return 0;
}
