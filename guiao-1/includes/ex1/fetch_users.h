#ifndef USERS_H
#define USERS_H

#include <time.h>
#include <stdbool.h>

/**
 * @brief Types of Users.
 */
enum Type { BOT = 1,   /**< Bot **/
            ORG = 2,   /**< Organization **/
            USR = 3,   /**< User **/
            NAT = 4 }; /**< Not A Type **/

/**
 * @brief Types of User elements.
 */
typedef struct
{
    long int public_repo, /**< long int that represents the number of public repos of a user. */
             followers, /**< long int that represents the number of followers of a user. */
             following, /**< long int that represents the number of following users of a user. */
             gists, /**< long int that represents the number of gists of a user. */
             id; /**< long int that represents the id number of a user. */

    int following_count; /**< int that represents the list count of following users of a user. */
    int* following_list; /**< array of integers that has other user ids that follow the original user. */

    int follower_count; /**< int that represents the list count of followers of a user. */
    int* follower_list; /**< array of integers that has other user ids whose are followed by the original user. */

    struct tm created_at; /**< struct that represents the datetime on which the user account was created. */
    enum Type type; /**< enum that represents the User Type. */
    
    bool valid; /**< bool to check if the user is valid or not. */

    char* user_name; /**< string that represents the login of the user. */
    
} User;

/**
 * @brief Gets a number from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a user (struct User).
 * @return long int number retrieved.
 */
long int fetch_number(char **buffer, User *u);

/**
 * @brief Gets the number of repos of a user from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a user (struct User).
 * @return long int number retrieved.
 */
long int fetch_repo(char** buffer, User *u);

/**
 * @brief Gets text from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a user (struct User).
 * @return string text retrieved.
 */
char* fetch_text(char **buffer, User *u);

/**
 * @brief Gets the type from a csv string.
 * @param double pointer to a string to be parsed.
 * @param pointer to a user (struct User).
 * @return enum type retrieved.
 */
enum Type fetch_type(char **buffer, User *u);

/**
 * @brief Creates limits to a date to check its validation.
 * @param int year.
 * @param int month.
 * @param int day.
 * @return true if it's valid, else false.
 */
bool verify_date(int year, int month, int day);

/**
 * @brief Verifies if a date is valid.
 * @param double pointer to a string to be parsed.
 * @param pointer to a date (struct tm).
 * @param pointer to a user (struct User).
 */
void fetch_date(char **buffer, struct tm *ct, User *u);

/**
 * @brief Removes first and last element from a string.
 * @param original string.
 * @param new string.
 */
void remove_first_last(const char *string, char *result);

/**
 * @brief Turns a string into an array.
 * @param string csv string.
 * @param pointer to a user (struct User).
 * @param int array size count.
 * @return an array of integers.
 */
int* string_to_array(char* string, User *user, int *count);

/**
 * @brief Filters users; returning users with possibly valid data.
 * @param pointer to a string to be parsed.
 * @return a user (struct User) to be tested.
 */
User init_user(char* buffer);

/**
 * @brief Check if a user is valid.
 * @param user (struct User).
 * @return true if the user is valid; else false.
 */
bool valid(User user);

/**
 * @brief Tests all users in a file.
 * @param filename Name of the file (users-ok.csv).
 * @return true if the user is valid; else false.
 */
bool fusers(char* filename);

#endif