#include "repo_store.h"
#include "statistics.h"
#include "user_store.h"
#include "commit_store.h"

#ifndef QP_H
#define QP_H

#define USERS "./entrada/users-final.csv"
#define COMMITS "./entrada/commits-final.csv"
#define REPOS "./entrada/repos-final.csv"

// #define USERS "./entrada/users-final.csv"
// #define COMMITS "./entrada/commits-final.csv"
// #define REPOS "./entrada/repos-final.csv"

void query_handler(user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics, int query_id, int id);
void store(user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics);

#endif