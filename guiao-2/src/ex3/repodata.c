#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <glib.h>
#include <ctype.h>

#include "../include/ex3/repodata.h"
#include "../include/ex3/database_tools.h"
#include "../include/ex3/database.h"
#include "../include/ex3/utils.h"

typedef struct repo_registry
{

    char* license; /**< string that represents the repo license (if it exists). */
    char* description; /**< string that represents the repo description. */
    char* language; /**< string that represents the repo language. */
    char* full_name; /**< string that represents the repo full name. */
    char* default_branch; /**< string that represents the repo default branch. */

    datetime *created_at; /**< struct that represents the datetime on which the repo was created. */
    datetime *updated_at; /**< struct that represents the datetime on which the repo was updated. */

    long forks_count; /**< long int that represents the number of the repo forks. */
    long open_issues; /**< long int that represents the number of the repo open issues. */
    long stargazers_count; /**< long int that represents the number of the repo stargazers. */
    long owner_id; /**< long int that represents the id of the repo owner. */
    long id; /**< long int that represents the repo id. */
    long size; /**< long int that represents the repo size. */
    
    bool has_wiki; /**< bool to check if the repo has wiki or not. */

} repo_reg;

typedef struct repo_catalog
{
    repo_reg **repos_storage;
    int repos_storage_size;

    repo_reg **repos_dates;
    int repos_dates_size;

} repo_cat;

void create_repo(char* csv_line, repo_reg *new_repo)
{
    // id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size

    // new_repo->created_at = malloc(sizeof(datetime*));
    // new_repo->updated_at = malloc(sizeof(datetime*));

    csv_to_number(&csv_line, &(new_repo->id));
    csv_to_number(&csv_line, &(new_repo->owner_id));
    csv_to_string(&csv_line, &(new_repo->full_name));
    csv_to_string(&csv_line, &(new_repo->license));
    csv_check_wiki(&csv_line, &(new_repo->has_wiki));
    csv_to_string(&csv_line, &(new_repo->description));
    csv_to_string(&csv_line, &(new_repo->language));
    csv_to_string(&csv_line, &(new_repo->default_branch));
    csv_to_datetime(&csv_line, &(new_repo->created_at));
    csv_to_datetime(&csv_line, &(new_repo->updated_at));
    csv_to_number(&csv_line, &(new_repo->forks_count));
    csv_to_number(&csv_line, &(new_repo->open_issues));
    new_repo->stargazers_count = atoi(csv_line);

}

void dump_repo(repo_reg *repo)
{
    // id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size

    printf("===USER================\n");
    printf("Id : %ld\n",           repo->id);
    printf("Owner : %ld\n",        repo->owner_id);
    printf("Name : %s\n",          repo->full_name);
    printf("Wiki : %d\n",          repo->has_wiki);
    printf("Description : %s\n",   repo->description);
    printf("Language : %s\n",      repo->language);
    printf("Default Branch : %s\n",repo->default_branch);
    printf("Created at : %s\n",    repo->created_at->str_repr);
    printf("Updated at : %s\n",    repo->updated_at->str_repr);
    printf("Forks : %ld\n",         repo->forks_count);
    printf("Open Issues : %ld\n",   repo->open_issues);
    printf("Stargazers: %ld\n",     repo->stargazers_count);
    printf("Size: %ld\n",           repo->size);
    printf("=======================\n");
}

bool build_repos(repo_cat* repositories, const char *filename)
{
    printf("(build_repos)\n");
    printf("    > Opening file %s... ", filename);

    int counter = 0;

    FILE *rfile = fopen(filename, "r");
    if (!rfile)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    printf("Done!\n");
    printf("    > Reading %s... ", filename);

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, rfile); // Skips the header line.
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, rfile);     

    while (line_size >= 0)
    {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Remove o \n ou \r\n

        repo_reg *repo = (repo_reg*)malloc(sizeof(*repo) * 6);
        create_repo(line_buffer, repo);

        repositories->repos_storage[counter] = repo;
        repositories->repos_dates[counter] = repo;

        counter++;
        line_size = getline(&line_buffer, &line_buffer_size, rfile);
    }

    repositories->repos_storage_size = counter;
    repositories->repos_dates_size = counter;

    printf("Done!\n");
    printf("    > Data stored successfully!\n");

    fclose(rfile);
    if (line_buffer) free(line_buffer);

    return true;
}

int compare_repos(const void* a, const void* b) 
{
    repo_reg *repo_a = *(repo_reg**)a;
    repo_reg *repo_b = *(repo_reg**)b;

    if (repo_a->id < repo_b->id) return -1;
    else if (repo_a->id > repo_b->id) return 1;
    else return 0;
}

int compare_created_at(const void *x, const void *y)
{
    repo_reg *a = *(repo_reg**)x;
    repo_reg *b = *(repo_reg**)y;

    int r = compare_datetimes(a->created_at, b->created_at);
    return r;
}

bool build_repos_catalog(repo_cat *new_catalog, stats *statistics, const char *filename)
{
    /* ==STRUCT============================================================================= */

    /*
        repo_cat *new_catalog
                | repo_reg *repos_storage
                | GHashTable* repos
    */

    /* ==INITIALIZATION============================================================================= */

    int repo_count = get_lines_from_file(filename);
    update_total_repos(statistics, repo_count - 1);

    new_catalog->repos_storage = malloc(sizeof(repo_reg) * repo_count);
    if (new_catalog->repos_storage == NULL)
    {
        fprintf(stderr, "Could not allocate memory for 'repos_storage' in 'build_repos_catalog' @ repodata.c\n");
        exit(EXIT_FAILURE);
    }

    new_catalog->repos_dates = malloc(sizeof(repo_reg) * repo_count);
    if (new_catalog->repos_dates == NULL)
    {
        fprintf(stderr, "Could not allocate memory for 'repos_dates' in 'build_repos_catalog' @ repodata.c\n");
        exit(EXIT_FAILURE);
    }

    /* ==BUILDING=================================================================================== */

    build_repos(new_catalog, filename); /* <-- Popular o array e struct stats */

    printf("    > Sorting the repositories array... ");

    qsort(new_catalog->repos_storage,
          new_catalog->repos_storage_size,
          sizeof(repo_reg*),
          &compare_repos);

    printf("Done!\n");

    printf("    > Sorting the created_at dates array... ");

    qsort(new_catalog->repos_dates, 
          new_catalog->repos_dates_size,
          sizeof(datetime*),
          compare_created_at);

    printf("Done!\n");

    return true;    
}

int repo_bin_search(repo_reg **repo_array, int start, int end, int key)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;

        if (repo_array[mid]->id == key) return mid;

        if (repo_array[mid]->id > key) return repo_bin_search(repo_array, start, mid - 1, key);
        else return repo_bin_search(repo_array, mid + 1, end, key);
    }

    return -1;
}

int future_date_index(repo_cat *repos, char *date_string)
{
    int year = 0, month = 0, day = 0;

    datetime *date = malloc(sizeof(*date));
    date->hours = 0; date->minutes = 0; date->seconds = 0;

    sscanf(date_string, "%4d-%2d-%2d", &year, &month, &day);
    date->year = year; date->month = month; date->day = day;

    int index = -1, l = 0;
    int size = repos->repos_dates_size;

    while (l <= size)
    {
        int mid = (l + size) / 2;
        int comp = compare_datetimes(date, repos->repos_dates[mid]->created_at);

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
        else
        {
            l = mid + 1;
        }
    }

    return index;    
}

void destroy_repo_catalog(repo_cat *repos)
{
    for (int i = 0; i < repos->repos_dates_size; i++) 
    {
        free(repos->repos_dates[i]->language);
        free(repos->repos_dates[i]->description);
        free(repos->repos_dates[i]->full_name);
        free(repos->repos_dates[i]->license);
        free(repos->repos_dates[i]);
    }
    free(repos->repos_dates);

    for (int i = 0; i < repos->repos_storage_size; i++) 
    {
        free(repos->repos_storage[i]->language);
        free(repos->repos_storage[i]->description);
        free(repos->repos_storage[i]->full_name);
        free(repos->repos_storage[i]->license);
        free(repos->repos_storage[i]);
    }
    free(repos->repos_storage);
}

int get_repo_size()
{
    return (int)sizeof(repo_reg);
}

int get_repos_cat_size()
{
    return (int)sizeof(repo_cat);
}

char* get_repo_language(repo_cat *repos, long repo_id)
{
    int index = repo_bin_search(repos->repos_storage, 0, repos->repos_storage_size, repo_id);
    if (index != -1) return strdup(repos->repos_storage[index]->language);
    else return NULL;
}

char* get_updated_at(repo_cat *repos, int index)
{
    return strdup(repos->repos_storage[index]->updated_at->str_repr);
}

int get_repo_array_size(repo_cat *repos)
{
    return repos->repos_storage_size;
}

long get_repo_id_by_index(repo_cat *repos, int index)
{
    return repos->repos_storage[index]->id;
}

char* get_repo_description(repo_cat *repos, int index)
{
    return strdup(repos->repos_storage[index]->description);
}

char* get_repo_lang_by_index(repo_cat *repos, int index)
{
    return strdup(repos->repos_dates[index]->language);
}

int get_owner_by_id(repo_cat *repos, long repo_id)
{
    int index = repo_bin_search(repos->repos_storage, 0, repos->repos_storage_size, repo_id);
    if (index != -1) return repos->repos_storage[index]->owner_id;
    else return -1;
}