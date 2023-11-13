#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <ldap.h>
#include <unistd.h>

#include "authenticate.h"
#include "auth_utils/auth_utils.h"

int authenticate_quant1_user(char* username, int interactive, char password_stored[256]) {
    // Authenticate user by calling ldapwhoami api.
    // If quant1 ldap server recognize user and password, then user is autenticated.
    int output = 0;
    char ldap_username[256];
    char *password = NULL;

    printf("Authenticating user %s...\n", username);

    strncpy(ldap_username, "cn=", sizeof(ldap_username));
    strncat(ldap_username, username, sizeof(ldap_username) - strlen(ldap_username) - 1);
    strncat(ldap_username, ",ou=users,dc=ldap,dc=quant1,dc=com,dc=br", \
sizeof(ldap_username) - strlen(ldap_username) - 1);

    if (interactive == 1) {
        printf("Insira a senha de acesso: ");
        // Read password
        password = getpass("");
    }
    else if (interactive == 0) {
        password = password_stored;
    }
    if (password == NULL) {
        printf("Error reading password.\n");
        return 1;
    }
    password[strcspn(password, "\n")] = '\0';  // remove the newline character

    output = ldap_authenticate(LDAP_HOST, ldap_username, password);

    // Clear the password from memory
    memset(password, 0, strlen(password));

    if (output != 0) {
        return 1;
    }
    else {
        printf("User authenticated successfully!\n");
    }
    return 0;
}
