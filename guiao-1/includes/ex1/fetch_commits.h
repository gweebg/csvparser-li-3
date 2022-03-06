#ifndef COMMITS_H
#define COMMITS_H

#include <time.h>

/**
 * @brief Types of Commit elements.
 */
typedef struct
{

    long int repo_id; /**< long int that represents the repo id. */
    long int committer_id; /**< long int that represents the committer id. */
    long int author_id; /**< long int that represents the author id. */

    struct tm commit_at; /**< struct that represents the datetime on which the commit was made. */

    char* message; /**< string that represents a commit message. */

    bool valid; /**< bool to check if the commit is valid or not. */

} Commit;

/**
 * @brief Creates limits to a date to check its validation.
 * @param int year.
 * @param int month.
 * @param int day.
 * @return true if it's valid, else false.
 */
bool confirm_date(int year, int month, int day);

/**
 * @brief Verifies if a date is valid.
 * @param double pointer to a string to be parsed.
 * @param pointer to a date (struct tm).
 * @param pointer to a commit (struct Commit).
 */
void update_date(char **buffer, struct tm *ct, Commit *c);

/**
 * @brief Gets an id number from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a commit (struct Commit).
 * @return long int repo,author or committer id.
 */
long int fetch_repo_author_commiter_id(char **buffer, Commit *c);

/**
 * @brief Gets a text message from a csv string.
 * @param pointer to a commit (struct Commit).
 * @return string commit message retrieved.
 */
char* fetch_message(char **buffer, Commit *c);

/**
 * @brief Filters commits; returning commits with valid data.
 * @param pointer to a commit (struct Commit).
 * @return a valid commit.
 */
Commit init_commit(char* buffer);

/**
 * @brief Tests all commits in a file.
 * @param filename Name of the file (commits-ok.csv).
 * @return true if the commit is valid; else false.
 */
bool fcommits(char* filename);

#endif