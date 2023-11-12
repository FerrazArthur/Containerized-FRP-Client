#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>

#include "main.h"
#include "main_utils/main_utils.h"
#include "configure/configure.h"
#include "authenticate/authenticate.h"
#include "output_input/output_input.h"

int main(int argc, char* argv[]) {
    // This code will break if any env variable or parameter passed is longer than 
    // 256 characters
    const char* username_const = getenv("QUANT1_USER");
    char* client_toml_tmp = NULL;
    char username[256];
    char client_toml[256];
    char run_command[256];
    int interactive = 0;
    int output = 0;
    char *input_buffer = NULL;

    // If the program is running in interactive mode, use stdin as input
    FILE *input_stream = stdin;
    
    // change stdout to not need \n to print.
    setvbuf(stdout, NULL, _IONBF, 0);
    
    // Check if the username was passed as an argument
    if (username_const == NULL || username_const[0] == '\0') {
        username[0] = '\0';
    }
    else {
        strcpy(username, username_const);
    }

    // Process any arguments passed to this function
    output = read_args(argc, argv, &interactive);
    if (output == 1) {
        return 1;
    }
    else if (output == 2) {
        return 0;
    }

    if (interactive == 0) {
        // If the program is not running in interactive mode, set input_stream to buffer 
        // and fill it
        input_buffer = malloc(BUFFER_SIZE);
        if (input_buffer == NULL) {
            fprintf(stderr, "Error allocating memory for input buffer.\n");
            return 1;
        }
        input_stream = fmemopen(input_buffer, BUFFER_SIZE, "r+");
        if (input_stream == NULL) {
            fprintf(stderr, "Error creating input stream.\n");
            free(input_buffer);
            return 1;
        }
        // Fill the input buffer with the output of output_main_input
        output_main_input(input_stream, CONFIG_FILE_SUFFIX, CREDENTIALS_PATH);
        rewind(input_stream);
    }

    // Search for a client configuration file
    client_toml_tmp = find_pattern_in_path("_client.toml", ".");

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
            if (fgets(username, sizeof(username), input_stream) == NULL) {
                fprintf(stderr, "Error reading input.\n");
                fclose(input_stream);
                free(input_buffer);
                return 1;
            }
            username[strcspn(username, "\n")] = '\0';  // Remove the newline character
        }
        // Authenticate user
        output = authenticate_quant1_user(username, interactive, input_stream);
        if (output != 0) {
            fprintf(stderr, "Error authenticating user. Please check your credentials\
 and try again.\n");
            fclose(input_stream);
            free(input_buffer);
            return 1;
        }

        // Create the configuration file
        output = configure_frp_client(username, interactive, input_stream);
        if (output != 0) {
            fprintf(stderr, "Error creating configuration file.\n");
            fclose(input_stream);
            free(input_buffer);
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
