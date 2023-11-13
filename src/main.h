#ifndef MAIN_H
#define MAIN_H

#define CONFIG_FILE_SUFFIX "_client.toml"
#ifndef CREDENTIALS_PATH

#define CREDENTIALS_PATH "/run/secrets/QUANT1_CREDENTIALS"
//#define CREDENTIALS_PATH "./.credentials"

#endif

#ifndef SERVER_CONFIG_PATH

#define SERVER_CONFIG_PATH "./server_config"

#endif

#ifndef FRPC_EXECUTABLE_NAME

#define FRPC_EXECUTABLE_NAME "frpc"

#endif

/**
 * @brief Main function of the program that configures and runs the FRP client.
 *
 * This function orchestrates the configuration and execution of the FRP client.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 *
 * @return 0 upon successful execution; 1 in case of errors.
 */
int main(int argc, char* argv[]);

#endif
