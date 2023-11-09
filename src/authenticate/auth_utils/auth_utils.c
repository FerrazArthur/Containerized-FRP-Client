#include <stdlib.h>
#include <stdio.h>
#include <ldap.h>

#include "auth_utils.h"

int ldap_authenticate(char* host_url, char* username, char* password) {
    LDAP *ld = NULL;
    int rc;
    int result;
    int protocol = 3;

    // Initialize an LDAP session
    rc = ldap_initialize(&ld, host_url);
    if ( rc != LDAP_SUCCESS) {
        fprintf(stderr, "ldap_initialize failed\n");
        return 1;
    }
    // Set LDAP protocol version to 3
    rc = ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &protocol);
    if (rc != LDAP_OPT_SUCCESS) {
        fprintf(stderr, "Setting LDAP protocol version failed: %s\n", ldap_err2string(rc));
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 1;
    }

    rc = ldap_connect( ld );
    if( rc != LDAP_SUCCESS ) {
        fprintf( stderr, "Could not connect to URL");
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 1;
    }
    // Try a simple bind with user and password provided
    result = ldap_simple_bind_s(ld, username, password);
    if (result != LDAP_SUCCESS) {
        fprintf(stderr, "Authentication error: %s\n", ldap_err2string(result));
        ldap_unbind_ext_s(ld, NULL, NULL);
        return 1;
    }
    
    // Disconect from server
    ldap_unbind_ext_s(ld, NULL, NULL);

    return 0;
}
