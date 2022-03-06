#ifndef STATS_H
#define STATS_H

typedef struct statistics stats;

void init_stats(stats* s);

int get_size_stats();

void set_total_colabs_bots(stats *s, int value);
void set_total_users(stats* statistics ,int value);
void set_total_commits(stats* statistics ,int value);
void set_total_repos(stats* statistics ,int value);

int get_user_count(stats* statistics);
int get_bot_count(stats* statistics);
int get_org_count(stats* statistics);

void set_bot_count(stats* statistics ,int value);
void set_user_count(stats* statistics ,int value);
void set_org_count(stats* statistics, int value);

int get_total_users(stats* statistics);
int get_total_repos(stats* statistics);
int get_total_commits(stats* statistics);

int get_avg_colab(stats* statistics);
void set_avg_commits(stats* statistics, double value);

void dump_stats(stats* statistics);

void set_total_colabs(stats *statistics ,int value);
int get_total_colabs(stats *statistics);

double get_avg_commits(stats *statistics);
void set_repos_with_bots(stats *statistics, int value);
int get_repos_with_bots(stats *statistics);

void destroy_stats(stats *statistics);
int get_total_repos_bot_colab(stats* statistics);

#endif