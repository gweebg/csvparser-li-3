#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <glib.h>

#include "../include/ex3/database_tools.h"
#include "../include/ex3/commitdata.h"
#include "../include/ex3/statistics.h"
#include "../include/ex3/userdata.h"

#include "../include/ex3/utils.h"

/*
    Queries 1 a 4 : apenas precisam de um array commiter_ids ordenado ou outra
                    data struct qualquer cuja procura e o insert seja rápido.
                    Vamos usar um array ordenado para guardar os commiter_id.

*/

typedef struct commit_registry
{
    char* message; /**< string that represents a commit message. */

    datetime* commit_at; /**< struct that represents the datetime on which the commit was made. */
    
    long repo_id; /**< long int that represents the repo id. */
    long committer_id; /**< long int that represents the committer id. */
    long author_id; /**< long int that represents the author id. */

} commit_reg;

typedef struct commit_catalog
{

    commit_reg **commits;
    int *committer_id_array;
    int *author_id_array;

    int committer_id_array_size;

} commit_cat;

void create_commit(char* csv_line, commit_reg *new_commit)
{
    //repo_id;author_id;committer_id;commit_at;message
    char* empty_string = "\0";

    csv_to_number(&csv_line, &(new_commit->repo_id));
    csv_to_number(&csv_line, &(new_commit->author_id));
    csv_to_number(&csv_line, &(new_commit->committer_id));
    csv_to_datetime(&csv_line, &(new_commit->commit_at));

    if (csv_line[0] != '\0')
        csv_to_string(&csv_line, &(new_commit->message));
    else
    {
        new_commit->message = malloc(sizeof(char) * 2);
        strncpy(new_commit->message, empty_string, 2);
    }

}

bool build_commits(commit_cat *catalog, const char *filename)
{
    printf("(build_commits)\n");

    int counter = 0;

    printf("    > Opening file %s... ", filename);

    FILE *cfile = fopen(filename, "r");
    if (!cfile)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    printf("Done!\n");
    printf("    > Reading %s... ", filename);

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, cfile); // Skips the header line.
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, cfile);     

    while (line_size >= 0)
    {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Remove o \n ou \r\n
        commit_reg *commit = (commit_reg*)malloc(sizeof(*commit));
        create_commit(line_buffer, commit);

        catalog->commits[counter] = commit;
        catalog->committer_id_array[counter] = commit->committer_id;
        catalog->author_id_array[counter] = commit->author_id;
        counter++;

        line_size = getline(&line_buffer, &line_buffer_size, cfile);
    }

    catalog->committer_id_array_size = counter;

    printf("Done!\n");
    printf("    > Data stored successfully!\n");

    fclose(cfile);
    if (line_buffer) free(line_buffer);

    return true;
}

void dump_commit(commit_reg *commit)
{
    printf("===COMMIT================\n");
    printf("Repo ID : %ld\n", commit->repo_id);
    printf("Comitter ID : %ld\n", commit->committer_id);
    printf("Author ID : %ld\n", commit->author_id);
    printf("Commited at : %s\n", commit->commit_at->str_repr);
    printf("Message : %s\n", commit->message);
    printf("=======================\n");
}

int compare(const void *x, const void *y) 
{
    int f = *((int*)x);
    int s = *((int*)y);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int compare_with_dates(const void *x, const void *y)
{
    commit_reg *a = *(commit_reg**)x;
    commit_reg *b = *(commit_reg**)y;

    int r = compare_datetimes(a->commit_at, b->commit_at);
    return r;
}

int bound_date_index(commit_cat *commits, char *date_string)
{
    int year = 0, month = 0, day = 0;

    datetime *date = malloc(sizeof(*date));
    date->hours = 0; date->minutes = 0; date->seconds = 0;

    sscanf(date_string, "%4d-%2d-%2d", &year, &month, &day);
    date->year = year; date->month = month; date->day = day;

    int index = -1, l = 0;
    int size = commits->committer_id_array_size;

    while (l <= size)
    {
        int mid = (l + size) / 2;
        int comp = compare_datetimes(date, commits->commits[mid]->commit_at);

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

bool build_commits_catalog(commit_cat *new_catalog, stats *statistics, const char *filename)
{
    /* ==STRUCT============================================================================= */

    /*
        commit_cat *new_catalog
                    | user_reg *committer_id_array
                    | int committer_id_array_size
    */

    /* ==INITIALIZATION============================================================================= */

    int commit_count = get_lines_from_file(filename);
    update_total_commits(statistics, commit_count - 1);

    new_catalog->committer_id_array = malloc(sizeof(commit_reg) * commit_count);
    if (new_catalog->committer_id_array == NULL)
    {
        fprintf(stderr, "Could not allocate memory for 'committer_id_array' in 'build_commit_catalog' @ commitdata.c\n");
        exit(EXIT_FAILURE);
    }

    new_catalog->author_id_array = malloc(sizeof(commit_reg) * commit_count);
    if (new_catalog->author_id_array == NULL)
    {
        fprintf(stderr, "Could not allocate memory for 'author_id_array' in 'build_commit_catalog' @ commitdata.c\n");
        exit(EXIT_FAILURE);
    }

    new_catalog->commits = malloc(sizeof(commit_reg) * commit_count);
    if (new_catalog->commits == NULL)
    {
        fprintf(stderr, "Could not allocate memory for 'commits' in 'build_commit_catalog' @ commitdata.c\n");
        exit(EXIT_FAILURE);
    }

    /* ==BUILDING=================================================================================== */

    build_commits(new_catalog, filename); /* <-- Popular o array e struct stats */

    printf("    > Sorting ids array... ");

    qsort(new_catalog->committer_id_array, 
          new_catalog->committer_id_array_size,
          sizeof(int),
          compare);

    qsort(new_catalog->author_id_array, 
          new_catalog->committer_id_array_size,
          sizeof(int),
          compare);          

    printf("Done!\n");

    printf("    > Sorting commits array... ");

    qsort(new_catalog->commits, 
          new_catalog->committer_id_array_size,
          sizeof(datetime*),
          compare_with_dates);

    printf("Done!\n\n");

    // for (int i = 0; i < new_catalog->committer_id_array_size; i++)
        // printf("%s\n", new_catalog->commits[i]->commit_at->str_repr);
        // dump_commit(new_catalog->commits[i]);

    return true;
}

// int search(int *array, int start, int end, int value)
// {
//     if (end >= start)
//     {
//         int mid = start + (end - start) / 2;

//         if (array[mid] == value) return mid;
//         if (array[mid] > value) return search(array, start, mid - 1, value);
        
//         return search(array, mid + 1, end, value);
//     }

//     return -1;
// }

bool is_collaborator(long id, commit_cat *commits)
{
    int s1 = search(commits->committer_id_array, 0, commits->committer_id_array_size, (int)id);
    int s2 = search(commits->author_id_array, 0, commits->committer_id_array_size, (int)id);
    if (s1 == -1 && s2 == -1) return false;
    else return true;
}

int* get_committer_ids(commit_cat *commits)
{
    return commits->committer_id_array;
}

int get_committer_ids_size(commit_cat *commits)
{
    return commits->committer_id_array_size;
}

int get_commit_size()
{
    return (int)sizeof(commit_reg);
}

int get_commit_cat_size()
{
    return (int)sizeof(commit_cat);
}

long get_committer_id(commit_cat *commits, int index)
{
    return commits->commits[index]->committer_id;
}

long get_repo_id(commit_cat *commits, int index)
{
    return commits->commits[index]->repo_id;
}

char *get_string_date(commit_cat *commits, int index)
{
    return strdup(commits->commits[index]->commit_at->str_repr);
}

void commitdup(commit_reg *dest, commit_reg *source)
{
    dest->commit_at = malloc(sizeof(datetime));
    datetimedup(dest->commit_at, source->commit_at);

    dest->author_id = source->author_id;
    dest->committer_id = source->committer_id;
    dest->message = strdup(source->message);
    dest->repo_id = source->repo_id;
}

commit_reg* get_commit(commit_cat *commits, int index)
{
    return commits->commits[index]; // commitdup.
}

int get_message_size (commit_cat *commits, int index)   
{
    return strlen(commits->commits[index]->message);
}