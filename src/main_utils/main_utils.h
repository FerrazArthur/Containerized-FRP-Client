#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

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
 * Process a command-line argument and set the interactive flag.
 *
 * @param arg (char*): The command-line argument to be processed.
 * @param interactive (int*): A pointer to an integer variable to indicate interactive mode.
 *
 * @return
 *   - 0: Indicates successful processing and that the program should run interactively.
 *   - 1: Indicates an unknown argument error.
 *   - 2: Indicates that the help message was displayed, and the program should exit.
 */
int process_arg(char* arg, int *interactive);

/**
 * Read and process command-line arguments.
 *
 * @param argc (int): The number of command-line arguments.
 * @param argv (char*[]): An array of command-line argument strings.
 * @param interactive (int*): A pointer to an integer variable to indicate interactive mode.
 *
 * @return
 *   - 0: Indicates successful processing and that the program should run interactively.
 *   - 1: Indicates an unknown argument error.
 *   - 2: Indicates that the help message was displayed, and the program should exit.
 */
int read_args(int argc, char* argv[], int* interactive);

#endif