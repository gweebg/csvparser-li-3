/**
 * @file clean_users.h
 * @brief Clean up functions for the user module.
 * @author gweebg
 * @copyright Copyright (c) 2021
 */

#ifndef UCLEAN_H
#define UCLEAN_H

/** Max line size on the file. */
#define MAX_SIZE 200000

#include <glib.h>
#include <stdbool.h>

/**
 * @brief Retrieves the first integer from a csv string.
 * @param string comma separated string containing the number we want.
 * @return The integer parsed from the string.
 */
int get_number(char *string);

/**
 * @brief Retrieves the second integer from a csv string.
 * @param string the comma separated string containing the number we want.
 * @return The integer parsed from the string.
 */
int get_second_number(char *string);

/**
 * @brief Saves the id parameter into a hash table and generates the final version of the file (users-final.csv).
 * @param filename the filename of our file (users-ok.csv).
 * @param storage parameter needed so we don't lose our hash table pointers.
 * @return The GHashTable containing the id parameter.
 */
GHashTable *clean_users(char const *filename, int *storage);

#endif