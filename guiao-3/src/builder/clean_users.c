#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <glib.h>

#include "../../include/builder/clean_users.h"
#include "../../include/ui_helpers.h"

/**
 * @brief Get the first number from a csv string.
 * @param string csv string.
 * @return int number retrieved.
 */
int get_number(char *string)
{
    char* num_string = strdup(strsep(&string, ";"));
    return atoi(num_string);
}

/**
 * @brief Get the second number from a csv string.
 * @param string csv string.
 * @return int number retrieved.
 */
int get_second_number(char *string)
{
    strsep(&string, ";");
    char* num_string = strdup(strsep(&string, ";"));
    return atoi(num_string);
}

/**
 * @brief Copies both every id from the file into an array and lines from users-ok.csv into users-final.csv.
 * @param filename Name of the file (users-ok.csv).
 * @param values The array we will be storing the data.
 * @param size The array size, which is equal to the number of lines in the file given.
 */
GHashTable *clean_users(char const *filename, int *storage)
{
    FILE *user_file = fopen(filename, "r");
    FILE *user_final = fopen("./saida/users-final.csv", "w");

    if (!user_file || !user_final) 
    {
        fprintf(stderr, "Couldn't open file(s).\n'");
        return NULL;
    }

    char* buffer = malloc(sizeof(char) * MAX_SIZE);
    char* keep = malloc(sizeof(char) * MAX_SIZE);

    if (!buffer || !keep)
    {
        fprintf(stderr, "Out of memory.\n'");
        return NULL;
    }

    char* USER_HEADER = fgets(buffer, MAX_SIZE, user_file);
    fputs(USER_HEADER, user_final);

    GHashTable *user_table = g_hash_table_new(g_int_hash, g_int_equal);
    int *num_to_add = (int*)malloc(sizeof(int));

    int i = 0;

    while(fgets(buffer, MAX_SIZE, user_file))
    {
        keep = strcpy(keep, buffer);
        fputs(keep, user_final);

        *num_to_add = get_number(buffer); 

        storage[i] = *num_to_add;
        g_hash_table_add(user_table, &storage[i]);

        i++;
    }

    free(buffer); free(keep); free(num_to_add);
    fclose(user_file);

    return user_table;
}   