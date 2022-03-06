#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "../../include/builder/fetch_commits.h"
#include "../../include/builder/fetch_tools.h"
#include "../../include/ui_helpers.h"

bool check_commit_line(char *csv_line)
{
    /* repo_id;author_id;committer_id;commit_at;message */
    /* int,int,int,date,string */
    bool validity = true;

    check_number(&csv_line, NULL, &validity);
    check_number(&csv_line, NULL, &validity);
    check_number(&csv_line, NULL, &validity);

    check_date(&csv_line, &validity);

    check_string(&csv_line, true, &validity);

    return validity;
}

bool build_commits(const char *filename)
{
    // printf("(build_commits)\n");
    // printf("    > Opening file %s... ", filename);
    // print_boxed("Commits ");

    FILE *cfile = fopen(filename, "r");
    FILE *out = fopen("./saida/commits-ok.csv", "w");

    if (!cfile || !out)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    // printf("Done!\n");
    // printf("    > Reading %s... ", filename);

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, cfile); // Skips the header line.
    fputs(line_buffer, out);
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, cfile);     

    int valid = 0, invalid = 0, current_line = 2;

    // printf("\033[%dA\033[%dL", 0, -2);
    while (line_size >= 0)
    {
        char *line_dup = strdup(line_buffer);
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Remove o \n ou \r\n

        if (check_commit_line(line_buffer)) 
        {
            fputs(line_dup, out);
            valid++;
        }       
        else 
        { 
            invalid++; 
            // printf("Inválida : %d\n", current_line); 
        }

        // printf("\r\ts │");
        // update_bar(current_line, 28, lines);
        // fflush(stdout);

        free(line_dup); current_line++;
        line_size = getline(&line_buffer, &line_buffer_size, cfile);
    }
    printf(GRN "✔\n" RESET);

    // printf(" ✔\n");
    // printf("\033[%dB", 1);

    // printf("Done!\n");
    // printf("    > Data stored successfully!\n");
    // printf("    > Total invalid : %d\n", invalid);

    fclose(cfile); fclose(out);
    if (line_buffer) free(line_buffer);

    return true;
}