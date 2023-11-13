#ifndef CONFIG_H
#define CONFIG_H

// The default FRPS server IP address.
#define FRPS_HOST_IP "194.163.176.185"

// The default FRPS server port.
#define FRPS_HOST_PORT "7000"

// Variáveis para o arquivo de configuração do cliente
#define PROXY_NAME "PROXY_NAME"
#define PROXY_TYPE "PROXY_TYPE"
#define PROXY_LOCAL_IP "PROXY_LOCAL_IP"
#define PROXY_LOCAL_PORT "PROXY_LOCAL_PORT"

/**
 * @brief Create an frp client configuration file in TOML format.
 *
 * This function creates a client configuration file in TOML format with user-specific settings, 
 * including server information, proxy details, and custom domains. 
 * The resulting configuration file enables the user's local application to be accessed globally.
 *
 * @param path The path where the configuration file will be saved.
 * @param server_url The address of the frp server.
 * @param server_port The port of the frp server.
 * @param proxy_name The name of the proxy configuration.
 * @param type_value The type of connection (e.g., http).
 * @param ip_value The local IP for the proxy.
 * @param local_port_value The local port for the proxy.
 * @param custom_domain The custom domain for accessing the local application.
 *
 * @return 0 if the configuration file is successfully created; 1 in case of an error during file 
 * creation.
 *
 * @note The function saves the configuration to a file in the specified path, and the created file 
 * can be used to configure the frp client.
 */

int create_configuration_toml(const char* path, const char* server_url, const char* server_port, \
        const char* proxy_name, const char* type_value, const char* ip_value, \
        const char* local_port_value, const char* custom_domain);

/**
 * @brief Configure an frp client for a user.
 *
 * This function configures an frp client by collecting user-specific settings and creating a 
 * configuration file. It prompts the user for input when running interactively or uses 
 * default values when running non-interactively. The resulting configuration file allows 
 * the user's local application to be accessed globally via a custom domain.
 *
 * @param username The user's username for configuration.
 * @param interactive A flag indicating whether to run interactively (1 for yes, 0 for no).
 *
 * @return 0 if the client is successfully configured; 1 in case of an error during configuration.
 *
 * @note When running interactively, this function will prompt the user for various configuration 
 * options.
 */
int configure_frp_client(char* username, int interactive);

#endif
