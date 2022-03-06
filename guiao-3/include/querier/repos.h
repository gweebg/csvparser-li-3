#include <stdbool.h>
#include "utils.h"

#ifndef REPOS_H
#define REPOS_H

typedef struct repo_registry repo_reg;

/* GETTERS */

char *get_license(repo_reg *repo);
char *get_description(repo_reg *repo);
char *get_language(repo_reg *repo);
char *get_full_name(repo_reg *repo);
char *get_default_branch(repo_reg *repo);
void get_created_at_repo(repo_reg *repo, datetime *new_date);
void get_updated_at(repo_reg *repo, datetime *new_date);
long get_forks_count(repo_reg *repo);
long get_open_issues(repo_reg *repo);
long get_stargazers_count(repo_reg *repo);
long get_owner_id(repo_reg *repo);
long get_id_repo(repo_reg *repo);
long get_size(repo_reg *repo);
bool get_wiki(repo_reg *repo);
size_t get_repo_size();

/* SETTERS */

void set_id_repo(repo_reg *repo, long id);
void set_owner_id(repo_reg *repo, long owner_id);
void set_full_name(repo_reg *repo, char *full_name);
void set_license(repo_reg *repo, char *license);
void set_wiki(repo_reg *repo, bool wiki);
void set_description(repo_reg *repo, char *desciption);
void set_languages(repo_reg *repo, char* language);
void set_default_branch(repo_reg *repo, char* default_branch);
void set_created_at_repo(repo_reg *repo, datetime *date);
void set_updated_at(repo_reg *repo, datetime *date);
void set_forks_count(repo_reg *repo, long forks_count);
void set_open_issues(repo_reg *repo, long open_issues);
void set_stargazers(repo_reg *repo, long stargazers);
void set_size(repo_reg *repo, long size);

#endif