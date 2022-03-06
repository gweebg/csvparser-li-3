#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/ex3/statistics.h"

typedef struct statistics
{

    double avg_colab_per_repo,
           avg_commit_per_user;

    int total_colab,
        total_repos_with_bots;

    int bot_count,
        user_count,
        org_count;
    
    int total_users,
        total_commits,
        total_repos;

    int repos_with_bots;

} stats;

void init_stats(stats *s)
{
    s->avg_colab_per_repo = -1;
    s->avg_commit_per_user = -1;
    s->total_users = -1;
    s->total_colab = -1;
    s->total_repos_with_bots = -1;
    s->total_commits = -1;
    s->total_repos = -1;
    s->repos_with_bots = -1;
}

void update_total_users(stats* statistics ,int value)
{
    statistics->total_users = value;
}

void update_total_commits(stats* statistics ,int value)
{
    statistics->total_commits = value;
}

void update_total_repos(stats* statistics ,int value)
{
    statistics->total_repos = value;
}

void update_bot_count(stats* statistics ,int value)
{
    statistics->bot_count = value;
}

void update_user_count(stats* statistics ,int value)
{
    statistics->user_count = value;
}

int get_user_count(stats* statistics)
{
    return statistics->user_count;
}
int get_bot_count(stats* statistics)
{
    return statistics->bot_count;
}
int get_org_count(stats* statistics)
{
    return statistics->org_count;
}

void update_org_count(stats* statistics, int value)
{
    statistics->org_count = value;
}

int get_total_users(stats* statistics)
{
    int r = statistics->total_users;
    return r;
}

int get_total_commits(stats* statistics)
{
    int r = statistics->total_commits;
    return r;
}

int get_total_repos(stats* statistics)
{
    int r = statistics->total_repos;
    return r;
}

void dump_stats(stats* statistics)
{
    printf("===STATISTICS==================\n");
    printf("Total Users: %d\n", statistics->total_users);
    printf("Total Commits: %d\n", statistics->total_commits);
    printf("Total Repos: %d\n", statistics->total_repos);
    printf("Total User Bot: %d\n", statistics->bot_count);
    printf("Total User Org: %d\n", statistics->org_count);
    printf("Total User User: %d\n", statistics->user_count);
    printf("===============================\n");
}

int get_avg_colab(stats *statistics)
{
    return statistics->avg_colab_per_repo;
}

int get_size_stats()
{
    return (int)sizeof(stats);
}

void update_total_colabs(stats *statistics, int value)
{
    statistics->total_colab = value;
}

int get_total_colabs(stats *statistics)
{
    return statistics->total_colab;
}

void update_avg_commits(stats *statistics, double value) 
{
    statistics->avg_commit_per_user = value;
}

double get_avg_commits(stats *statistics)
{
    return statistics->avg_commit_per_user;
}

void update_repos_with_bots(stats *statistics, int value)
{
    statistics->repos_with_bots = value;
}

int get_repos_with_bots(stats *statistics)
{
    return statistics->repos_with_bots;
}