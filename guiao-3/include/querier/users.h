#include "utils.h"

#ifndef USER_H
#define USER_H

typedef struct user_registry user_reg;

/* GETTERS */

void get_created_at(user_reg *user, datetime *new_date);
void get_follower_list(user_reg *user, int *dest, int size);
void get_following_list(user_reg *user, int *dest, int size);
void free_user(user_reg *user);

type get_type(user_reg *user);
char *get_username(user_reg *user);

int get_public_repo(user_reg *user);
int get_followers(user_reg *user);
int get_following(user_reg *user);
int get_gists(user_reg *user);
int get_id(user_reg *user);

size_t get_user_size();

/* SETTERS */

void set_username(user_reg *user, char *username);
void set_type(user_reg *user, type t);
void set_created_at(user_reg *user, datetime *date);
void set_public_repo(user_reg *user, int public_repo);
void set_followers(user_reg *user, int followers);
void set_following(user_reg *user, int following);
void set_gists(user_reg *user, int gists);
void set_id(user_reg *user, int id);
void set_follower_list(user_reg *user, int *list);
void set_following_list(user_reg *user, int *list);
void dump_user(user_reg *user);

#endif