#include <sys/stat.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>  
#include <time.h>

#include "../include/ex3/query_parser.h"
#include "../include/ex3/execute.h"
#include "../include/ex3/utils.h"
#include "../include/ex3/database.h"

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

bool is_valid_date(char *string)
{
    if (strlen(string) != 10) return false;

    int year, month, day;
    sscanf(string, 
           "%4d-%2d-%2d", 
           &year, &month, &day);

    struct tm *ct = {0};
    strptime(string, "%Y-%m-%d", ct);

    ct->tm_year += 1900;
    ct->tm_mon += 1;

    if (year != ct->tm_year || month != ct->tm_mon || day != ct->tm_mday) return false;
    return true;
}

bool validate_params(char *parameters, int query_id)
{
    if ((query_id >= 1 && query_id <= 4) && parameters[0] != '\0')
    {
        printf("        > ERROR : Expected no arguments for query with id %d.\n", query_id);
        return false;
    }

    if (query_id == 9 || query_id == 10)
    {
        char *endptr; int n = strtol(parameters, &endptr, 10);

        if (parameters[0] == '\0') {printf("        > ERROR : Expected one argument but got 0 instead (id : %d).\n", query_id); return false;}
        if (n == 0 && *endptr == '\0') {printf("        > ERROR : Expected a positive argument but got 0 instead (id : %d).\n", query_id); return false;}
        if (*endptr != '\0') {printf("        > ERROR : Expected one argument but got more than one instead or an invalid one (id : %d).\n", query_id); return false;}
    }

    if (query_id == 5)
    {
        if (parameters[0] == '\0' || strlen(parameters) < 24)
            { printf("        > ERROR : Detected invalid parameters (id : %d).\n", query_id); return false; }
        int where = 0;

        char *tok = strtok(parameters, " ");
        while(tok != NULL) 
        {
            if (where == 0 && atoi(tok) == 0) 
                { printf("        > ERROR : Expected a positive argument but got 0 instead (id : %d).\n", query_id); return false; }
            if ((where == 1 || where == 2) && !is_valid_date(tok))
                { printf("        > ERROR : Detected invalid date (id : %d).\n", query_id); return false; }

            where++;
            tok = strtok(NULL, " ");
        }
    } 

    if (query_id == 7)
    {
        if (parameters[0] == '\0' || strlen(parameters) != 10)
        {
            printf("        > ERROR : No argument given or invalid date (id : %d).\n", query_id);
            return false;
        }
        if (!is_valid_date(parameters)) 
        {
            printf("        > ERROR : Detected invalid date (id : %d).\n", query_id);
            return false;
        }
    }

    if (query_id == 8)
    {
        if (parameters[0] == '\0' || strlen(parameters) < 13)
        { 
            printf("        > ERROR : Detected invalid parameters (id : %d).\n", query_id); 
            return false; 
        }

        int where = 0;

        char *tok = strtok(parameters, " ");
        while(tok != NULL) 
        {
            if (where == 0 && atoi(tok) == 0) 
            { 
                printf("        > ERROR : Expected a positive argument but got 0 instead (id : %d).\n", query_id); 
                return false; 
            }
            else if ((where == 1) && !is_valid_date(tok))
            { 
                printf("        > ERROR : Detected invalid date (id : %d).\n", query_id); 
                return false; 
            }
            else break;

            where++;
            tok = strtok(NULL, " ");
        }
    }

    if (query_id == 6)
    {
        if (parameters[0] == '\0')
        { 
            printf("        > ERROR : Expected two parameters but got zero (id : %d).\n", query_id); 
            return false; 
        }

        int where = 0;

        char *tok = strtok(parameters, " ");
        while(tok != NULL) 
        {
            if (where == 0 && atoi(tok) == 0) 
            { 
                printf("        > ERROR : Expected a positive argument but got 0 instead (id : %d).\n", query_id); 
                return false; 
            }

            where++;
            tok = strtok(NULL, " ");
        }
    }

    return true;
}

bool read_queries(const char *filename)
{
    /* ====STATISTICS============================================================= */

    int stats_size = get_size_stats();
    stats *statistics = (stats*)malloc(stats_size);
    init_stats(statistics);

    /* ====USERS================================================================== */

    int user_cat_size = get_user_cat_size();
    user_cat *users = (user_cat*)malloc(user_cat_size);

    /* ====COMMITS================================================================ */

    int commit_cat_size = get_commit_cat_size();
    commit_cat *commits = (commit_cat*)malloc(commit_cat_size);

    /* ====REPOS================================================================ */

    int repos_cat_size = get_repos_cat_size();
    repo_cat *repos = (repo_cat*)malloc(repos_cat_size);

    /* ====BUILDING================================================================ */

    printf("====BUILDING CATALOGS=======================\n");
    bool done = generate_catalogs(users, commits, repos, statistics);

    int total_colabs = compute_total_colabs(users, commits);
    update_total_colabs(statistics, total_colabs);
    
    if (!done)
    {
        printf("====FAILED==================================\n");
        exit(EXIT_FAILURE);
    }
    else printf("====SUCCESS=================================\n\n");

    printf("====STARTING THE QUERIES====================\n");

    printf("(read_queries)\n");

    char *buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));

    if (file_exists(filename) && check_ext(filename, ".txt"))
    {

        printf("    > Opening query file... ");
        
        FILE *qfile = fopen(filename, "r");
        int query_num;

        if (!qfile || !buffer) 
        {
            fprintf(stderr, "Failed to open the query file, exiting.\n");
            return false;
        }

        printf("Done!\n");
        printf("    > Reading and executing queries...\n");

        QUERY *q = (QUERY*)malloc(sizeof(QUERY)); 
        int counter = 1;

        while(fgets(buffer, BUFFER_SIZE, qfile))
        {
            char *rest = buffer;
            (rest)[strcspn((rest), "\n")] = 0;

            char *id_str = strtok_r(rest, " ", &rest);
            query_num = atoi(id_str);

            if (query_num <= 10)
            {
                // if (validate_params(strdup(rest), query_num))
                // {
                //     q->query_id = query_num; q->param = strdup(rest); q->number = counter;
                //     counter++;
                //     if (!execute(q, users, commits, repos, statistics)) return false;
                // }
                
                q->query_id = query_num; q->param = strdup(rest); q->number = counter;
                counter++;
                if (!execute(q, users, commits, repos, statistics)) return false;
            }
            else printf("    > Invalid query id! Skipping...\n");
        }

        printf("===SUCCESS===================================\n\n");

        free(q); fclose(qfile);
        destroy_user_catalog(users);
        free(statistics); free(commits); free(repos);

        return true;
    }

    free(buffer);
    destroy_user_catalog(users);
    destroy_repo_catalog(repos);
    free(statistics); free(commits);

    printf("    > Failed to detect a query file!\n");
    return false;
}