#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h> 

#include "../include/querier/commits.h"
#include "../include/querier/repos.h"
#include "../include/querier/utils.h"
#include "../include/builder/fetch_tools.h"
#include "../include/ui_helpers.h"

void *xmalloc(size_t size)
{
    void *result = malloc(size);
    if (!result)
    {
        printf("Failed to alocate memory!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

void datetimedup(datetime *dest, datetime *from)
{
    dest->hours = from->hours;
    dest->minutes = from->minutes;
    dest->seconds = from->seconds;

    dest->day = from->day;
    dest->month = from->month;
    dest->year = from->year;

    if (from->str_repr)
        dest->str_repr = strdup(from->str_repr);
}

void free_datetime(datetime *date)
{
    if (date->str_repr) free(date->str_repr);
    free(date);
}

int count_lines(const char *filename)
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

int compare_int(const void *x, const void *y) 
{
    int f = *((int*)x);
    int s = *((int*)y);
    
    if (f > s) return  1;
    if (f < s) return -1;

    return 0;
}

int search_int(int arr[], int l, int r, int x)
{
    if (r >= l) 
    {
        int mid = l + (r - l) / 2;
 
        if (arr[mid] == x) return mid;
        if (arr[mid] > x) return search_int(arr, l, mid - 1, x);
        return search_int(arr, mid + 1, r, x);
    }

    return -1;
}

bool file_exists(const char *filename) 
{
  struct stat buffer;   
  return (stat(filename, &buffer) == 0);
}

bool check_ext(const char *filename, const char *ext)
{
    int arg_length = strlen(filename);
    const char *extension = &filename[arg_length - 4];
    return (strcmp(extension, ext) == 0);
}

bool write_to_file(const char *filepath, char* body)
{
    if (file_exists(filepath)) return false;

    FILE *output = fopen(filepath, "w+");
    if (!output) return false;

    fputs(body, output);
    fclose(output);
    
    return true;
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

int compare_dates(const void *x, const void *y)
{
    commit_reg *a = *(commit_reg**)x;
    commit_reg *b = *(commit_reg**)y;

    datetime *date_a = xmalloc(sizeof(datetime));
    datetime *date_b = xmalloc(sizeof(datetime));

    get_commit_at(a, date_a);
    get_commit_at(b, date_b);

    int result = compare_datetimes(date_a, date_b);

    free(date_a); free(date_b);
    return result;
}

int compare_date_repo(const void *x, const void *y)
{
    repo_reg *a = *(repo_reg**)x;
    repo_reg *b = *(repo_reg**)y;

    datetime *date_a = xmalloc(sizeof(datetime));
    datetime *date_b = xmalloc(sizeof(datetime));

    get_created_at_repo(a, date_a);
    get_created_at_repo(b, date_b);

    int result = compare_datetimes(date_a, date_b);

    free(date_a); free(date_b);
    return result;
}

int compare_sets(const void *x, const void *y)
{
    set a = *((set*)x);
    set b = *((set*)y);

    if (a.occurs < b.occurs) return  1;
    if (a.occurs > b.occurs) return -1;
    return 0;
}

int compare_sets_v2(const void *x, const void *y)
{
    set_v2 a = *((set_v2*)x);
    set_v2 b = *((set_v2*)y);

    if (a.repo_id < b.repo_id) return  1;
    if (a.repo_id > b.repo_id) return -1;
    return 0;
}

int compare_sets_v3(const void *x, const void *y)
{
    set_v3 *a = *((set_v3**)x);
    set_v3 *b = *((set_v3**)y);

    if (a->message_size < b->message_size) return  1;
    if (a->message_size > b->message_size) return -1;
    return 0;
}

bool in_range(int number, int min, int max)
{
    return (number >= min && number <= max) ? true : false;
}

bool test_number(char **string)
{
    char *rest = *string;
    char *id_str = strtok_r(rest, " ", &rest);

    if (atoi(id_str) <= 0)
    {
        *string = rest;
        return false;
    }

    *string = rest;
    return true;
}

bool test_lang(char **string)
{
    char *rest = *string;
    char *lang = strtok_r(rest, " ", &rest);

    if (!is_lang(lang))
    {
        *string = rest;
        return false;
    }

    *string = rest;
    return true;
}

bool test_date(char **string)
{
    char *rest = *string;
    char *date = strtok_r(rest, " ", &rest);

    if (!is_date(date))
    {
        *string = rest;
        return false;
    }

    *string = rest;
    return true;
}

bool test_line(char *params, int id)
{
    if (in_range(id, 1, 4) && strlen(params) > 0)
    {
        printf("Query %d: Invalid parameters, expected 0 but got more.\n", id);
        return false;
    }

    if (id == 5 && !(test_number(&params) && test_date(&params) && test_date(&params)))
    {
        printf("Query %d: Invalid parameters.\n", id);
        return false;
    }

    if (id == 6 && !(test_number(&params) && test_lang(&params)))
    {
        printf("Query %d: Invalid parameters.\n", id);
        return false;
    }

    if (id == 7 && !(test_date(&params)))
    {
        printf("Query %d: Invalid parameters.\n", id);
        return false;
    }

    if (id == 8 && !(test_number(&params) && test_date(&params)))
    {
        printf("Query %d: Invalid parameters.\n", id);
        return false;
    }

    if ((id == 9 || id == 10) && !test_number(&params))
    {
        printf("Query %d: Invalid parameters.\n", id);
        return false;
    }

    return true;
}