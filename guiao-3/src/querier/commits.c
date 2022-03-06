#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/querier/utils.h" 
#include "../include/querier/commits.h"

typedef struct commit_registry
{
    char* message; /**< string that represents a commit message. */

    datetime* commit_at; /**< struct that represents the datetime on which the commit was made. */
    
    long repo_id; /**< long int that represents the repo id. */
    long committer_id; /**< long int that represents the committer id. */
    long author_id; /**< long int that represents the author id. */

} commit_reg;

/* GETTERS */

char *get_message(commit_reg *commit)
{
    return strdup(commit->message);
}

void get_commit_at(commit_reg *commit, datetime *new_date)
{
    datetimedup(new_date, commit->commit_at);
}

int get_repo_id(commit_reg *commit)
{
    return commit->repo_id;
}

int get_comitter_id(commit_reg *commit)
{
    return commit->committer_id;
}

int get_author_id(commit_reg *commit)
{
    return commit->author_id;
}

size_t get_commit_size()
{
    return sizeof(commit_reg);
}

/* SETTERS */

void set_message(commit_reg *commit, char *message)
{
    commit->message = strdup(message);
}

void set_commit_at(commit_reg *commit, datetime *date)
{
    commit->commit_at = xmalloc(sizeof(datetime));
    datetimedup(commit->commit_at, date);
}

void set_repo_id(commit_reg *commit, int repo_id)
{
    commit->repo_id = repo_id;
}

void set_comitter_id(commit_reg *commit, int comitter_id)
{
    commit->committer_id = comitter_id;
}

void set_author_id(commit_reg *commit, int author_id)
{
    commit->author_id = author_id;
}

/* PRINT */

void dump_commit(commit_reg *commit)
{
    printf("Comitter id: %ld\n", commit->committer_id);
    printf("Repo id: %ld\n", commit->repo_id);
    printf("Author id: %ld\n", commit->author_id);
    printf("Commited at: %s\n", commit->commit_at->str_repr);
    printf("Message: %s\n\n", commit->message);
}