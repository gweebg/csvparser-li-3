#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

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

typedef enum user_type
{
    TYPE_BOT = 0, /**< represents the BOT user type */
    TYPE_USR = 1, /**< represents the USER user type */
    TYPE_ORG = 2  /**< represents the ORGANIZATION user type */
    
} type;

typedef struct set
{
    long user_id;
    int occurs;
} set;

typedef struct set2
{
    long repo_id;
    char* string;
} set_v2;

typedef struct set3
{
    long committer_id;
    int message_size;
} set_v3;

typedef struct c_array
{
    set_v3 **array;
    int size;
} commit_array;

void *xmalloc(size_t size);
void datetimedup(datetime *dest, datetime *from);
void free_datetime(datetime *date);
int count_lines(const char *filename);
int compare_int(const void *x, const void *y);
int search_int(int arr[], int l, int r, int x);
bool file_exists(const char *filename);
bool check_ext(const char *filename, const char *ext);
bool write_to_file(const char *filepath, char* body);
int compare_datetimes(datetime *my_date, datetime *to_compare);
int compare_dates(const void *x, const void *y);
int compare_date_repo(const void *x, const void *y);
int compare_sets(const void *x, const void *y);
int compare_sets_v2(const void *x, const void *y);
int compare_sets_v3(const void *x, const void *y);
bool test_line(char *params, int id);

#endif