#ifndef REPOS_H
#define REPOS_H

#include <time.h>
#include <stdbool.h>

/**
 * @brief Types of Repo elements.
 */
typedef struct
{
    char* license; /**< sting that represents the repo license(if it exists). */
    char* description; /**< string that represents the repo description. */
    char* language; /**< string that represents the repo language. */
    char* full_name; /**< string that represents the repo full name. */
    char* default_branch; /**< string that represents the repo default branch. */

    struct tm created_at; /**< struct that represents the datetime on which the repo was created. */
    struct tm updated_at; /**< struct that represents the datetime on which the repo was updated. */

    long int forks_count; /**< long int that represents the number of the repo forks. */
    long int open_issues; /**< long int that represents the number of the repo open issues. */
    long int stargazers_count; /**< long int that represents the number of the repo stargazers. */
    long int owner_id; /**< long int that represents the id of the repo owner. */
    long int id; /**< long int that represents the repo id. */
    long int size; /**< long int that represents the repo size. */
    
    bool has_wiki; /**< bool to check if the repo has wiki or not. */
    bool valid; /**< bool to check if the repo is valid or not. */

} Repo;

/**
 * @brief Gets a number from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a repo (struct Repo).
 * @return long int number retrieved.
 */
long int fetch_countable(char **buffer, Repo *r);

/**
 * @brief Gets text from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a repo (struct Repo).
 * @return string text retrieved.
 */
char* fetch_not_numbers(char **buffer, Repo *r, bool sw);

/**
 * @brief Checks if repo has wiki.
 * @param double pointer to a string to be parsed.
 * @param pointer to a repo (struct Repo).
 * @return true if yes, else false.
 */
bool has_wiki(char **buffer, Repo *r);

/**
 * @brief Creates limits to a date to check its validation.
 * @param int year.
 * @param int month.
 * @param int day.
 * @return true if it's valid, else false.
 */
bool c_date(int year, int month, int day);

/**
 * @brief Verifies if a date is valid.
 * @param double pointer to a string to be parsed.
 * @param pointer to a date (struct tm).
 * @param pointer to a repo (struct Repo).
 */
void f_date(char **buffer, struct tm *ct, Repo *r);

/**
 * @brief Gets the size number of a repo from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a repo (struct Repo).
 * @return long int repo size retrieved.
 */
long int fetch_size(char **buffer, Repo *r);

/**
 * @brief Filters repos; returning repos with valid data.
 * @param pointer to a repo (struct Repo).
 * @return a valid repo.
 */
Repo init_repo(char* buffer);

/**
 * @brief Tests all repos in a file.
 * @param filename Name of the file (repos-ok.csv).
 * @return true if the repo is valid; else false.
 */
bool frepos(char* filename);

#endif

