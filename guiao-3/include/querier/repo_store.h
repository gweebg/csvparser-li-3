#include <stdbool.h>
#include <glib.h>
#include "statistics.h"
#include "utils.h"

#ifndef REPO_STORE_H
#define REPO_STORE_H

typedef struct repo_catalog repo_cat;
bool store_repos(repo_cat *repo_catalog, stats *statistics, const char *filename);
char *get_language_from_id(repo_cat *repos, int repo_id);
char *get_description_from_id(repo_cat *repos, int repo_id);
int get_owner_id_by_repo(repo_cat *repos, int repo_id);
void make_results_7(int *results, GHashTable *table, repo_cat *repos, int *final_size);
int search_dates_repos(repo_cat *repos, datetime *date);
void make_table_8(repo_cat *repos, GHashTable *table, int start);

#endif