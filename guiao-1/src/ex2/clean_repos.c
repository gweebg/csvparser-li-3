#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>

#include "../includes/ex2/clean_up.h"
#include "../includes/ex2/clean_users.h"
#include "../includes/ex2/clean_repos.h"

/*
Returns a hashtable containing the id parameter of the csv file.
*/
GHashTable *id_to_hashset(char* filename, int* storage)
{
    printf("> (id_to_hashset)\n");

    FILE *repo_file = fopen(filename, "r");

    printf("    > Opening files...");

    if (!repo_file)
    {
        fprintf(stderr, "   > Could not open file.\n");
        return NULL;
    }

    printf("Done!\n");

    char* buffer = (char*)malloc(sizeof(char) * REPO_SIZE);

    if (!buffer)
    {
        fprintf(stderr, "   > Out of memory.\n");
        return NULL;
    }

    printf("    > Creating hash table...");

    GHashTable *repos_table = g_hash_table_new(g_int_hash, g_int_equal);

    int *id = (int*)malloc(sizeof(int));
    int i = 0;

    fgets(buffer, REPO_SIZE, repo_file);
    while(fgets(buffer, REPO_SIZE, repo_file))
    {
        *id = get_number(buffer);

        storage[i] = *id;
        g_hash_table_add(repos_table, &storage[i]);

        i++;
    }

    printf("Done!\n\n");

    free(buffer); free(id);
    fclose(repo_file);

    return repos_table;
}

bool clean_repos(char* filename, GHashTable *user_table, GHashTable *commit_table, int *to_ignore, int *to_ignore_size)
{
    printf("> (clean_repos)\n");

    FILE *repo_file = fopen(filename, "r");

    printf("    > Opening file...");

    if (!repo_file)
    {
        fprintf(stderr, "   > Could not open file.\n");
        return false;
    }

    printf("Done!\n");

    char* buffer = (char*)malloc(sizeof(char) * REPO_SIZE);

    if (!buffer)
    {
        fprintf(stderr, "   > Out of memory.\n");
        return false;
    }

    int *id = malloc(sizeof(int));
    int *owner_id = malloc(sizeof(int));

    if (!id || !owner_id)
    {
        fprintf(stderr, "    > Out of memory.\n");
        return NULL;
    }

    int i = 0; 
    int line = 1;
    
    printf("    > Reading data...");

    fgets(buffer, REPO_SIZE, repo_file);
    while(fgets(buffer, REPO_SIZE, repo_file))
    {
        *id = get_number(g_strdup(buffer));
        *owner_id = get_second_number(g_strdup(buffer));

        if (!(g_hash_table_lookup(user_table, owner_id) && g_hash_table_lookup(commit_table, id)))
        {
            to_ignore[i] = line; 
            i++;
        }

        line++;
    }

    *to_ignore_size = i;

    printf("Done!\n");
    free(buffer); free(id); free(owner_id); 

    return true;
}
