#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/querier/utils.h" // Contains datetime and type structs.
#include "../include/querier/users.h" // Header file.

typedef struct user_registry
{
    int* following_list; /**< array of integers that has other user ids that follow the original user. */
    int* follower_list; /**< array of integers that has other user ids whose are followed by the original user. */
    char* username; /**< string that represents the login of the user. */

    datetime* created_at; /**< struct that represents the datetime on which the user account was created. */
    type type; /**< enum that represents the User Type. */

    long public_repo, /**< long int that represents the number of public repos of a user. */
         followers, /**< long int that represents the number of followers of a user. */
         following, /**< long int that represents the number of following users of a user. */
         gists, /**< long int that represents the number of gists of a user. */
         id; /**< long int that represents the id number of a user. */

} user_reg;

/* GETTERS */

char* get_username(user_reg *user)
{
    return strdup(user->username);
}

type get_type(user_reg *user)
{
    return user->type;
}

void get_created_at(user_reg *user, datetime *new_date)
{
    datetimedup(new_date, user->created_at);
}

int get_public_repo(user_reg *user)
{
    return user->public_repo;
}

int get_followers(user_reg *user)
{
    return user->followers;
}

int get_following(user_reg *user)
{
    return user->following;
}

int get_gists(user_reg *user)
{
    return user->gists;
}

int get_id(user_reg *user)
{
    return user->id;
}

void get_follower_list(user_reg *user, int *dest, int size)
{
    for (int i = 0; i < size; i++)
        dest[i] = user->follower_list[i];
}

void get_following_list(user_reg *user, int *dest, int size)
{
    for (int i = 0; i < size; i++)
        dest[i] = user->following_list[i];
}

void free_user(user_reg *user)
{
    free_datetime(user->created_at);
    free(user->follower_list);
    free(user->follower_list);
    free(user->username);
    free(user);
}

size_t get_user_size()
{
    return sizeof(user_reg);
}

/* SETTERS */

void set_username(user_reg *user, char *username)
{
    user->username = strdup(username);
}

void set_type(user_reg *user, type t)
{
    user->type = t;
}

void set_created_at(user_reg *user, datetime *date)
{
    user->created_at = malloc(sizeof(datetime));
    datetimedup(user->created_at, date);
}

void set_public_repo(user_reg *user, int public_repo)
{
    user->public_repo = public_repo;
}

void set_followers(user_reg *user, int followers)
{
    user->followers = followers;
}

void set_following(user_reg *user, int following)
{
    user->following = following;
}

void set_gists(user_reg *user, int gists)
{
    user->gists = gists;
}

void set_id(user_reg *user, int id)
{
    user->id = id;
}

void set_follower_list(user_reg *user, int *list)
{
    int follower_size = get_followers(user);
    user->follower_list = malloc(sizeof(int) * follower_size);

    for (int i = 0; i < follower_size; i++)
        user->follower_list[i] = list[i];
}

void set_following_list(user_reg *user, int *list)
{
    int following_size = get_following(user);
    user->following_list = malloc(sizeof(int) * following_size);

    for (int i = 0; i < following_size; i++)
        user->following_list[i] = list[i];
}

void dump_user(user_reg *user)
{
    // id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos
    printf("Id:   %ld\n", user->id);
    printf("Name: %s\n", user->username);
    printf("Type: %d\n", user->type);
    printf("Created_at: %s\n", user->created_at->str_repr);
    printf("Followers: %ld\n", user->followers);
    printf("Following: %ld\n", user->following);
    printf("Gists: %ld\n", user->gists);
    printf("Repos: %ld\n\n", user->public_repo);
}