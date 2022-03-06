#ifndef REPODATA_H
#define REPODATA_H

#include "../include/ex3/statistics.h"

/**
 * @brief Struct that represents a repository.
 */
typedef struct repo_registry repo_reg;

/**
 * @brief Struct that represents a repository catalog.
 */
typedef struct repo_catalog repo_cat;

/**
 * @brief Create a repository object, given a csv string.
 * @param csv_line line containing the parameters for the repo.
 * @return repo_reg, the repo struct we populated.
 */
void create_repo(char* csv_line, repo_reg *new_repo);

/**
 * @brief Creates a 'database' containing every single repository from the csv file.
 * @param db where we are storing the repos.
 * @param db_storage needed to keep the pointers in place for the hash table.
 * @param filename csv file filename.
 * @return Returns the number of repos added.
 */
bool build_repos(repo_cat* repositories, const char *filename);
/**
 * @brief 
 * 
 * @param new_catalog 
 * @param statistics 
 * @param filename 
 * @return true 
 * @return false 
 */
bool build_repos_catalog(repo_cat *new_catalog, stats *statistics, const char *filename);

/**
 * @brief Since repo_reg is an opaque structure, we can't get it's size using sizeof, so we need a function that does that for us.
 * @return int size of repo_reg
 */
int get_repo_size();

int get_repos_cat_size();

char* get_repo_language(repo_cat *repos, long repo_id);

char* get_updated_at(repo_cat *repos, int index);

int get_repo_array_size(repo_cat *repos);

long get_repo_id_by_index(repo_cat *repos, int index);

char* get_repo_description(repo_cat *repos, int index);

int future_date_index(repo_cat *repos, char *date_string);

char* get_repo_lang_by_index(repo_cat *repos, int index);

void destroy_repo_catalog(repo_cat *repos);

int get_owner_by_id(repo_cat *repos, long repo_id);

#endif