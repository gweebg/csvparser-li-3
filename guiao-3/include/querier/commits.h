#include "utils.h"

#ifndef COMMITS_H
#define COMMITS_H

typedef struct commit_registry commit_reg;

/* GETTERS */

char *get_message(commit_reg *commit);
void get_commit_at(commit_reg *commit, datetime *new_date);
int get_repo_id(commit_reg *commit);
int get_comitter_id(commit_reg *commit);
int get_author_id(commit_reg *commit);
size_t get_commit_size();

/* SETTERS */

void set_message(commit_reg *commit, char *message);
void set_commit_at(commit_reg *commit, datetime *date);
void set_repo_id(commit_reg *commit, int repo_id);
void set_comitter_id(commit_reg *commit, int comitter_id);
void set_author_id(commit_reg *commit, int author_id);

/* PRINT */

void dump_commit(commit_reg *commit);

#endif