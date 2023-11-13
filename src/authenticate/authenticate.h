#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

// LDAPHOST is the address of the LDAP server used for authentication (Quant1).
// Update this value to match the LDAP server's address.
#define LDAP_HOST "ldap://ldap.quant1.com.br:389"

/**
 * @brief Authenticate a user against the Quant1 LDAP server.
 *
 * This function uses the `ldapwhoami` command to authenticate a user against the Quant1 LDAP server.
 * If the server recognizes the user and password, the user is considered authenticated.
 *
 * @param username The username of the user to authenticate.
 * @param interactive Set to 1 to enable interactive input for LDAP password; set to 0 for non-interactive mode.
 * @param password_stored The password of the user to authenticate. This parameter is only used in non-interactive mode.
 *
 * @return 0 upon successful authentication; 1 in case of authentication failure.
 */
int authenticate_quant1_user(char* username, int interactive, char password_stored[256]);
#endif
