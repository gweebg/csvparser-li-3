#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <time.h>

/* Recebe um string pointer e vê se o primeiro parametro é um número. */
void check_number(char **csv_string, long *value, bool *validity)
{
    if (!*validity) return;
    if ((*csv_string)[0] == ';')
    {
        *validity = false;
        return;
    }

    char* endptr = NULL;
    char* rest = *csv_string;
    char* str_num = strtok_r(rest, ";", &rest);

    long num = strtol(str_num, &endptr, 10);
    if (num < 0 || (str_num[0] == '\0') || *endptr != '\0') *validity = false;

    if (value != NULL) *value = num;
    *csv_string = rest;
}

/* Extrai a primeira string delimitada pelo ; da frase csv. */
void check_string(char **csv_string, bool can_be_empty, bool *validity)
{
    char* message = strdup(strsep(csv_string, ";"));

    if (message[0] == '\0' && !can_be_empty) 
    {
        free(message);
        *validity = false;
        return;
    }
    else if (message[0] == '\0' && can_be_empty) 
    {
        free(message); 
        return;
    }
}

void skip_token(char **csv_string)
{
    char* rest = *csv_string;
    strtok_r(rest, ";", &rest);
    *csv_string = rest;
}

/* Verifica se o primeiro token é um type */
void check_type(char **csv_string, bool *validity)
{
    if (!*validity) return;

    char* rest = *csv_string;
    char* type = strtok_r(rest, ";", &rest);

    if (strcmp(type, "User") != 0 &&
        strcmp(type, "Bot") != 0 &&
        strcmp(type, "Organization") != 0  ) *validity = false;

    *csv_string = rest;
}

bool check_date_bounds(int year, int month, int day)
{
    if (year == 2005)
    {
        if ((month <= 4) && (day < 7)) return false;
        else if (month < 4) return false;
    }
    else if (year < 2005) return false;

    if (year == 2021)
    {
        if ((month >= 11) && (day > 10)) return false;
        else if (month > 11) return false;
    }
    else if (year > 2021) return false;

    return true;
}

void check_date(char **csv_string, bool *validity) // 26 !=
{
    if (!*validity) return;
    if ((*csv_string)[0] == ';')
    {
        *validity = false;
        return;
    }

    int hour, min, sec; 
    int year, month, day;

    char* rest = *csv_string;
    char* datetime = strtok_r(rest, ";", &rest);

    if (datetime[19] != '\0')
    {
        *validity = false;
        return;
    }

    sscanf(datetime, 
            "%4d-%2d-%2d %2d:%2d:%2d", 
            &year, &month, &day, &hour, &min, &sec);


    struct tm ct = {0};
    strptime(datetime, "%Y-%m-%d %H:%M:%S", &ct);

    ct.tm_year += 1900;
    ct.tm_mon += 1;

    if (year != ct.tm_year || month != ct.tm_mon || day != ct.tm_mday ||
        hour != ct.tm_hour || min != ct.tm_min   || sec != ct.tm_sec   )
        { *validity = false; return; }

    if (!check_date_bounds(year, month, day)) { *validity = false; return; }
    *csv_string = rest;
}

bool is_number(char* str_num)
{
    char* endptr = NULL;
    long num = strtol(str_num, &endptr, 10);

    if (num < 0 || (str_num[0] == '\0') || *endptr != '\0') return false;
    else return true;
}

void check_array(char **csv_string, long *size, bool *validity)
{
    if (!*validity) return;
    if ((*csv_string)[0] == ';')
    {
        *validity = false;
        return;
    }

    char* rest = *csv_string;
    char* array_str = strtok_r(rest, ";", &rest);

    if (array_str[0] != '[' || array_str[strlen(array_str) - 1] != ']')
    {
        *validity = false;
        return;
    }

    char *result = array_str + 1;
    result[strlen(result) - 1] = '\0'; 

    char* array_elem = strtok(result, ",");
    while(array_elem)
    {
        if (is_number(array_elem)) (*size)++;
        array_elem = strtok(NULL, ",");
    }

    *csv_string = rest;
}

void compare_f(long followers, long actual_followers, long following, long actual_following, bool *validity)
{
    if (!*validity) return;
    if ((followers != actual_followers) || (following != actual_following)) *validity = false;
}

void check_wiki(char **csv_string, bool *validity)
{
    if (!*validity) return;

    char* rest = *csv_string;
    char* type = strtok_r(rest, ";", &rest);

    if (strcmp(type, "True") != 0 &&
        strcmp(type, "False") != 0) *validity = false;

    *csv_string = rest;
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

bool is_date(char *datetime)
{
    int year, month, day;
    if (datetime[11] != '\0') return false;

    sscanf(datetime, 
           "%4d-%2d-%2d", 
           &year, &month, &day);

    struct tm ct = {0};
    strptime(datetime, "%Y-%m-%d", &ct);

    ct.tm_year += 1900;
    ct.tm_mon += 1;

    if (year != ct.tm_year || month != ct.tm_mon || day != ct.tm_mday) return false;

    if (!check_date_bounds(year, month, day)) return false;

    return true;
}