#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

/**
 * @brief Search for a file with a specific pattern in the given directory path.
 *
 * This function searches for files in the specified directory path that match the provided pattern.
 * It returns the name of the first file found, or NULL if no matching file is found.
 *
 * @param pattern The pattern to match in file names.
 * @param path The directory path to search for files.
 *
 * @return A dynamically allocated string containing the name of the first matching file, or NULL if not found.
 *
 * @note The returned string is dynamically allocated and should be freed when no longer needed to 
 * prevent memory leaks.
 */
char* find_pattern_in_path(const char* pattern, const char* path);

/**
 * @brief Remove a specified file extension from a string.
 *
 * This function searches the input string for the specified extension and removes it by placing a 
 * null terminator ('\0') in its place.
 *
 * @param name The input string from which to remove the extension.
 * @param extension The extension to be removed from the string.
 *
 * @note This function modifies the input string directly by replacing the extension with a null character ('\0').
 */
void remove_extension_from_string(char* name, char* extension);

/**
 * @brief Set a default value for a string if it is empty (contains a null character).
 *
 * This function checks if the input string is empty (contains a null character) and, if so, sets it 
 * to the provided default value.
 *
 * @param input The input string to check and potentially update.
 * @param default_value The default value to set for the input string if it is empty.
 *
 * @note This function modifies the input string directly if it is empty.
 */
void set_default_if_empty(char* input, const char* default_value);

/**
 * @brief Set a value for a string by prioritizing an environment variable or a default value.
 *
 * This function sets the value of the destination string by prioritizing the value of an environment 
 * variable (if not null or empty). If the environment variable is null or empty, it uses 
 * the provided default value.
 *
 * @param destination The string to set the value for.
 * @param env_value The value of an environment variable (or NULL if not set).
 * @param default_value The default value to use if the environment variable is not set or empty.
 *
 * @note This function modifies the destination string directly based on the environment variable or 
 * the default value.
 */
void set_default_or_env(char* destination, const char* env_value, const char* default_value);

/**
 * @brief Get user input and store it in a string, optionally displaying a message.
 *
 * This function reads user input from the standard input (stdin) and stores it in the provided destination string. 
 * It can also display an optional message to the user before input is collected. 
 * The function removes the newline character from the input.
 *
 * @param destination The string where the user input will be stored.
 * @param size_of_dest The size of the destination string to prevent buffer overflows.
 * @param interactive A flag indicating whether to display a message to the user (1 for yes, 0 for no).
 * @param message The optional message to display to the user before collecting input.
 *
 * @return 0 if input is successfully read and stored; 1 in case of an error, such as failure to read input.
 *
 * @note This function modifies the destination string and removes the newline character from the input.
 */
int get_config_input(char* destination,size_t size_of_dest, int interactive, char* message);

#endif