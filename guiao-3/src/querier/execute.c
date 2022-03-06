#include <glib.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>  
#include <stdbool.h>

#include "../include/querier/users.h"

#include "../include/querier/user_store.h"
#include "../include/querier/repo_store.h"
#include "../include/querier/commit_store.h"

#include "../include/querier/query_parser.h"
#include "../include/querier/utils.h"

void qtty_usr_bot_org(stats *statistics, int id)
{
    int users = get_user_count(statistics),
        bots  = get_bot_count(statistics),
        orgs  = get_org_count(statistics);
    
    char* out = "Bot: %d\nOrganization: %d\nUser: %d\n";
    char* put = malloc(sizeof(char) * 1024);
    char* filename = malloc(sizeof(char) * 64);

    sprintf(filename, "./saida/command%d_output.txt", id);
    sprintf(put, out, bots, orgs, users);
    
    write_to_file(filename, put);
    free(put); free(filename);
}

void avg_colab_per_repo(stats *statistics, user_cat *users, commit_cat *commits, int id)
{
    compute_colaborators(statistics, users, commits);

    int total_colaborators = get_total_colabs(statistics),
        total_repositories = get_total_repos(statistics);

    double average = ((double)total_colaborators / (double)total_repositories); 

    char* out = "Average collaborator per repository: %.2f collaborators per repository.\n";
    char* put = malloc(sizeof(char) * 1024);
    char* filename = malloc(sizeof(char) * 64);

    sprintf(filename, "./saida/command%d_output.txt", id);

    sprintf(put, out, average);
    write_to_file(filename, put); 
    free(put); free(filename);
}

void repos_with_bots(stats *statistics, int id)
{
    int total_bot_colab = get_total_repos_bot_colab(statistics);

    char* out = "Total repositories with bots as collaborators : %d repositories.\n";
    char* put = malloc(sizeof(char) * 1024);
    char* filename = malloc(sizeof(char) * 64);

    sprintf(filename, "./saida/command%d_output.txt", id);

    sprintf(put, out, total_bot_colab);
    write_to_file(filename, put); 
    free(put); free(filename);
}

void avg_commits_per_usr(stats *statistics, int id)
{
    int total_users  = get_user_count(statistics) + get_org_count(statistics) + get_bot_count(statistics),
        total_commit = get_total_commits(statistics);

    double average = ((double)total_commit / (double)total_users); 

    char* out = "Each user has an average of %.2f commits.\n";
    char* put = malloc(sizeof(char) * 1024);
    char* filename = malloc(sizeof(char) * 64);

    sprintf(filename, "./saida/command%d_output.txt", id);

    sprintf(put, out, average);
    write_to_file(filename, put); 
    free(put); free(filename);
}

void extract_number(char **string, int *output)
{
    char* rest = *string;
    char* str_num = strtok_r(rest, " ", &rest);

    *output = atoi(str_num);
    *string = rest;
}

void extract_date(char **string, datetime *date)
{
    char* rest = *string;
    char* str_num = strtok_r(rest, " ", &rest);

    int year = 0, month = 0, day = 0;
    date->hours = 0; date->minutes = 0; date->seconds = 0; date->str_repr = NULL;

    sscanf(str_num, "%4d-%2d-%2d", &year, &month, &day);
    date->year = year; date->month = month; date->day = day;

    *string = rest;
}

void commit_leaderboard(user_cat *users, commit_cat *commits, char *params, int id)
{
    int n_first = 0;
    datetime *date_a = xmalloc(sizeof(datetime));
    datetime *date_b = xmalloc(sizeof(datetime));

    extract_number(&params, &n_first);
    extract_date(&params, date_a);
    extract_date(&params, date_b);

    int start_index = search_dates(commits, date_a),
        end_index   = search_dates(commits, date_b);

    GHashTable *table = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, g_free);
    make_table_5(commits, table, start_index, end_index);

    GHashTableIter iter;
    set *result = xmalloc(sizeof(set) * g_hash_table_size(table));
    int *key_, *val, counter = 0;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        set elem = {.user_id = *key_, .occurs = *val};
        result[counter] = elem;
        counter++;
    }

    qsort(result,
          g_hash_table_size(table),
          sizeof(set),
          compare_sets);
    
    char* filename = malloc(sizeof(char) * 64);
    sprintf(filename, "./saida/command%d_output.txt", id);

    FILE* output = fopen(filename, "w");
    if (!output) return;

    char* line = NULL;
    for (int i = 0; i < n_first; i++)
    {
        char *username = get_username_by_id(users, result[i].user_id);
        int occurs = result[i].occurs;
        long uid = result[i].user_id;

        line = malloc(sizeof(char) * 1024);
        sprintf(line, "%ld;%s;%d\n", uid, username, occurs);
        fputs(line, output);
        free(line);
    }

    g_hash_table_destroy(table);
    fclose(output);
    free(filename);
    free(result);
}

void commit_leaderboard_lang(user_cat *users, commit_cat *commits, repo_cat *repos, char *params, int id)
{
    int n_first = 0;
    extract_number(&params, &n_first);

    GHashTable *table = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, g_free);
    make_table_6(commits, repos, table, params);

    GHashTableIter iter;
    set *result = xmalloc(sizeof(set) * g_hash_table_size(table));
    int *key_, *val, counter = 0;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        set elem = {.user_id = *key_, .occurs = *val};
        result[counter] = elem;
        counter++;
    }

    qsort(result,
          g_hash_table_size(table),
          sizeof(set),
          compare_sets);
    
    char* filename = malloc(sizeof(char) * 64);
    sprintf(filename, "./saida/command%d_output.txt", id);

    FILE* output = fopen(filename, "w");
    if (!output) return;

    char* line = NULL;
    for (int i = 0; i < n_first; i++)
    {
        char *username = get_username_by_id(users, result[i].user_id);
        int occurs = result[i].occurs;
        long uid = result[i].user_id;

        line = malloc(sizeof(char) * 1024);
        sprintf(line, "%ld;%s;%d\n", uid, username, occurs);
        fputs(line, output);
        free(line);
    }

    g_hash_table_destroy(table);
    fclose(output);
    free(filename);
    free(result);
}

void unactive_repos(commit_cat *commits, repo_cat *repos, char *params, int id)
{
    datetime *date = xmalloc(sizeof(datetime));
    extract_date(&params, date);

    int look_index = search_dates(commits, date);

    GHashTable *table = g_hash_table_new(g_int_hash, g_int_equal);
    make_table_7(commits, repos, table, look_index);

    int size = 0;
    int *valid_ids = xmalloc(sizeof(int) * g_hash_table_size(table));

    make_results_7(valid_ids, table, repos, &size);

    char* filename = malloc(sizeof(char) * 64), *line = NULL;
    sprintf(filename, "./saida/command%d_output.txt", id);

    FILE* output = fopen(filename, "w");
    if (!output) return;

    for (int i = 0; i < size; i++)
    {
        int repo_id = valid_ids[i];
        char *description = get_description_from_id(repos, repo_id);

        line = malloc(sizeof(char) * 8192);
        sprintf(line, "%d;%s\n", repo_id, description);
        fputs(line, output);
        free(line);
    }

    g_hash_table_destroy(table);
    free(valid_ids);
}

void top_languages(repo_cat *repos, char *params, int id)
{
    datetime *date = xmalloc(sizeof(datetime));
    int n_first = 0;

    extract_number(&params, &n_first);
    extract_date(&params, date);

    int start_index = search_dates_repos(repos, date);

    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);
    make_table_8(repos, table, start_index);

    GHashTableIter iter;
    int *val, counter = 0;
    char *key_; 
    set_v2 *results = xmalloc(sizeof(set_v2) * g_hash_table_size(table));

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        set_v2 elem = { .string = key_, .repo_id = *val };
        results[counter] = elem;
        counter++;
    }

    qsort(results, 
          counter,
          sizeof(set_v2),
          compare_sets_v2);

    char* filename = malloc(sizeof(char) * 64);
    sprintf(filename, "./saida/command%d_output.txt", id);

    FILE* output = fopen(filename, "w");
    if (!output) return;

    char* line = NULL;
    for (int i = 0; i < n_first; i++)
    {
        line = malloc(sizeof(char) * 1024);
        sprintf(line, "%s\n", results[i].string);
        fputs(line, output);
        free(line);
    }

    g_hash_table_destroy(table);
    free(results);
    free(filename);
    fclose(output);
}

void top_commits_friends(user_cat *users, commit_cat *commits, repo_cat *repos, char *params, int id)
{
    int n_first = 0;
    extract_number(&params, &n_first);

    GHashTable *table = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, g_free);
    make_table_9(commits, repos, users, table);

    GHashTableIter iter;
    int *key_, *val, size = g_hash_table_size(table), counter = 0;
    set *results = xmalloc(sizeof(set) * size);

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        set elem = { .user_id = (int)*key_ , .occurs = *val };
        results[counter++] = elem;
    }

    qsort(results, 
          size, 
          sizeof(set),
          compare_sets);

    char* filename = malloc(sizeof(char) * 64);
    sprintf(filename, "./saida/command%d_output.txt", id);

    FILE* output = fopen(filename, "w");
    if (!output) return;

    char* line = NULL;
    if (n_first > size) n_first = size;
    for (int i = 0; i < n_first; i++)
    {
        char *username = get_username_by_id(users, results[i].user_id);
        long uid = results[i].user_id;

        line = malloc(sizeof(char) * 1024);
        sprintf(line, "%ld;%s\n", uid, username);
        fputs(line, output);
        free(line);
    }

    free(results); free(filename);
    g_hash_table_destroy(table);
    fclose(output);
}

void top_user_repo_message(user_cat *users, commit_cat *commits, repo_cat *repos, char *params, int id)
{
    int n_first = 0;
    extract_number(&params, &n_first);

    GHashTable *table = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, g_free);
    make_table_10(commits, repos, table);

    commit_array *val;
    GHashTableIter iter;
    int *key_;

    char* filename = malloc(sizeof(char) * 64);
    sprintf(filename, "./saida/command%d_output.txt", id);

    FILE* output = fopen(filename, "w");
    if (!output) return;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        char *line = NULL;
        char *title = malloc(sizeof(char) * 256);

        sprintf(title, "=================================== REPO: %d ===================================\n", *key_);
        fputs(title, output);

        if (n_first > val->size) n_first = val->size;
        for (int i = 0; i < n_first; i++) 
        {
            char* username = get_username_by_id(users, val->array[i]->committer_id);
            int user_id = val->array[i]->committer_id;
            int size = val->array[i]->message_size;
            int repo_id = *key_;

            line = malloc(sizeof(char) * 1024);
            sprintf(line, "%d;%s;%d;%d\n", user_id, username, size, repo_id);
            fputs(line, output);
            free(line);
        }

        free(title);
    }

    free(filename);
    fclose(output);
    g_hash_table_destroy(table);
}

bool execute(QUERY *query, user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics)
{
    int id = query->query_id;
    int num = query->number;

    if (id == 1)  qtty_usr_bot_org(statistics, num);
    if (id == 2)  avg_colab_per_repo(statistics, users, commits, num);
    if (id == 3)  repos_with_bots(statistics, num);
    if (id == 4)  avg_commits_per_usr(statistics, num);
    if (id == 5)  commit_leaderboard(users, commits, strdup(query->param), num);
    if (id == 6)  commit_leaderboard_lang(users, commits, repos, strdup(query->param), num);
    if (id == 7)  unactive_repos(commits, repos, strdup(query->param), num);
    if (id == 8)  top_languages(repos, strdup(query->param), num);
    if (id == 9)  top_commits_friends(users, commits, repos, strdup(query->param), num);
    if (id == 10) top_user_repo_message(users, commits, repos, strdup(query->param), num);

    return true;
}

