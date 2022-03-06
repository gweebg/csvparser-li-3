#ifndef COMMITDATA_H
#define COMMITDATA_H

#include "../include/ex3/statistics.h"

/**
 * @brief Struct that represents a commit.
 */
typedef struct commit_registry commit_reg;

/**
 * @brief Struct that represents a commit catalog.
 */
typedef struct commit_catalog commit_cat;

/**
 * @brief Create a commit object, given a csv string.
 * @param csv_line line containing the parameters for the commit.
 * @return commit_reg, the commit struct we populated.
 */
void create_commit(char* csv_line, commit_reg *new_commit);

/**
 * @brief Creates a 'database' containing every single commit from the csv file.
 * @param db where we are storing the commit.
 * @param db_storage needed to keep the pointers in place for the hash table.
 * @param filename csv file filename.
 * @return Returns the number of commits added.
 */
bool build_commits(commit_cat *catalog, const char *filename);

bool build_commits_catalog(commit_cat *new_catalog, stats *statistics, const char *filename);
int compare(const void * x, const void * y);
int *get_committer_ids(commit_cat *commits);
int get_committer_ids_size(commit_cat *commits);
void dump_commit(commit_reg *commit);

/**
 * @brief Check if an user is a collaborator.
 * This is,checks if the user has at least one commit made on any repository.
 * @param id the id of the user.
 * @param commits where we will search by the id.
 * @return True, if the user is a collaborator. False otherwise.
 */
bool is_collaborator(long id, commit_cat *commits);

/**
 * @brief Since commit_reg is an opaque structure, we can't get it's size using sizeof, so we need a function that does that for us.
 * @return int size of commit_reg
 */
int get_commit_size();

/**
 * @brief Since commit_cat is an opaque structure, we can't get it's size using sizeof, so we need a function that does that for us.
 * @return int, size of commit_cat
 */
int get_commit_cat_size();

long get_committer_id(commit_cat *commits, int index);
long get_repo_id(commit_cat *commits, int index);

int bound_date_index(commit_cat *commits, char *date_string);
char *get_string_date(commit_cat *commits, int index);

commit_reg* get_commit(commit_cat *commits, int index);
int get_message_size (commit_cat *commits, int index);

#endif