#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>

#include "config_utils/config_utils.h"
#include "configure.h"

int create_configuration_toml(const char* path, const char* server_url, const char* server_port, \
        const char* proxy_name, const char* type_value, const char* ip_value, \
        const char* local_port_value, const char* custom_domain) {
    // Write the configuration file in folder.
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening the file for writing: %s\n", path);
        return 1;
    }
    
    fprintf(file, FRPS_HOST_IP "\"%s\"\n", server_url);
    fprintf(file, FRPS_HOST_PORT "%s\n", server_port);
    fprintf(file, "log_file = \"/var/log/frps.log\"\nlog_level = \"trace\"\nlog.maxDays = 3");
    fprintf(file, "\n[[proxies]]\n");
    // fprintf(file, PROXY_TOKEN "\"%s\"\n", frps_token);
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

    char server_url[SIMPLE_CHAR_SIZE];
    char server_port[SIMPLE_CHAR_SIZE];
    char name_value[SIMPLE_CHAR_SIZE];
    char type_value[SIMPLE_CHAR_SIZE];
    // char frps_token[SIMPLE_CHAR_SIZE];
    char ip_value[SIMPLE_CHAR_SIZE];
    char local_port_value[SIMPLE_CHAR_SIZE];
    char custom_domain[SIMPLE_CHAR_SIZE];
    char default_proxy_name[SIMPLE_CHAR_SIZE];
    char client_toml[SIMPLE_CHAR_SIZE];
    char message[MESSAGE_SIZE];
    // char *custom_domain_hash = NULL;

    int output = 0;

    strncpy(default_proxy_name, username, SIMPLE_CHAR_SIZE);
    strncat(default_proxy_name, DEFAULT_PROXY_SUFFIX, SIMPLE_CHAR_SIZE - strlen(default_proxy_name));

    // Check if the SERVER_ADDR environment variable is empty. If it is, default it.
    set_default_or_env(server_url, server_url_const, DEFAULT_FRPS_HOST_IP);

    set_default_or_env(server_port, server_port_const , DEFAULT_FRPS_HOST_PORT);

    // Customize the client proxy file

    snprintf(message, MESSAGE_SIZE, "Enter a name for the proxy [%s]: ", default_proxy_name);
    output = get_config_input(name_value, sizeof(name_value), interactive, message, PROXY_NAME_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(name_value, default_proxy_name);

    snprintf(message, MESSAGE_SIZE, "Enter the connection type [%s]: ", DEFAULT_PROXY_TYPE);
    output = get_config_input(type_value, sizeof(type_value), interactive, message, PROXY_TYPE_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(type_value, DEFAULT_PROXY_TYPE);

    snprintf(message, MESSAGE_SIZE, "Enter the local IP [%s]: ", DEFAULT_PROXY_LOCAL_IP);
    output = get_config_input(ip_value, sizeof(ip_value), interactive, message, PROXY_LOCAL_IP_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(ip_value, DEFAULT_PROXY_LOCAL_IP);

    snprintf(message, MESSAGE_SIZE, "Enter the local port [%s]: ", DEFAULT_PROXY_LOCAL_PORT);
    output = get_config_input(local_port_value, sizeof(local_port_value), interactive, message, PROXY_LOCAL_PORT_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(local_port_value, DEFAULT_PROXY_LOCAL_PORT);

    snprintf(message, MESSAGE_SIZE, "Enter the local service url [%s]: ", DEFAULT_PROXY_CUSTOM_DOMAIN);
    output = get_config_input(custom_domain, sizeof(custom_domain), interactive, message, PROXY_CUSTOM_DOMAIN_ENV);
    if (output != 0) {
        return 1;
    }
    set_default_if_empty(custom_domain, DEFAULT_PROXY_CUSTOM_DOMAIN);

    // snprintf(message, MESSAGE_SIZE, "Enter the token for frp server authentication [%s]: ", PROXY_TOKEN_DEFAULT);
    // output = get_config_input(frps_token, sizeof(frps_token), interactive, message, PROXY_TOKEN_ENV);
    // if (output != 0) {
    //     return 1;
    // }
    
    // Create the configuration file
    strncpy(client_toml, username, SIMPLE_CHAR_SIZE);
    strncat(client_toml, "_client.toml", SIMPLE_CHAR_SIZE - strlen(client_toml));

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
