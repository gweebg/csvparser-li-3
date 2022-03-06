#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>

#include "../../include/builder/clean_up.h"
#include "../../include/builder/clean_users.h"
#include "../../include/builder/clean_commits.h"
#include "../../include/builder/clean_repos.h"

#include "../../include/builder/fetch_tools.h"
#include "../../include/ui_helpers.h"

bool contains(int *array, int start, int end, int value)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;

        if (array[mid] == value) return true;
        if (array[mid] > value ) return contains(array, start, mid - 1, value);

        return contains(array, mid + 1, end, value);
    }

    return false;
}

bool write_changes(char* filename, char* pathname, int *to_ignore, int *size, int FILE_MAX_SIZE)
{
    FILE *file = fopen(filename, "r");
    FILE *output = fopen(pathname, "w");

    if (!file || !output) 
    {
        fprintf(stderr, "Could not open the file!\n");
        return false;
    }

    char* buffer = (char*)malloc(sizeof(char) * FILE_MAX_SIZE);

    if (!buffer)
    {
        fprintf(stderr, "Out of memory!\n");
        return false;
    }

    int current_line = 0;

    while(fgets(buffer, FILE_MAX_SIZE, file))
    {
        if (!contains(to_ignore, 0, *size, current_line)) fputs(buffer, output);
        current_line++;
    }

    fclose(file); fclose(output);
    free(buffer);

    return true;
}

bool clean_up()
{
    char* user_filename = "./saida/users-ok.csv";
    char* commits_filename = "./saida/commits-ok.csv";
    char* repos_filename = "./saida/repos-ok.csv";

    /* Storing the id from user.csv on a hash table. */
    int user_lines = get_lines_from_file(user_filename);
    int *user_storage = (int*)malloc(sizeof(int) * user_lines);

    GHashTable *user = clean_users(user_filename, user_storage);
    if (!user) return false;

    /* Getting a hash table containing the id stored in the repos.csv file. */
    int repo_lines = get_lines_from_file(repos_filename);
    int *repo_storage = (int*)malloc(sizeof(int) * repo_lines);

    GHashTable *id_from_repos = id_to_hashset(repos_filename, repo_storage);
    if(!id_from_repos) return false;

    /* Comparing author_id and committer_id from commits.csv with the ids from the hash table above. */
    int commits_lines = get_lines_from_file(commits_filename);

    int *commits_storage = (int*)malloc(sizeof(int) * commits_lines);
    int *commit_ignore = (int*)malloc(sizeof(int) * commits_lines);
    int *commit_ignore_size = (int*)malloc(sizeof(int));

    if (!commits_storage || !commit_ignore || !commit_ignore_size) return false;

    GHashTable *repo_ids = clean_commits(commits_filename, user, id_from_repos, 
                                         commits_storage, commit_ignore, commit_ignore_size);
    if (!repo_ids) return false;

    g_hash_table_destroy(id_from_repos); free(repo_storage);


    /* Comparing id(user.csv) with owner_id and id(repos.csv) with repo_id and adding to the list the invalid lines.*/
    int *repo_ignore = (int*)malloc(sizeof(int) * repo_lines);
    int *repo_ignore_size = (int*)malloc(sizeof(int));

    if (!repo_ignore || !repo_ignore_size) return false;

    bool repos = clean_repos(repos_filename, user, repo_ids, repo_ignore, repo_ignore_size);
    if(!repos) return false;

    g_hash_table_destroy(user); g_hash_table_destroy(repo_ids);
    free(commits_storage); free(user_storage);

    /* Writing the changes of the above functions. */
    bool write_c = write_changes(commits_filename, "./saida/commits-final.csv", commit_ignore, commit_ignore_size, COMMIT_SIZE);
    if (!write_c) return false;

    bool write_r = write_changes(repos_filename, "./saida/repos-final.csv", repo_ignore, repo_ignore_size, REPO_SIZE);
    if (!write_r) return false;

    printf(GRN "✔\n" RESET);
    return true;
}







