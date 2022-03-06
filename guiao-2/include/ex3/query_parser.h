#ifndef QPARSER_H
#define QPARSER_H

#include <stdbool.h>

#define BUFFER_SIZE 500

/**
 * @brief Checks a file exists given it's name.
 * @param filename name of the file.
 * @return True, if the file exists.
 */
bool file_exists(const char *filename);

/**
 * @brief Given a filename, checks a file has a determinate file extension.
 * @param filename name of the file. 
 * @param ext extension we are comparing with.
 * @return True, if it has the given extension.
 */
bool check_ext(const char *filename, const char *ext);

/**
 * @brief Writes the text into a file.
 * @param filepath name, path of the file.
 * @param body text to write on the file.
 * @return True if the write was successful. 
 */
bool write_to_file(const char *filepath, char* body);

/**
 * @brief Reads the query file line by line and executes the query if valid.
 * @param filename name of the query file. 
 * @return True, if every query was executed 
 */
bool read_queries(const char *filename);

#endif