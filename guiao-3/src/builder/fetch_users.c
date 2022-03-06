#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "../../include/builder/fetch_users.h"
#include "../../include/builder/fetch_tools.h"
#include "../../include/ui_helpers.h"

bool check_user_line(char *csv_line)
{
    /* id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos */
    /* int,str,type,date,int,array,int,array,int,int,int */
    bool validity = true;

    check_number(&csv_line, NULL, &validity);
    check_string(&csv_line, false, &validity);
    check_type(&csv_line, &validity);
    check_date(&csv_line, &validity);

    long *followers = malloc(sizeof(long));
    *followers = -1;
    check_number(&csv_line, followers, &validity);

    long *actual_followers = malloc(sizeof(long));
    *actual_followers = 0;
    check_array(&csv_line, actual_followers, &validity);

    long *following = malloc(sizeof(long));
    *following = -1;
    check_number(&csv_line, following, &validity);

    long *actual_following = malloc(sizeof(long));
    *actual_following = 0;
    check_array(&csv_line, actual_following, &validity);

    compare_f(*followers, *actual_followers, *following, *actual_following, &validity);

    check_number(&csv_line, NULL, &validity);
    if (!is_number(csv_line)) validity = false;
    
    free(followers); free(following); free(actual_followers); free(actual_following);
    return validity;
}


bool build_users(const char *filename)
{
    // printf("(build_users)\n");
    // printf("    > Opening file %s... ", filename);
    // print_boxed("Users ");

    FILE *ufile = fopen(filename, "r");
    FILE *out = fopen("./saida/users-ok.csv", "w");

    if (!ufile || !out)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    // printf("Done!\n");
    // printf("    > Reading %s... ", filename);

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, ufile); // Skips the header line.
    fputs(line_buffer, out);
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, ufile);     

    int valid = 0, invalid = 0, current_line = 2;

    // printf("\033[%dA\033[%dL", 0, -2);
    while (line_size >= 0)
    {
        char *line_dup = strdup(line_buffer);
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Remove o \n ou \r\n

        if (check_user_line(line_buffer)) 
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
        line_size = getline(&line_buffer, &line_buffer_size, ufile);
    }
    printf(GRN "✔\n" RESET);

    // printf(" ✔\n");
    // printf("\033[%dB", 1);

    // printf("Done!\n");
    // printf("    > Data stored successfully!\n");
    // printf("    > Total invalid : %d\n", invalid);

    fclose(ufile); fclose(out);
    if (line_buffer) free(line_buffer);

    return true;
}