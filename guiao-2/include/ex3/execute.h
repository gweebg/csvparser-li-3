#ifndef EXEC_H
#define EXEC_H

#include "../include/ex3/database.h"

typedef struct query
{
    int query_id;
    char* param;
    int number;
} QUERY;

/**
 * @brief Executes a query that has parameters (ids 5 to 10).
 * @param query struct containing the operation id and parameters.
 * @return True, if the query is valid and the query got executed. 
 */
bool execute_param(QUERY *query);

/**
 * @brief Executes a non-parameterized query (ids 1 to 4).
 * @param query struct containing the operation id and parameters.
 * @return True, if the query is valid and the query got executed.
 */
bool execute(QUERY *query, user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics);

#endif