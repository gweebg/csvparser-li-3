#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>

#include "../../include/builder/clean_commits.h"
#include "../../include/builder/clean_users.h"

int get_third_number(char* string)
{
    strsep(&string, ";"); strsep(&string, ";");
    char* num_string = strdup(strsep(&string, ";"));
    return atoi(num_string);
}

GHashTable *clean_commits(char const *filename, GHashTable *user_table, GHashTable *repo_table, int *storage, int *to_ignore, int *size)
{
    FILE *commits_file = fopen(filename, "r");

    if (!commits_file)
    {
        fprintf(stderr, "Could not open file.\n");
        return NULL;
    }

    char* buffer = (char*)malloc(sizeof(char) * COMMIT_SIZE);

    if (!buffer)
    {
        fprintf(stderr, "Out of memory.\n");
        return NULL;
    }

    GHashTable *commits_table = g_hash_table_new(g_int_hash, g_int_equal);
    // GTree *commits_tree = g_tree_new(g_int_equal);

    int *repo_id = (int*)malloc(sizeof(int));
    int *author_id = (int*)malloc(sizeof(int));
    int *commiter_id = (int*)malloc(sizeof(int));

    if (!repo_id || !author_id || !commiter_id)
    {
        fprintf(stderr, "Out of memory.\n");
        return NULL;
    }

    int i = 0;
    int c = 0;
    int line = 1;

    fgets(buffer, COMMIT_SIZE, commits_file);
    while(fgets(buffer, COMMIT_SIZE, commits_file))
    {
        *repo_id = get_number(g_strdup(buffer));
        *author_id   = get_second_number(g_strdup(buffer));
        *commiter_id = get_third_number(g_strdup(buffer));

        if (g_hash_table_lookup(user_table, author_id) && g_hash_table_lookup(user_table, commiter_id) && g_hash_table_lookup(repo_table, repo_id))
        {
            storage[i] = *repo_id;
            g_hash_table_add(commits_table, &storage[i]);

            i++;
        }
        else { to_ignore[c] = line; c++; }

        *repo_id = get_number(g_strdup(buffer));
        
        line++;
    }

    *size = c;
    
    free(buffer); free(repo_id); free(author_id); free(commiter_id);
    fclose(commits_file);

    return commits_table;
    // return commits_tree;
}
