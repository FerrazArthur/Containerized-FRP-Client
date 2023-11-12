#ifndef OUTPUT_INPUT_H
#define OUTPUT_INPUT_H

#include <stdio.h>  // Include the standard input/output header

/**
 * @struct Credentials
 * @brief Structure to hold credential information.
 */
struct Credentials {
    char user[256];       /**< User credential. */
    char password[256];   /**< Password credential. */
};

/**
 * @brief Read credentials from a file and store them in a structure.
 *
 * This function opens a file specified by the given path, reads the first line,
 * and extracts user and password credentials, storing them in the provided
 * Credentials structure.
 *
 * @param credentials Pointer to a Credentials structure to store the extracted credentials.
 * @param path Path to the file containing credentials.
 *
 * @note Exits the program with EXIT_FAILURE if an error occurs during file operations.
 */
void read_store_credentials(struct Credentials *credentials, const char* path);

/**
 * @brief Output the configuration input to a specified stream.
 *
 * This function outputs the configuration input, including password and optional
 * environment variables, to the specified output stream.
 *
 * @param credentials Pointer to a Credentials structure containing user and password information.
 * @param output_stream File stream to which the configuration input is written.
 */
void output_config_input(const struct Credentials *credentials, FILE *output_stream);

/**
 * @brief Reproduce the input prompt from the main function and fill the output stream.
 *
 * This function reproduces the input prompt from the main function and fills the
 * specified output stream with the requested input, in the same order. It searches
 * for a configuration file in the current directory and, if not found, configures
 * the client by outputting user credentials.
 *
 * @param output_stream File stream to which the input prompt is written.
 *
 * @note Exits the program with EXIT_FAILURE if an error occurs during file operations.
 */
void output_main_input(FILE *output_stream, const char* config_file_suffix, const char* credentials_path);

#endif // OUTPUT_INPUT_H
