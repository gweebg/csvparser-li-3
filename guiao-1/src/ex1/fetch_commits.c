#define __USE_XOPEN
#define _GNU_SOURCE

#define MAX_SIZE 5000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../includes/ex1/fetch_commits.h"

// repo_id;author_id;committer_id;commit_at;message

bool confirm_date(int year, int month, int day)
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

void update_date(char **buffer, struct tm *ct, Commit *c)
{
    char* string = strdup(strsep(buffer, ";"));

    int year, month, day, hour, min, sec;
    char tz_sym;

    sscanf(string, 
           "%4d-%2d-%2d %2d:%2d:%2d%c", 
           &year, &month, &day, &hour, &min, &sec, &tz_sym);

    if (string[19] != '\0') { free(string); c->valid = false; return; }

    strptime(string, "%Y-%m-%d %H:%M:%S", ct);

    ct->tm_year += 1900;
    ct->tm_mon += 1;

    if (year != ct->tm_year || month != ct->tm_mon || day != ct->tm_mday ||
        hour != ct->tm_hour || min != ct->tm_min   || sec != ct->tm_sec   )
        { free(string); c->valid = false; return; }

    if (!confirm_date(ct->tm_year, ct->tm_mon, ct->tm_mday)) { free(string); c->valid = false; return; }

}

long int fetch_repo_author_commiter_id(char **buffer, Commit *c)
{
    char *endptr = NULL;
    char *id_str = strdup(strsep(buffer, ";"));

    long int id = strtol(id_str, &endptr, 10);

    if (id < 0 || (id_str[0] == '\0') || *endptr != '\0')
    { 
        free(id_str);
        c->valid = false; 
        return -1;
    }

    free(id_str);
    return id;
}

char* fetch_message(char **buffer, Commit *c)
{
    char* message = strdup(strsep(buffer, ";"));

    if (message[0] == '\0') 
    {
        c->valid = false;
        return "NONE";
    }

    return message;
}

Commit init_commit(char* buffer)
{
    Commit c;
    struct tm t = {0};

    c.valid = true;

    c.repo_id = fetch_repo_author_commiter_id(&buffer, &c);
    c.author_id = fetch_repo_author_commiter_id(&buffer, &c);
    c.committer_id = fetch_repo_author_commiter_id(&buffer, &c);

    update_date(&buffer, &t, &c);
    c.commit_at = t;

    c.message = fetch_message(&buffer, &c);;

    return c;
}

bool fcommits(char* filename)
{
    /**
     * @brief Starting the CPU timer.
     */
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    /**
     * @brief Opening both the input and output file.
     */
    FILE *file_pointer = fopen(filename, "r");
    FILE *output_file = fopen("./saida/commits-ok.csv", "w");

    if (!file_pointer || !output_file) { fprintf(stderr, "Could not open the file!"); return false; }

    printf("Opened file: %s\n",filename);

    /**
     * @brief Max line size on the file. Might have to change it some other time.
     */
    char* buffer = malloc(sizeof(char) * MAX_SIZE);
    char* keep = malloc(sizeof(char) * MAX_SIZE);

    int i = 0, not_valid = 0;

    /**
     * @brief Copypasting the file header to the output file.
     */
    char* COMMIT_HEADER = fgets(buffer, MAX_SIZE, file_pointer);
    fputs(COMMIT_HEADER, output_file); 

    printf("> Reading data ...\n");

    /**
     * @brief Looping through the file and parsing the data.
     */
    while(fgets(buffer, MAX_SIZE, file_pointer))
    {
        keep = strcpy(keep, buffer);

        Commit commit = init_commit(buffer);
        // print_commit(commit);

        if (commit.valid) {fputs(keep, output_file);} //  printf("%s\n", keep);
        else { not_valid++;}

        i++;
    }

    free(buffer); free(keep);
    fclose(file_pointer); fclose(output_file);

    printf("> Done!\n");
    printf("> Scanned %d lines, removed %d keeping %d lines.\n", i, not_valid, i - not_valid);

    /**
     * @brief Computing time of usage of the CPU.
     */
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("> CPU time used %f sec.\n\n", cpu_time_used);

    return true;

}