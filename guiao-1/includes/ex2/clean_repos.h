/**
 * @file clean_repos.h
 * @author gweebg
 * @copyright Copyright (c) 2021
 */

#ifndef RCLEAN_H
#define RCLEAN_H

/** Max line size on the file repo.csv. */
#define REPO_SIZE 70000

#include <glib.h>

/**
 * @brief Generates a GHashTable containing the id parameter of a repo file.
 * @param filname the name of our file (repos-ok.csv).
 * @param storage parameter needed so we don't lose our hash table pointers.
 * @return GHashTable* the hash table containing the id parameter.
 */
GHashTable *id_to_hashset(char* filename, int* storage);

/**
 * @brief Cleans up the repos file by comparing various values within the other files.
 * @param filename the name of our file (repos-ok.csv).
 * @param user_table GHashTable returned by the clean_users function.
 * @param commit_table GHashTable returned by the clean_commits function.
 * @param to_ignore Array where we are goint to insert the lines to ignore.
 * @param to_ignore_size Size of the "to_ignore" array.
 * @return true if there were no errors.
 */
bool clean_repos(char* filename, GHashTable *user_table, GHashTable *commit_table, int *to_ignore, int *to_ignore_size);

#endif