#ifndef DB_H
#define DB_H

#define  USERS  "./entrada/users-g3.csv"
#define  REPOS  "./entrada/repos-g3.csv"
#define COMMITS "./entrada/commits-g3.csv"

#include "userdata.h"
#include "commitdata.h"
#include "repodata.h"

typedef enum user_type
{
    TYPE_BOT = 0, /**< represents the BOT user type */
    TYPE_USR = 1, /**< represents the USER user type */
    TYPE_ORG = 2  /**< represents the ORGANIZATION user type */
} type;

/**
 * @brief Struct that represents a datetime with the %Y-%m-%d %H-%m-%s format.
 */
typedef struct datetime
{

    int year, /**< int that represents the date year. */ 
        month, /**< int that represents the date month. */ 
        day; /**< int that represents the date day. */ 

    int hours, /**< int that represents the time hours. */  
        minutes, /**< int that represents the time minutes. */ 
        seconds; /**< int that represents the time seconds. */ 

    char* str_repr; /**< string representation of datetime. */

} datetime;

typedef struct triplet
{
    char *username;
    long id;
    int total_commits;
    int id2;
} triplet;

typedef struct q10aux
{
    char* username;
    long repo_id;
    long id;
    int message_size;
} q10;

typedef struct q9aux
{
    char* username;
    long user_id;
    int total_commits;
} q9;

bool generate_catalogs(user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics);

#endif