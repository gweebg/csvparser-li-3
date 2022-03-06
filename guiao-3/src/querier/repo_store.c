#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../include/querier/storing_tools.h"
#include "../include/querier/repo_store.h"
#include "../include/querier/statistics.h"
#include "../include/querier/utils.h"
#include "../include/querier/repos.h"

typedef struct repo_catalog
{
    GHashTable *repos;
    repo_reg **repos_date_sorted;
    int size;

} repo_cat;

void create_repo(char* csv_line, repo_reg *new_repo)
{
    // id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size

    long id, owner_id, forks_count, open_issues, stargazers_count, size;
    char *license, *description, *language, *default_branch, *full_name;
    datetime *created_at, *updated_at;
    bool wiki;

    csv_to_number(&csv_line, &id);
    csv_to_number(&csv_line, &owner_id);
    csv_to_string(&csv_line, &full_name);
    csv_to_string(&csv_line, &license);
    csv_check_wiki(&csv_line, &wiki);
    csv_to_string(&csv_line, &description);
    csv_to_string(&csv_line, &language);
    csv_to_string(&csv_line, &default_branch);
    csv_to_datetime(&csv_line, &created_at);
    csv_to_datetime(&csv_line, &updated_at);
    csv_to_number(&csv_line, &forks_count);
    csv_to_number(&csv_line, &open_issues);
    csv_to_number(&csv_line, &stargazers_count);
    size = atoi(csv_line);

    set_id_repo(new_repo, id);
    set_owner_id(new_repo, owner_id);
    set_full_name(new_repo, full_name);
    set_license(new_repo, license);
    set_wiki(new_repo, wiki);
    set_description(new_repo, description);
    set_languages(new_repo, language);
    set_default_branch(new_repo, default_branch);
    set_created_at_repo(new_repo, created_at);
    set_updated_at(new_repo, updated_at);
    set_forks_count(new_repo, forks_count);
    set_open_issues(new_repo, open_issues);
    set_size(new_repo, size);
    set_stargazers(new_repo, stargazers_count);
}

bool store_repos(repo_cat *repo_catalog, stats *statistics, const char *filename)
{
    // printf("(store_repos)\n");
    // printf("    > Opening file %s... ", filename);

    FILE *ufile = fopen(filename, "r");
    if (!ufile)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    int lines = count_lines(filename);

    // printf("Done!\n");
    // printf("    > Reading %s... ", filename);

    int total_repos = 0;

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, ufile);
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, ufile);     

    repo_catalog->repos = g_hash_table_new(g_int_hash, g_int_equal);
    repo_catalog->repos_date_sorted = xmalloc(get_repo_size() * (lines - 1));   

    while (line_size >= 0)
    {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0;

        size_t repo_size = get_repo_size();
        repo_reg *repo = g_malloc(repo_size + 2 * sizeof(datetime) + 5 * 1024 * sizeof(char));

        create_repo(line_buffer, repo);

        int *id = xmalloc(sizeof(int));
        *id = get_id_repo(repo);

        g_hash_table_insert(repo_catalog->repos, GINT_TO_POINTER(id), repo);
        repo_catalog->repos_date_sorted[total_repos] = repo;

        line_size = getline(&line_buffer, &line_buffer_size, ufile);
        total_repos++;
    }

    repo_catalog->size = total_repos;

    set_total_repos(statistics, total_repos);

    qsort(repo_catalog->repos_date_sorted, 
          total_repos,
          sizeof(datetime*),
          compare_date_repo);

    // printf("Done!\n");
    // printf("    > Data stored successfully!\n");

    fclose(ufile);
    if (line_buffer) free(line_buffer);

    return true;
}

char *get_language_from_id(repo_cat *repos, int repo_id)
{
    int *id = xmalloc(sizeof(int));
    *id = repo_id;

    repo_reg *repo = g_hash_table_lookup(repos->repos, (gpointer)id);
    if (repo) return get_language(repo);
    else return "UNKNOWN";
}

char *get_description_from_id(repo_cat *repos, int repo_id)
{
    int *id = xmalloc(sizeof(int));
    *id = repo_id;

    repo_reg *repo = g_hash_table_lookup(repos->repos, (gpointer)id);
    if (repo) return get_description(repo);
    else return "UNKNOWN";
}

int get_owner_id_by_repo(repo_cat *repos, int repo_id)
{
    int *id = xmalloc(sizeof(int));
    *id = repo_id;

    repo_reg *repo = g_hash_table_lookup(repos->repos, (gpointer)id);
    if (repo) return get_owner_id(repo);
    else return -1;
}

void make_results_7(int *results, GHashTable *table, repo_cat *repos, int *final_size)
{
    repo_reg *val;
    GHashTableIter iter;
    int counter = 0, *key_, *current_id = xmalloc(sizeof(int));

    g_hash_table_iter_init(&iter, repos->repos);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        *current_id = get_id_repo(val);
        char *desc = g_hash_table_lookup(table, (gpointer)current_id);
        if (desc)
        {
            results[counter] = *current_id;
            counter++;
        }

        free(desc);
    }

    *final_size = counter;
    free(current_id);
}

int search_dates_repos(repo_cat *repos, datetime *date)
{
    int index = -1, l = 0;
    int size = repos->size;

    while (l <= size)
    {
        int mid = (l + size) / 2;

        datetime *date_comp = xmalloc(sizeof(datetime));
        get_created_at_repo(repos->repos_date_sorted[mid], date_comp);

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

void make_table_8(repo_cat *repos, GHashTable *table, int start)
{
    int size = repos->size;
    for (int u = start; u < size; u++)
    {
        char* language = get_language(repos->repos_date_sorted[u]);
        char *old_key; int *old_value;

        if (g_hash_table_lookup_extended(table, (gpointer)language, (gpointer)&old_key, (gpointer)&old_value))
        {
            int *new_value = xmalloc(sizeof(int)); 
            *new_value = (*old_value) + 1;

            g_hash_table_insert(table, (gpointer)language, (gpointer)new_value);

        }
        else
        {
            int *starter = xmalloc(sizeof(int)); *starter = 1;
            g_hash_table_insert(table, (gpointer)language, (gpointer)starter);
        }  
    }
}
