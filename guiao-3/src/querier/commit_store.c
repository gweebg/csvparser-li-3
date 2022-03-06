#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <glib.h>

#include "../include/querier/storing_tools.h"
#include "../include/querier/commit_store.h"
#include "../include/querier/statistics.h"
#include "../include/querier/utils.h"
#include "../include/querier/commits.h"
#include "../include/querier/repo_store.h"
#include "../include/querier/repos.h"
#include "../include/querier/user_store.h"

typedef struct commit_catalog
{
    commit_reg **commits; /* Sorted by date */
    int *committer_id_list;
    int *author_id_list;
    int size;

} commit_cat;

/* GETTERS */

void get_committer_id_list(commit_cat *commits, int *dest, int size)
{
    for (int i = 0; i < size; i++)
        dest[i] = commits->committer_id_list[i];
}

void get_author_id_list(commit_cat *commits, int *dest, int size)
{
    for (int i = 0; i < size; i++)
        dest[i] = commits->author_id_list[i];
}

void create_commit(char* csv_line, commit_reg *new_commit)
{
    //repo_id;author_id;committer_id;commit_at;message
    char* empty_string = "\0", *message;
    long repo_id, author_id, committer_id;
    datetime *commit_at;
    
    csv_to_number(&csv_line, &repo_id);
    csv_to_number(&csv_line, &author_id);
    csv_to_number(&csv_line, &committer_id);
    csv_to_datetime(&csv_line, &commit_at);

    if (csv_line[0] != '\0')
        csv_to_string(&csv_line, &message);
    else
    {
        message = malloc(sizeof(char) * 2);
        strncpy(message, empty_string, 2);
    }

    set_repo_id(new_commit, repo_id);
    set_author_id(new_commit, author_id);
    set_comitter_id(new_commit, committer_id);
    set_commit_at(new_commit, commit_at);
    set_message(new_commit, message);

    free(message);
    // free_datetime(commit_at);
}

bool store_commits(commit_cat *commit_catalog, stats *statistics, const char *filename)
{
    // printf("(store_commits)\n");
    // printf("    > Opening file %s... ", filename);

    int lines = count_lines(filename);

    FILE *ufile = fopen(filename, "r");
    if (!ufile)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    // printf("Done!\n");
    // printf("    > Reading %s... ", filename);

    int total_commits = 0;

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, ufile);
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, ufile);     

    commit_catalog->commits           = xmalloc(get_commit_size() * (lines - 1));   
    commit_catalog->committer_id_list = xmalloc(sizeof(int) * (lines - 1));
    commit_catalog->author_id_list    = xmalloc(sizeof(int) * (lines - 1));

    while (line_size >= 0)
    {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0;

        size_t commit_size = get_commit_size();
        commit_reg *commit = xmalloc(commit_size);

        create_commit(line_buffer, commit);

        commit_catalog->committer_id_list[total_commits] = get_comitter_id(commit);
        commit_catalog->author_id_list[total_commits] = get_author_id(commit);
        commit_catalog->commits[total_commits] = commit;

        total_commits++;
        line_size = getline(&line_buffer, &line_buffer_size, ufile);
    }

    commit_catalog->size = total_commits;
    set_total_commits(statistics, total_commits);

    qsort(commit_catalog->committer_id_list,
          total_commits,
          sizeof(int),
          compare_int);

    qsort(commit_catalog->author_id_list,
          total_commits,
          sizeof(int),
          compare_int);    

    qsort(commit_catalog->commits, 
          total_commits,
          sizeof(datetime*),
          compare_dates);

    // printf("Done!\n");
    // printf("    > Data stored successfully!\n");

    fclose(ufile);
    if (line_buffer) free(line_buffer);

    return true;
}

int search_dates(commit_cat *commits, datetime *date)
{
    int index = -1, l = 0;
    int size = commits->size;

    while (l <= size)
    {
        int mid = (l + size) / 2;

        datetime *date_comp = xmalloc(sizeof(datetime));
        get_commit_at(commits->commits[mid], date_comp);

        int comp = compare_datetimes(date, date_comp);

        if (comp == 0)
        {
            index = mid;
            break;
        }
        else if (comp < 0)
        {
            size = mid - 1;
            index = mid;
        }
        else l = mid + 1;

        free_datetime(date_comp);
    }

    return index;  
}

void make_table_5(commit_cat *commits, GHashTable *table, int start, int end)
{
    for (int u = start; u < end; u++)
    {
        int *id = xmalloc(sizeof(int));
        *id = get_comitter_id(commits->commits[u]);

        int *old_key; int *old_value;

        if (g_hash_table_lookup_extended(table, (gpointer)id, (gpointer)&old_key, (gpointer)&old_value))
        {
            int *new_value = xmalloc(sizeof(int)); 
            *new_value = (*old_value) + 1;

            g_hash_table_insert(table, (gpointer)id, (gpointer)new_value);

        }
        else
        {
            int *starter = xmalloc(sizeof(int)); *starter = 1;
            g_hash_table_insert(table, (gpointer)id, (gpointer)starter);
        }  
    }
}

void make_table_6(commit_cat *commits, repo_cat *repos, GHashTable *table, char *language)
{
    int size = commits->size;
    for (int u = 0; u < size; u++)
    {
        int *id = xmalloc(sizeof(int));
        *id = get_comitter_id(commits->commits[u]);

        int repo_id = get_repo_id(commits->commits[u]);
        char *lang  = get_language_from_id(repos, repo_id);

        if (strcmp(lang, language) == 0)
        {
            int *old_key; int *old_value;

            if (g_hash_table_lookup_extended(table, (gpointer)id, (gpointer)&old_key, (gpointer)&old_value))
            {
                int *new_value = xmalloc(sizeof(int)); 
                *new_value = (*old_value) + 1;

                g_hash_table_insert(table, (gpointer)id, (gpointer)new_value);
            }
            else
            {
                int *starter = xmalloc(sizeof(int)); *starter = 1;
                g_hash_table_insert(table, (gpointer)id, (gpointer)starter);
            }  
        }
    }
}

int get_commit_array_size(commit_cat *commits)
{
    return commits->size;
} 

void make_table_7(commit_cat *commits, repo_cat *repos, GHashTable *table, int start)
{
    for (int u = 0; u < start; u++)
    {
        int *id = xmalloc(sizeof(int));
        *id = get_repo_id(commits->commits[u]);

        char *desc = get_description_from_id(repos, *id);
        g_hash_table_insert(table, (gpointer)id, (gpointer)desc); 
    }
}

void make_table_9(commit_cat *commits, repo_cat *repos, user_cat *users, GHashTable *table)
{

    /*
        1. Iterar sob o array dos commits.
        2. Verificar se o owner do repo é amigo do commiter_id pelo repo_id. 
            + => função que retorna owner_id pelo repo_id
            + se é amigo adicionamos à hash table caso já não exista
            + se é amigo e já existir, incrementamos o count
            + se não for amigo damos skip.

        hash_table<user_id, ncommits>
    */

    for (int u = 0; u < commits->size; u++)
    {
        int *id = xmalloc(sizeof(int));
        *id = get_comitter_id(commits->commits[u]);

        int repo_id = get_repo_id(commits->commits[u]);
        int owner_id = get_owner_id_by_repo(repos, repo_id);

        int *old_key; int *old_value;

        if (is_friend(users, *id, owner_id))
        {
            if (g_hash_table_lookup_extended(table, (gpointer)id, (gpointer)&old_key, (gpointer)&old_value))
            {
                int *new_value = xmalloc(sizeof(int)); 
                *new_value = (*old_value) + 1;

                g_hash_table_insert(table, (gpointer)id, (gpointer)new_value);

            }
            else
            {
                int *starter = xmalloc(sizeof(int)); *starter = 1;
                g_hash_table_insert(table, (gpointer)id, (gpointer)starter);
            }  
        }
    }
   
}

void make_table_10(commit_cat *commits, repo_cat *repos, GHashTable *table)
{
    for (int u = 0; u < commits->size; u++)
    {
        int *repo_id = xmalloc(sizeof(int));
        *repo_id = get_repo_id(commits->commits[u]);

        int committer_id = get_comitter_id(commits->commits[u]);

        int *old_key; commit_array *old_value;

        if (g_hash_table_lookup_extended(table, (gpointer)repo_id, (gpointer)&old_key, (gpointer)&old_value))
        {
            old_value->array[old_value->size] = xmalloc(sizeof(set_v3));
            old_value->array[old_value->size]->committer_id = committer_id;
            old_value->array[old_value->size]->message_size = strlen(get_message(commits->commits[u]));
            old_value->size++;
        }
        else
        {
            commit_array *array = xmalloc(sizeof(commit_array));

            array->array = xmalloc(sizeof(set_v3) * 256);
            array->size = 0;
            array->array[array->size] = xmalloc(sizeof(set_v3));

            array->array[array->size]->committer_id = committer_id;
            array->array[array->size]->message_size = strlen(get_message(commits->commits[u]));

            array->size++;

            g_hash_table_insert(table, (gpointer)repo_id, (gpointer)array);
        }  
    }

    commit_array *val;
    GHashTableIter iter;
    int *key_;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        qsort(val->array, val->size, sizeof(set_v3*), compare_sets_v3);
    }
}