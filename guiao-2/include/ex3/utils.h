#ifndef HELPERS_H
#define HELPERS_H

#define __USE_XOPEN
#define _GNU_SOURCE

#include "../include/ex3/database.h"

void failed_red();
void passed_green();
void func_name();
void yellow();
void reset();

bool compare_date (datetime my_date, datetime given_date);
int date_cmp (datetime *my_date, datetime *given_date);
int time_cmp (datetime *my_date, datetime *given_date);

bool linear_search(long *array, int key, int size);
int compare_datetimes(datetime *my_date, datetime *to_compare);

int triplet_search(triplet **array, long key, int size);
int compare_triplet(const void *x, const void *y);

void string_to_lower(char *string);

int triplet_search_language(triplet **array, char* key, int size);
int get_random_int(int max);

int get_lines_from_file(const char *filename);
int search(int *array, int start, int end, int value);
int search_q9(q9 **array, long key, int size);
int compare_q9_commits(const void *x, const void *y);
int compare_q9_ids(const void *x, const void *y);
int bin_search_q9(q9 **array, int start, int end, long key);

void datetimedup(datetime *dest, datetime *source);

#endif