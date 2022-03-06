#ifndef USERDATA_H
#define USERDATA_H

#include "../include/ex3/statistics.h"
#include "commitdata.h"

/**
 * @brief Struct that represents a user.
 */
typedef struct user_registry user_reg;

/**
 * @brief Struct that represents a user catalog.
 */
typedef struct user_catalog user_cat;

/**
 * @brief Sorts an array of user_reg, using the quicksort algorithm.
 * @param user_array array we want to sort.
 * @param first where should the sorting start.
 * @param last where should the sorting end.
 */
void sort_user_array_by_id(user_reg **user_array, int first, int last);
/**
 * @brief Create a user object, given a csv string.
 * @param csv_line line containing the parameters for the user.
 * @return user_reg, the user struct we populated.
 */
void create_user(char* csv_line, user_reg *new_user);

/**
 * @brief Creates a 'database' containing every single user from the csv file.
 * @param db where we are storing the users.
 * @param db_storage needed to keep the pointers in place for the hash table.
 * @param filename csv file filename.
 * @return Returns the number of users added.
 */
bool build_users(user_cat *user_catalog, stats *statistics, const char *filename);

/**
 * @brief Builds and populates an user catalog containing different data structures that store either user_reg or valueable information about the users in general.
 * @param new_catalog the catalog we will populate.
 * @param statistics the statistics catalog we will populate.
 * @param filename the absolute path where the csv file is.
 * @return True, if the building and populating were successful. False otherwise.
 */
bool build_user_catalog(user_cat *new_catalog, stats *statistics, const char *filename);

/**
 * @brief Uses the binary search algorithm to check if an user is present or not on an sorted array of user_reg.
 * @param user_array the array we want to lookup.
 * @param start where to start looking.
 * @param end where to stop looking.
 * @param key the value we are looking for.
 * @return The index in the array where the user is located. -1 if not found.
 */
int user_bin_search(user_reg **user_array, int start, int end, int key);

/**
 * @brief Verifies if a user matches the given type. Checks the user by it's id.
 * The compare_type parameter goes from 1 to 3 : 1 being the TYPE_USR, 2 being the TYPE_BOT and 3 the TYPE_ORG.
 * @param catalog the catalog containing the user_reg array.
 * @param compare_type the type we want to match. 
 * @param id the user id.
 * @return True, if the types match. False otherwise
 */
bool check_user_type(user_cat *catalog, int compare_type, int id);

/**
 * @brief Utility function to destroy a user catalog by freeing every freeable parameter or sub parameter.
 * @param catalog catalog we want to destroy.
 */
void destroy_user_catalog(user_cat *catalog);

int compute_total_colabs(user_cat *users, commit_cat *commits);

void dump_user(user_reg* user);

/**
 * @brief Get the if from an user.
 * @param user the user. 
 * @return Returns it's id.
 */
int get_user_id(user_reg *user);

int get_user_array_size(user_cat *cat);

/**
 * @brief Since user_reg is an opaque structure, we can't get it's size using sizeof, so we need a function that does that for us.
 * @return int size of user_reg
 */
int get_user_size();

/**
 * @brief Since user_cat is an opaque structure, we can't get it's size using sizeof, so we need a function that does that for us.
 * @return int size of user_cat
 */
int get_user_cat_size();

char* get_username_by_id(user_cat *users, long id);
bool is_friend(user_cat *users, long my_id, long owner_id);
int get_user_id_by_index(user_cat *users, int index);
char* get_username_by_index(user_cat *users, int index);




#endif