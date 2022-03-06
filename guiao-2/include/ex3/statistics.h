#ifndef STATS_H
#define STATS_H

typedef struct statistics stats;

void init_stats(stats* s);

int get_size_stats();

void update_total_users(stats* statistics ,int value);
void update_total_commits(stats* statistics ,int value);
void update_total_repos(stats* statistics ,int value);

int get_user_count(stats* statistics);
int get_bot_count(stats* statistics);
int get_org_count(stats* statistics);

void update_bot_count(stats* statistics ,int value);
void update_user_count(stats* statistics ,int value);
void update_org_count(stats* statistics, int value);

int get_total_users(stats* statistics);
int get_total_repos(stats* statistics);
int get_total_commits(stats* statistics);

int get_avg_colab(stats* statistics);
void update_avg_commits(stats* statistics, double value);

void dump_stats(stats* statistics);

void update_total_colabs(stats *statistics ,int value);
int get_total_colabs(stats *statistics);

double get_avg_commits(stats *statistics);
void update_repos_with_bots(stats *statistics, int value);
int get_repos_with_bots(stats *statistics);


#endif