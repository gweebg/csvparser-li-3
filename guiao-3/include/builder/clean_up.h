/**
 * @file clean_users.h
 * @brief Clean up functions for every module.
 * @author gweebg
 * @copyright Copyright (c) 2021
 */

#ifndef CLEANUP_H
#define CLEANUP_H

#include <stdbool.h>
#include <glib.h>

/**
 * @brief Binary searches an element in an ordered array.
 * @param array array that's going to be searched.
 * @param start where to start looking.
 * @param end where to stop looking.
 * @param value value we are looking for.
 * @return true if the value is present in the array.
 * @return false if the value is not present in the array.
 */
bool contains(int *array, int start, int end, int value);

/**
 * @brief Writes the final files for a given filename.
 * Given a file and an array containing the lines to ignore, this function generates the same file given without the lines marked by the array.
 * @param filename target file.
 * @param pathname file we are going to create.
 * @param to_ignore array containing the lines to ignore.
 * @param size size of the array.
 * @param FILE_MAX_SIZE max line size on the given file.
 * @return true if the write was a success.
 */
bool write_changes(char* filename, char* pathname, int *to_ignore, int *size, int FILE_MAX_SIZE);

/**
 * @brief Cleans up every type of file.
 * Cleans up all users-ok.csv, commits-ok.csv and repos-ok.csv.
 * @return true if the clean was a success.
 */
bool clean_up();

#endif