#include <stdbool.h>
#include "user_store.h"
#include "repo_store.h"
#include "commit_store.h"
#include "query_parser.h"

#ifndef EXEC_H
#define EXEC_H

void qtty_usr_bot_org(stats *statistics, int id);
void avg_colab_per_repo(stats *statistics, user_cat *users, commit_cat *commits, int id);
void repos_with_bots(stats *statistics, int id);
void avg_commits_per_usr(stats *statistics, int id);
void commit_leaderboard(user_cat *users, commit_cat *commits, char *params, int id);
void commit_leaderboard_lang(user_cat *users, commit_cat *commits, repo_cat *repos, char *params, int id);
void unactive_repos(commit_cat *commits, repo_cat *repos, char *params, int id);
void top_languages(repo_cat *repos, char *params, int id);
void top_commits_friends(user_cat *users, commit_cat *commits, repo_cat *repos, char *params, int id);
void top_user_repo_message(user_cat *users, commit_cat *commits, repo_cat *repos, char *params, int id);

bool execute(QUERY *query, user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics);

#endif