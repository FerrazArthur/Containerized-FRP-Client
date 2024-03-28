#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include "main.h"
#include "main_utils/main_utils.h"
#include "configure/configure.h"
#include "authenticate/authenticate.h"

int main(int argc, char* argv[]) {
    // This code will break if any env variable or parameter passed is longer than 
    // SIMPLE_CHAR_SIZE characters
    char* client_toml_tmp = NULL;
    char client_toml[SIMPLE_CHAR_SIZE] = "";
    const char *run_command_argv[] = {FRPC_EXECUTABLE_NAME, "-c", client_toml, NULL};
    const char *run_command = FRPC_EXECUTABLE_NAME;
    int interactive = 0;
    int output = 0;
    struct Credentials credentials;

    // change stdout to not need \n to print.
    setvbuf(stdout, NULL, _IONBF, 0);

    // // Process any arguments passed to this function
    output = read_args(argc, argv, &interactive);
    if (output == 1) {
        return 1;
    }
    else if (output == 2) {
        return 0;
    }

    output = read_server_configuration(SERVER_CONFIG_PATH, FRPS_HOST_IP_ENV, \
            FRPS_HOST_PORT_ENV);

    if (output != 0) {
        exit(EXIT_FAILURE);
    }

    // Search for a client configuration file
    client_toml_tmp = find_pattern_in_path(CONFIG_FILE_SUFFIX, ".");

    // If a client configuration file was found
    if (client_toml_tmp != NULL) {

        strncpy(client_toml, client_toml_tmp, SIMPLE_CHAR_SIZE);
        free(client_toml_tmp);

        // Extract the username from the file name
        strncpy(credentials.user, client_toml, SIMPLE_CHAR_SIZE);
        remove_extension_from_string(credentials.user, CONFIG_FILE_SUFFIX);
    }
    else {
        // If no client configuration file was found, create one
        if (interactive == 1){
            printf("Enter your username: ");

            if (fgets(credentials.user, sizeof(credentials.user), stdin) == NULL) {
                fprintf(stderr, "Error reading input.\n");
                return 1;
            }
        }
        else if (interactive == 0) {
            read_store_credentials(&credentials, CREDENTIALS_PATH);
        }
        credentials.user[strcspn(credentials.user, "\n")] = '\0';  // Remove the newline character

        // Authenticate user
        output = authenticate_LDAP_user(credentials.user, interactive, credentials.password);
        if (output != 0) {
            fprintf(stderr, "Error authenticating user. Please check your credentials\
 and try again.\n");
            return 1;
        }

        // Create the configuration file
        output = configure_frp_client(credentials.user, interactive);
        if (output != 0) {
            fprintf(stderr, "Error creating configuration file.\n");
            return 1;
        }
    }

    strncpy(client_toml, credentials.user, SIMPLE_CHAR_SIZE);
    strncat(client_toml, CONFIG_FILE_SUFFIX, SIMPLE_CHAR_SIZE - strlen(client_toml));

    // Run the frpc binary
    printf("Running frp client: \n");

    if (execvp(run_command, (char* const*)run_command_argv) != 0) {
        fprintf(stderr, "Error running frp client.\n");
        return 1;
    }

    return 0;
}
