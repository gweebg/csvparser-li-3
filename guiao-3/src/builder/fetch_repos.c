#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "../../include/builder/fetch_repos.h"
#include "../../include/builder/fetch_tools.h"
#include "../../include/ui_helpers.h"

bool check_repo_line(char *csv_line)
{
    /* id;owner_id;full_name;license;has_wiki;description;language;default_branch;created_at;updated_at;forks_count;open_issues;stargazers_count;size */
    /* int,int,string,string,bool,string,string,string,date,date,int,int,int,int */
    bool validity = true;

    check_number(&csv_line, NULL, &validity); // id
    check_number(&csv_line, NULL, &validity); // owner_id

    check_string(&csv_line, false, &validity); // full_name
    check_string(&csv_line, false, &validity); // license
    check_wiki(&csv_line, &validity); // wiki

    check_string(&csv_line, true, &validity); // description
    check_string(&csv_line, false, &validity); // language
    check_string(&csv_line, false, &validity); // default_branch - 206

    check_date(&csv_line, &validity); // created_at - 242 - 29 erradas
    check_date(&csv_line, &validity); // updated_at - 262

    check_number(&csv_line, NULL, &validity); // forks_count
    check_number(&csv_line, NULL, &validity); // open_issues
    check_number(&csv_line, NULL, &validity); // stargazers_count

    if (!is_number(csv_line)) validity = false; // size

    return validity;
}

bool build_repos(const char *filename)
{
    // printf("(build_repos)\n");
    // printf("    > Opening file %s... ", filename);
    // print_boxed("Repos ");

    FILE *rfile = fopen(filename, "r");
    FILE *out = fopen("./saida/repos-ok.csv", "w");

    if (!rfile || !out)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    // printf("Done!\n");
    // printf("    > Reading %s... ", filename);

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, rfile); // Skips the header line.
    fputs(line_buffer, out);
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, rfile);     

    int valid = 0, invalid = 0, current_line = 2;

    // printf("\033[%dA\033[%dL", 0, -2);
    while (line_size >= 0)
    {
        char *line_dup = strdup(line_buffer);
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Remove o \n ou \r\n

        if (check_repo_line(line_buffer)) 
        {
            fputs(line_dup, out);
            valid++;
        }       
        else 
        { 
            invalid++; 
            // printf("Inválida : %d\n", current_line); 
        }

        // printf("\r\t│");
        // update_bar(current_line, 30, lines);
        // fflush(stdout);

        free(line_dup); current_line++;
        line_size = getline(&line_buffer, &line_buffer_size, rfile);
    }
    printf(GRN "✔\n" RESET);

    // printf(" ✔\n");
    // printf("\033[%dB", 1);

    // printf("Done!\n");
    // printf("    > Data stored successfully!\n");
    // printf("    > Total invalid : %d\n", invalid);

    fclose(rfile); fclose(out);
    if (line_buffer) free(line_buffer);

    return true;
}