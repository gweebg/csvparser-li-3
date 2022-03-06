#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/querier/utils.h" // Contains datetime and type structs.
#include "../include/querier/repos.h" // Header file.

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

/* GETTERS */

char *get_license(repo_reg *repo)
{
    return strdup(repo->license);
}

char *get_description(repo_reg *repo)
{
    return strdup(repo->description);
}

char *get_language(repo_reg *repo)
{
    return strdup(repo->language);
}

char *get_full_name(repo_reg *repo)
{
    return strdup(repo->full_name);
}

char *get_default_branch(repo_reg *repo)
{
    return strdup(repo->default_branch);
}

void get_created_at_repo(repo_reg *repo, datetime *new_date)
{
    datetimedup(new_date, repo->created_at);
}

void get_updated_at(repo_reg *repo, datetime *new_date)
{
    datetimedup(new_date, repo->updated_at);
}

long get_forks_count(repo_reg *repo)
{
    return repo->forks_count;
}

long get_open_issues(repo_reg *repo)
{
    return repo->open_issues;
}

long get_stargazers_count(repo_reg *repo)
{
    return repo->stargazers_count;
}

long get_owner_id(repo_reg *repo)
{
    return repo->owner_id;
}

long get_id_repo(repo_reg *repo)
{
    return repo->id;
}

long get_size(repo_reg *repo)
{
    return repo->size;
}

bool get_wiki(repo_reg *repo)
{
    return repo->has_wiki;
}

size_t get_repo_size()
{
    return sizeof(repo_reg);
}

/* SETTERS */

void set_id_repo(repo_reg *repo, long id)
{
    repo->id = id;
}

void set_owner_id(repo_reg *repo, long owner_id)
{
    repo->owner_id = owner_id;
}

void set_full_name(repo_reg *repo, char *full_name)
{
    repo->full_name = strdup(full_name);
}

void set_license(repo_reg *repo, char *license)
{
    repo->license = strdup(license);
}

void set_wiki(repo_reg *repo, bool wiki)
{
    repo->has_wiki = wiki;
}

void set_description(repo_reg *repo, char *description)
{
    repo->description = strdup(description);
}

void set_languages(repo_reg *repo, char* language)
{
    repo->language = strdup(language);
}

void set_default_branch(repo_reg *repo, char* default_branch)
{
    repo->default_branch = strdup(default_branch);
}

void set_created_at_repo(repo_reg *repo, datetime *date)
{
    repo->created_at = malloc(sizeof(datetime));
    datetimedup(repo->created_at, date);
}

void set_updated_at(repo_reg *repo, datetime *date)
{
    repo->updated_at = malloc(sizeof(datetime));
    datetimedup(repo->updated_at, date);
}

void set_forks_count(repo_reg *repo, long forks_count)
{
    repo->forks_count = forks_count;
}

void set_open_issues(repo_reg *repo, long open_issues)
{
    repo->open_issues = open_issues;
}

void set_stargazers(repo_reg *repo, long stargazers)
{
    repo->stargazers_count = stargazers;
}

void set_size(repo_reg *repo, long size)
{
    repo->size = size;
}

