#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "main.h"
#include "main_utils/main_utils.h"
#include "configure/configure.h"
#include "autenticate/autenticate.h"

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
    if (argc > 1 && argv[0][0] != '\0') {
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
