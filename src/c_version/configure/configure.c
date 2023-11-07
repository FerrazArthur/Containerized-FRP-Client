#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "config_utils.h"
#include "configure.h"

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
