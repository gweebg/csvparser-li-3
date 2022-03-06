#pragma once

#include <stdbool.h>
#include <glib.h>

#include "statistics.h"
#include "repo_store.h"
#include "user_store.h"
#include "utils.h"

typedef struct commit_catalog commit_cat;

bool store_commits(commit_cat *commit_catalog, stats *statistics, const char *filename);
void get_committer_id_list(commit_cat *commits, int *dest, int size);
void get_author_id_list(commit_cat *commits, int *dest, int size);
int search_dates(commit_cat *commits, datetime *date);
void make_table_5(commit_cat *commits, GHashTable *table, int start, int end);
void make_table_6(commit_cat *commits, repo_cat *repos, GHashTable *table, char *language);
int get_commit_array_size(commit_cat *commits);
void make_table_7(commit_cat *commits, repo_cat *repos, GHashTable *table, int start);
void make_table_9(commit_cat *commits, repo_cat *repos, user_cat *users, GHashTable *table);
void make_table_10(commit_cat *commits, repo_cat *repos, GHashTable *table);
