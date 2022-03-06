#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "../include/ex3/database.h"
#include "../include/ex3/utils.h"
#include "../include/ex3/database_tools.h"

int date_cmp (datetime *my_date, datetime *given_date) 
{
    //  0 : my_date date = given_date date 
    //  1 : my_date date > given_date date 
    // -1 : my_date date < given_date date
    int r = my_date->year - given_date->year,
        y = my_date->month - given_date->month,
        x = my_date->day - given_date->day;

    int ret = -2;

    if (r > 0) ret = 1; 
    if (r < 0) ret = -1; 
    else if (r == 0) 
    {
        if(y> 0) ret = 1;
        if(y< 0) ret = -1;
        else if (y == 0) 
        {
            if(x > 0) ret = 1;
            else if(x < 0) ret = -1;
            else ret = time_cmp(my_date,given_date);
        }
    }

    return ret;
}

int time_cmp (datetime *my_date, datetime *given_date) 
{    
    //  0 : my_date date = given_date date 
    //  1 : my_date date > given_date date 
    // -1 : my_date date < given_date date
    int r = my_date->hours - given_date->hours,
        y = my_date->minutes- given_date->minutes,
        x = my_date->seconds- given_date->seconds;

    int ret = -2;

    if (r > 0) ret = 1; 
    if (r < 0) ret = -1; 
    else if (r == 0) 
    {
        if(y > 0) ret = 1;
        if(y < 0) ret = -1;
        else if (y == 0) 
        {
            if(x > 0) ret = 1;
            else ret = -1;
        }
    }

    return ret;

}

bool linear_search(long *array, int key, int size)
{
    if (size == 0) return false;
    for (int i = 0; i < size; i++)
    {
        if (array[i] == key) return true;
    }
    
    return false;
}

bool equal_date(datetime *my_date, datetime *given_date) 
{
    return strcmp(my_date->str_repr, given_date->str_repr) ? true : false;
}

int compare_datetimes(datetime *my_date, datetime *to_compare)
{
    /*
        -1 : my_date < to_compare
        0  : equal dates, with equal times
        1  : my_date > to_compare
    */

    int my_day  = my_date->day,
        my_mon  = my_date->month,
        my_year = my_date->year;

    int to_day  = to_compare->day,
        to_mon  = to_compare->month,
        to_year = to_compare->year;

    if (my_year > to_year) return 1; // my_date > to_compare
    if (my_year < to_year) return -1; // my_date < to_compare

    // At this point years are the same.
    if (my_mon > to_mon) return 1; // my_date > to_compare
    if (my_mon < to_mon) return -1; // my_date < to_compare

    // Here the months are the same.
    if (my_day > to_day) return 1; // my_date > to_compare
    if (my_day < to_day) return -1; // my_date < to_compare

    // Here the days are the same, so we need to compare dates.
    int my_hour = my_date->hours,
        my_mins = my_date->minutes,
        my_secs = my_date->seconds;

    int to_hour = to_compare->hours,
        to_mins = to_compare->minutes,
        to_secs = to_compare->seconds;

    if (my_hour > to_hour) return 1;
    if (my_hour < to_hour) return -1;

    // Hours are the same.
    if (my_mins > to_mins) return 1;
    if (my_mins < to_mins) return -1;

    // Minutes are the same.
    if (my_secs > to_secs) return 1;
    if (my_secs < to_secs) return -1;

    return 0;

}

int triplet_search(triplet **array, long key, int size)
{
    if (size <= 0) return -1;

    for (int i = 0; i < size; i++)
    {
        if (array[i]->id == key) return i;
    }

    return -1;
}

int triplet_search_language(triplet **array, char *key, int size)
{
    if (size <= 0) return -1;

    for (int i = 0; i < size; i++)
    {
        if (strcmp(array[i]->username, key) == 0) return i;
    }   

    return -1; 
}

int search_q9(q9 **array, long key, int size)
{
    if (size <= 0) return -1;

    for (int i = 0; i < size; i++)
    {
        if (array[i]->user_id == key) return i;
    }   

    return -1; 
}

int compare_q9_commits(const void *x, const void *y)
{
    q9 *a = *(q9**)x;
    q9 *b = *(q9**)y;

    if (a->total_commits > b->total_commits) return -1;
    if (a->total_commits < b->total_commits) return 1;
    return 0;
}

int compare_q9_ids(const void *x, const void *y)
{
    q9 *a = *(q9**)x;
    q9 *b = *(q9**)y;

    if (a->user_id > b->user_id) return -1;
    if (a->user_id < b->user_id) return 1;
    return 0;
}

int bin_search_q9(q9 **array, int start, int end, long key)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;

        if (array[mid]->user_id == key) return mid;

        if (array[mid]->user_id > key) return bin_search_q9(array, start, mid - 1, key);
        else return bin_search_q9(array, mid + 1, end, key);
    }

    return -1;
}

int compare_triplet(const void *x, const void *y)
{
    triplet *a = *(triplet**)x;
    triplet *b = *(triplet**)y;

    if (a->total_commits > b->total_commits) return -1;
    if (a->total_commits < b->total_commits) return 1;
    return 0;
}

void string_to_lower(char *string)
{
    for(int i = 0; string[i]; i++) string[i] = tolower(string[i]);
}

int get_random_int(int max)
{
    srand(time(NULL));
    return (rand() % max);
}

int get_lines_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    int lines = 0;

    while(!feof(file))
    {
        char c = fgetc(file);
        if(c == '\n') lines++;
    }

    return lines;
}

int search(int *array, int start, int end, int value)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;

        if (array[mid] == value) return mid;
        if (array[mid] > value) return search(array, start, mid - 1, value);
        
        return search(array, mid + 1, end, value);
    }

    return -1;
}

void datetimedup(datetime *dest, datetime *source)
{
    dest->str_repr = strdup(source->str_repr);

    dest->day = source->day;
    dest->hours = source->hours;
    dest->minutes = source->minutes;
    
    dest->year = source->year;
    dest->month = source->month;
    dest->day = source->day;
}

// Terminal colors.
void failed_red() { printf("\033[1;31m"); }
void passed_green() { printf("\033[1;33m"); }
void func_name() { printf("\033[0;34m"); }
void yellow() { printf("\033[0;33m"); }
void reset() { printf("\033[0m"); }
