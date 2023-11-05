#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

// Define directives

#define LDAP_HOST "ldap://ldap.quant1.com.br:389"
#define FRPS_HOST_IP "194.163.176.185"
#define FRPS_HOST_PORT "7000"

// Define a preprocessor macro to enable or disable debugging.
// When DEBUG is defined, debugging messages will be printed.

#ifdef DEBUG
// Define a custom debug print function that behaves like printf.
#define Dprintf(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
// Define an empty macro when DEBUG is not defined, effectively disabling debugging.
#define Dprint(fmt, ...)
#endif

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

void set_default_if_empty(char* input, const char* default_value) {
    // If input is empty, set it to default_value
    if (input[0] == '\0') {
        snprintf(input, 100, "%s", default_value);
    }
}

void set_default_or_env(char* destination, const char* env_value, const char* default_value) {
    // If env_value is null or empty, copy default_value to destination. Else, copy env_value
    if (env_value == NULL || env_value[0] == "\0") {
        strcpy(destination, default_value);
    }
    else {
        strcpy(destination, env_value);
    }
}
int get_config_input(char* destination,size_t size_of_dest, int interactive, char* message) {
    if (interactive == 1) {
        printf(message);
    }
    if (fgets(destination, size_of_dest, stdin) == NULL) {
        perror("Error reading input.\n");
        return 1;
    }

    destination[strcspn(destination, "\n")] = '\0';  // remove the newline character
    return 0;
}

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

int create_configuration_toml(const char* path, const char* server_url, const char* server_port, \
        const char* proxy_name, const char* type_value, const char* ip_value, \
        const char* local_port_value, const char* custom_domain) {
    // Write the configuration file in folder.
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        perror("Error opening the file for writing.\n");
        return 1;
    }
    
    fprintf(file, "serverAddr = \"%s\"\n", server_url);
    fprintf(file, "serverPort = %s\n", server_port);
    fprintf(file, "\n[[proxies]]\n");
    fprintf(file, "name = \"%s\"\n", proxy_name);
    fprintf(file, "type = \"%s\"\n", type_value);
    fprintf(file, "localPort = %s\n", local_port_value);
    fprintf(file, "localIp = \"%s\"\n", ip_value);
    fprintf(file, "customDomains = [\"%s\"]\n", custom_domain);
    
    fclose(file);
    return 0;
}

int configure_frp_client(char* username, int interactive) {
    // Get the client file configurations 
    const char* server_url_const = getenv("SERVER_ADDR");
    const char* server_port_const = getenv("SERVER_PORT");

    char server_url[100];
    char server_port[100];
    char name_value[100];
    char type_value[100];
    char ip_value[100];
    char local_port_value[100];
    char custom_domain[100];
    char proxy_name[100];
    char client_toml[100];
    char message[200];

    int output = 0;

    strcpy(proxy_name, username);
    strcat(proxy_name, "-proxy");

    // Check if the SERVER_ADDR environment variable is empty. If it is, default it.
    set_default_or_env(server_url, server_url_const, FRPS_HOST_IP);

    set_default_or_env(server_port, server_port_const , FRPS_HOST_PORT);

    // Customize the client proxy file

    snprintf(message, sizeof(message), "Enter a name for the proxy [%s-proxy]: ", username);
    output = get_config_input(name_value, sizeof(name_value), interactive, message);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(name_value, proxy_name);

    snprintf(message, sizeof(message), "Enter the connection type [http]: ");
    output = get_config_input(type_value, sizeof(type_value), interactive, message);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(type_value, "http");

    snprintf(message, sizeof(message), "Enter the local IP [127.0.0.1]: ");
    output = get_config_input(ip_value, sizeof(ip_value), interactive, message);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(ip_value, "127.0.0.1");

    snprintf(message, sizeof(message), "Enter the local port [3000]: ");
    output = get_config_input(local_port_value, sizeof(local_port_value), interactive, message);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(local_port_value, "3000");

    // Generate a custom domain
    snprintf(custom_domain, sizeof(custom_domain), "test.frp.quant1.com.br");

    // Create the configuration file

    strcpy(client_toml, username);
    strcat(client_toml, "_client.toml");

    output = create_configuration_toml(client_toml, server_url, server_port, name_value, \
            type_value, ip_value, local_port_value, custom_domain);
    if (output == 0){
        // Print configuration
        printf("\nConfiguration completed! Your local application can be accessed \
globally via the domain:\n%s\n\n", custom_domain);
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    // This code will break if any env variable or parameter passed is longer than 
    //100 characters
    const char* username_const = getenv("QUANT1_USER");
    char username[100];
    char client_toml[100];
    char run_command[100];
    int interactive = 1;
    
    // Remove stdout need of \n to be printed.
    setvbuf(stdout, NULL, _IONBF, 0);
    
    // Check if the username was passed as an argument
    if (username_const == NULL || username_const[0] == '\0') {
        username[0] = '\0';
    }
    else {
        strcpy(username, username_const);
    }

    // If anything else was passed as an argument, disable interactive mode
    if (argc > 1) {
        interactive = 0;
    }
    // Search for a client configuration file
    char* client_toml_tmp = find_pattern_in_path("_client.toml", ".");

    // If a client configuration file was found
    if (client_toml_tmp != NULL) {

        strcpy(client_toml, client_toml_tmp);
        free(client_toml_tmp);

        if (username[0] == '\0') {
            // Extract the username from the file name
            strcpy(username, client_toml);
            remove_extension_from_string(username, "_client.toml");
        }
    }
    else {
        // If no client configuration file was found
        if (username[0] == '\0') {
            // If no username is set on env:QUANT1_USER, ask for it
            if ( interactive == 1){
                printf("Enter your quant1 username: ");
            }
            if (fgets(username, sizeof(username), stdin) == NULL) {
                perror("Error reading input.\n");
                return 1;
            }
            username[strcspn(username, "\n")] = '\0';  // Remove the newline character
        }
        // Authenticate user
        int output = autenticate_quant1_user(username, interactive);
        if (output != 0) {
            perror("Error authenticating user.");
            return 1;
        }

        // Create the configuration file
        output = configure_frp_client(username, interactive);
        if (output != 0) {
            perror("Error creating configuration file.");
            return 1;
        }
    }

    strcpy(client_toml, username);
    strcat(client_toml, "_client.toml");
    
    // Run the frpc binary
    strcpy(run_command, "frpc -c ");
    strcat(run_command, client_toml);
    printf("Running frp client: \n");
    system(run_command);

    return 0;
}
