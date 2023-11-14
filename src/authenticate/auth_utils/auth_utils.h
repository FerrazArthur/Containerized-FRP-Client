#ifndef AUTH_UTILS
#define AUTH_UTILS

// This is implemented by the lib, but the compiller is warning against not being declared.
int ldap_simple_bind_s(LDAP *ld, const char *who, const char *cred);

/**
 * Attempts to authenticate a user on an LDAP server using the simple authentication method.
 *
 * @param host_url The LDAP server's host URL.
 * @param username The username for authentication on the LDAP server.
 * @param password The password corresponding to the username.
 *
 * @return Returns 0 if authentication is successful, otherwise returns 1.
 *         Additionally, prints error messages to the console in case of failures.
 */
int ldap_authenticate(char* host_url, char* username, char* password);

#endif
