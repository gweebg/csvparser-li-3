#pragma once

#include <stdbool.h>
#include "statistics.h"

typedef struct commit_catalog commit_cat;
typedef struct user_catalog user_cat;

bool store_users(user_cat *user_catalog, stats *statistics, const char *filename);
void compute_colaborators(stats *statistics, user_cat *users, commit_cat *commits);
char* get_username_by_id(user_cat *users, long id);
bool is_friend(user_cat *users, int id, int owner_id);
