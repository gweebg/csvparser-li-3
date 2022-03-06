/**
 * @file clean_commits.h
 * @author gweebg
 * @copyright Copyright (c) 2021
 */

#ifndef CCLEAN_H
#define CCLEAN_H

/** Max line size on the file commits.csv. */
#define COMMIT_SIZE 70000

#include <stdbool.h>
#include <glib.h>

/**
 * @brief Retrieves the third integer from a csv string.
 * @param string the comma separated string containing the number we want.
 * @return The integer parsed from the string.
 */
int get_third_number(char* string);

/**
 * @brief Cleans up the commits file by comparing various values within the other files, and saves the repo_id parameter into a hash table.
 * @param filename the name of our file (commits-ok.csv)
 * @param user_table GHashTable returned by the clean_users function.
 * @param repo_table GHashTable returned by the id_to_hashtable function.
 * @param storage parameter needed so we don't lose our hash table pointers.
 * @param to_ignore Array where we are goint to insert the lines to ignore.
 * @param size Size of the "to_ignore" array.
 * @return GHashTable* 
 */
GHashTable *clean_commits(char const *filename, GHashTable *user_table, GHashTable *repo_table, int *storage, int *to_ignore, int *size);

#endif