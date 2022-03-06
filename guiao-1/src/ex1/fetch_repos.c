#define __USE_XOPEN
#define _GNU_SOURCE

#define MAX_SIZE 130000//70000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../includes/ex1/fetch_repos.h"

// fetches numbers :P
long int fetch_countable(char **buffer, Repo *r)
{
    char *endptr = NULL;
    char *num_str = strdup(strsep(buffer, ";"));

    long int num = strtol(num_str, &endptr, 10);

    if (num < 0 || (num_str[0] == '\0') || *endptr != '\0')
    {
        free(num_str); 
        r->valid = false; 
        return -1;
    }

    free(num_str);
    return num;
}

// fetches text :P
char* fetch_not_numbers(char **buffer, Repo *r, bool sw)
{
    char* message = strdup(strsep(buffer, ";"));

    if (message[0] == '\0' && sw) 
    {
        free(message);
        r->valid = false;
        return "NONE";
    }
    else if (message[0] == '\0' && !sw) {free(message); return "NONE";}

    return message;

}

bool has_wiki(char **buffer, Repo *r)
{
    char* value = strdup(strsep(buffer, ";"));

    if (strcmp(value , "True") == 0) {free(value); return true;}
    else if (strcmp(value, "False") == 0) {free(value); return false;}
    else { free(value); r->valid = false; return false; }
}

bool c_date(int year, int month, int day)
{
    if (year == 2005)
    {
        if ((month <= 4) && (day < 7)) return false;
    }
    else if (year < 2005) return false;

    if (year == 2021)
    {
        if ((month >= 11) && (day > 10)) return false;
    }
    else if (year > 2021) return false;

    return true;
}

void f_date(char **buffer, struct tm *ct, Repo *r)
{
    char* string = strdup(strsep(buffer, ";"));

    int year, month, day, hour, min, sec;
    char tz_sym;

    sscanf(string, 
           "%4d-%2d-%2d %2d:%2d:%2d%c", 
           &year, &month, &day, &hour, &min, &sec, &tz_sym);

    if (string[19] != '\0') { free(string); r->valid = false; return; }

    strptime(string, "%Y-%m-%d %H:%M:%S", ct);

    ct->tm_year += 1900;
    ct->tm_mon += 1;

    if (year != ct->tm_year || month != ct->tm_mon || day != ct->tm_mday ||
        hour != ct->tm_hour || min != ct->tm_min   || sec != ct->tm_sec   )
        { free(string); r->valid = false; printf("%s\n", *buffer); return; }

    if (!c_date(ct->tm_year, ct->tm_mon, ct->tm_mday)) { free(string); r->valid = false; return; }

}

long int fetch_size(char **buffer, Repo *r)
{
    char *tmp = strdup(*buffer);
    tmp[strcspn(tmp, "\t\r\n")] = 0;

    char *endptr = NULL;
    char *size_str = strdup(strsep(&tmp, ";"));

    long int size = strtol(size_str, &endptr, 10);

    if (size < 0 || (size_str[0] == '\0') || (*endptr != '\0'))
    { 
        free(tmp); free(size_str);
        r->valid = false; 
        return -1;
    }

    free(tmp); free(size_str);
    return size;
}

Repo init_repo(char* buffer)
{
    Repo r;
    // ct : commit_time , ut : update_time
    struct tm ct = {0}, ut = {0};

    r.valid = true;

    r.id = fetch_countable(&buffer, &r);
    r.owner_id = fetch_countable(&buffer, &r);

    r.full_name = fetch_not_numbers(&buffer, &r, true);
    r.license = fetch_not_numbers(&buffer, &r, true);

    r.has_wiki = has_wiki(&buffer, &r);

    r.description = fetch_not_numbers(&buffer, &r, false);
    r.language = fetch_not_numbers(&buffer, &r, true);
    r.default_branch = fetch_not_numbers(&buffer, &r, true);

    f_date(&buffer, &ct, &r);
    r.created_at = ct;

    f_date(&buffer, &ut, &r);
    r.updated_at = ut;

    r.forks_count = fetch_countable(&buffer, &r);
    r.open_issues = fetch_countable(&buffer, &r);
    r.stargazers_count = fetch_countable(&buffer, &r);
    r.size = fetch_size(&buffer, &r);

    return r;
}

bool frepos(char* filename)
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE *file_pointer = fopen(filename, "r");
    FILE *output_file = fopen("./saida/repos-ok.csv", "w");

    if (!file_pointer) { fprintf(stderr, "Could not open %s!\n", filename); return false; }
    if (!output_file)  { fprintf(stderr, "Could not open ./saida/repos-ok.csv !\n"); return false; }

    printf("Opened file: %s\n",filename);

    char* buffer = malloc(sizeof(char) * MAX_SIZE);
    char* keep = malloc(sizeof(char) * MAX_SIZE);

    int i = 0, not_valid = 0;

    char* REPO_HEADER = fgets(buffer, MAX_SIZE, file_pointer);
    fputs(REPO_HEADER, output_file);

    printf("> Reading data ...\n");

    while(fgets(buffer, MAX_SIZE, file_pointer))
    {
        keep = strcpy(keep, buffer);

        Repo repo = init_repo(buffer);

        if (repo.valid) {fputs(keep, output_file);}
        else { not_valid++; printf("%s\n", keep); }

        i++;
    }

    free(buffer); free(keep);
    fclose(file_pointer); fclose(output_file);

    printf("> Done!\n");
    printf("> Scanned %d lines, removed %d keeping %d lines.\n", i, not_valid, i - not_valid);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("> CPU time used %f sec.\n\n", cpu_time_used);

    return true;

}
