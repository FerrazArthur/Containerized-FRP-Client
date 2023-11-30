#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>

#include "config_utils/config_utils.h"
#include "configure.h"

int create_configuration_toml(const char* path, const char* server_url, const char* server_port, \
        const char* proxy_name, const char* type_value, const char* ip_value, \
        const char* local_port_value, const char* frps_token,  const char* custom_domain) {
    // Write the configuration file in folder.
    FILE* file = fopen(path, "w");

    if (file == NULL) {
        fprintf(stderr, "Error opening the file for writing: %s\n", path);
        return 1;
    }
    
    fprintf(file, FRPS_HOST_IP "\"%s\"\n", server_url);
    fprintf(file, FRPS_HOST_PORT "%s\n", server_port);
    fprintf(file, "\n[[proxies]]\n");
    fprintf(file, PROXY_TOKEN "\"%s\"\n", frps_token);
    fprintf(file, PROXY_NAME "\"%s\"\n", proxy_name);
    fprintf(file, PROXY_TYPE "\"%s\"\n", type_value);
    fprintf(file, PROXY_LOCAL_PORT "%s\n", local_port_value);
    fprintf(file, PROXY_LOCAL_IP "\"%s\"\n", ip_value);
    fprintf(file, PROXY_CUSTOM_DOMAIN "[\"%s\"]\n", custom_domain);
    
    fclose(file);
    return 0;
}

int configure_frp_client(char* username, int interactive) {
    // Get the client file configurations
    const char* server_url_const = getenv(FRPS_HOST_IP_ENV);
    const char* server_port_const = getenv(FRPS_HOST_PORT_ENV);

    char server_url[256];
    char server_port[256];
    char name_value[256];
    char type_value[256];
    char frps_token[256];
    char ip_value[256];
    char local_port_value[256];
    char custom_domain[256];
    char default_proxy_name[256];
    char client_toml[256];
    char message[356];
    // char *custom_domain_hash = NULL;

    int output = 0;

    strcpy(default_proxy_name, username);
    strcat(default_proxy_name, DEFAULT_PROXY_SUFFIX);

    // Check if the SERVER_ADDR environment variable is empty. If it is, default it.
    set_default_or_env(server_url, server_url_const, DEFAULT_FRPS_HOST_IP);

    set_default_or_env(server_port, server_port_const , DEFAULT_FRPS_HOST_PORT);

    // Customize the client proxy file

    snprintf(message, sizeof(message), "Enter a name for the proxy [%s]: ", default_proxy_name);
    output = get_config_input(name_value, sizeof(name_value), interactive, message, PROXY_NAME_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(name_value, default_proxy_name);

    snprintf(message, sizeof(message), "Enter the connection type [%s]: ", DEFAULT_PROXY_TYPE);
    output = get_config_input(type_value, sizeof(type_value), interactive, message, PROXY_TYPE_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(type_value, DEFAULT_PROXY_TYPE);

    snprintf(message, sizeof(message), "Enter the local IP [%s]: ", DEFAULT_PROXY_LOCAL_IP);
    output = get_config_input(ip_value, sizeof(ip_value), interactive, message, PROXY_LOCAL_IP_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(ip_value, DEFAULT_PROXY_LOCAL_IP);

    snprintf(message, sizeof(message), "Enter the local port [%s]: ", DEFAULT_PROXY_LOCAL_PORT);
    output = get_config_input(local_port_value, sizeof(local_port_value), interactive, message, PROXY_LOCAL_PORT_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(local_port_value, DEFAULT_PROXY_LOCAL_PORT);

    snprintf(message, sizeof(message), "Enter the token for frp server authentication [%s]: ", PROXY_TOKEN_DEFAULT);
    output = get_config_input(frps_token, sizeof(frps_token), interactive, message, PROXY_TOKEN_ENV);
    if (output != 0) {
        return 1;
    }

    // // Generate a user hash for the custom domain
    // output = md5_hash(username, &custom_domain_hash);

    // if (output != 0) {
    //     fprintf(stderr, "Error while generating the custom domain hash.\n");
    //     return 1;
    // }

    // // Append the custom domain suffix to the username
    snprintf(custom_domain, sizeof(custom_domain), "%s" PROXY_CUSTOM_DOMAIN_SUFFIX, username);
    // free(custom_domain_hash);
    
    // Create the configuration file
    strcpy(client_toml, username);
    strcat(client_toml, "_client.toml");

    output = create_configuration_toml(client_toml, server_url, server_port, name_value, \
            type_value, ip_value, local_port_value, frps_token, custom_domain);
    if (output == 0){
        // Print configuration
        printf("\nConfiguration completed! Your local application can be accessed \
globally via the domain:\n%s\n\n", custom_domain);
        return 0;
    }
    return 1;
}
