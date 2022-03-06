#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "../include/ex3/database_tools.h"
#include "../include/ex3/database.h"
#include "../include/ex3/utils.h"

int compare_int(const void *x, const void *y) 
{
    int f = *((int*)x);
    int s = *((int*)y);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

void csv_to_number(char **csv_string, long *output)
{
    char* rest = *csv_string;
    char* str_num = strtok_r(rest, ";", &rest);

    *output = atoi(str_num);
    *csv_string = rest;
}

void csv_to_string(char **csv_string, char **output)
{
    char* rest = *csv_string;
    char* string = strtok_r(rest, ";", &rest);

    size_t size = rest - string;

    if (strlen(string) > 0)
    {
        *output = (char*)malloc(sizeof(char) * size);
        strncpy(*output, string, size);
    }

    *csv_string = rest;
}

void csv_to_type(char **csv_string, type *output)
{
    char* rest = *csv_string;
    char* string = strtok_r(rest, ";", &rest);

    if (strcmp(string, "User") == 0) *output = TYPE_USR;
    else if (strcmp(string, "Bot") == 0) *output = TYPE_BOT;
    else *output = TYPE_ORG;

    *csv_string = rest;
}

void csv_to_datetime(char **csv_string, datetime **output)
{
    (*output) = malloc(sizeof(datetime*));

    char* rest = *csv_string;
    char* datetime_string = strtok_r(rest, ";", &rest);

    (*output)->str_repr = malloc(sizeof(char) * strlen(datetime_string));
    strncpy((*output)->str_repr, datetime_string, strlen(datetime_string));

    int year, month, day, hour, min, sec;

    sscanf(datetime_string, 
           "%4d-%2d-%2d %2d:%2d:%2d", 
           &year, &month, &day, &hour, &min, &sec);

    (*output)->year = year; (*output)->month = month; (*output)->day = day;
    (*output)->hours = hour; (*output)->minutes = min; (*output)->seconds = sec;

    *csv_string = rest;
}

void csv_to_array(char **csv_string, int *output, int output_size)
{
    char* rest = *csv_string;
    char* array_string = strtok_r(rest, ";", &rest);

    if (output_size < 0)
    {
        *csv_string = rest;
        return;
    }

    if (!output)
    {
        fprintf(stderr, "Could not allocate memory for 'follower_list' in 'create_user'.\n");
        exit(EXIT_FAILURE);
    }

    char *result = array_string + 1;
    result[strlen(result) - 1] = '\0'; 

    int index = 0;
    char* array_elem = strtok(result, ",");

    while(array_elem)
    {
        int value = atoi(array_elem);
        output[index] = value;
        index++;
        array_elem = strtok(NULL, ",");
    }

    qsort(output, output_size, sizeof(int), compare_int);
    // for (int i = 0; i < output_size; i++) printf("%d\n", output[i]);

    *csv_string = rest;
}

void csv_check_wiki(char **csv_string, bool *output)
{
    char* rest = *csv_string;
    char* string = strtok_r(rest, ";", &rest);

    if (strcmp(string, "True") == 0) *output = true;
    else if (strcmp(string, "False") == 0) *output = false;

    *csv_string = rest;
}