#ifndef AUTH_UTILS
#define AUTH_UTILS

// This is implemented by the lib, but the compiller is warning against not being declared.
int ldap_simple_bind_s(LDAP *ld, const char *who, const char *cred);

/**
 * Tenta autenticar um usuário em um servidor LDAP usando o método de autenticação simples.
 *
 * @param host_url A url host do servidor LDAP.
 * @param username O nome de usuário para autenticação no servidor LDAP.
 * @param password A senha correspondente ao nome de usuário.
 *
 * @return Retorna 0 se a autenticação for bem-sucedida, caso contrário, retorna 1.
 *         Além disso, imprime mensagens de erro no console, se houver falhas.
 */
int ldap_authenticate(char* host_url, char* username, char* password);

#endif
