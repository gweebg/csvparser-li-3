#define __USE_XOPEN
#define _GNU_SOURCE

#define MAX_SIZE 480000 // 200000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../includes/ex1/fetch_users.h"
#include "../includes/ex1/fetch_commits.h"

long int fetch_number(char **buffer, User *u)
{
    (*buffer)[strcspn((*buffer), "\t\r\n")] = 0;

    char *endptr = NULL;
    char *num_str = strdup(strsep(buffer, ";"));

    long int num = strtol(num_str, &endptr, 10);

    if (num < 0 || (num_str[0] == '\0') || *endptr != '\0')
    {
        u->valid = false;
        return -1;
    }

    free(num_str);
    return num;
}

char* fetch_text(char **buffer, User *u)
{
    char* list_str = (char*)malloc(sizeof(char) * MAX_SIZE);
    strcpy(list_str, strsep(buffer, ";"));

    if (list_str[0] == '\0') 
    { 
        u->valid = false; 
        return "NONE"; 
    }
    
    return list_str;
}

enum Type fetch_type(char **buffer, User *u)
{
    enum Type type;
    char* type_str = strdup(strsep(buffer, ";"));

    if (strcmp(type_str, "Bot") == 0) type = BOT;
    else if (strcmp(type_str, "Organization") == 0) type = ORG;
    else if (strcmp(type_str, "User") == 0) type = USR;
    else
    {
        u->valid = false;
        type = NAT;
    }

    free(type_str);
    return type;
}

bool verify_date(int year, int month, int day)
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

void fetch_date(char **buffer, struct tm *ct, User *u)
{
    char* string = strdup(strsep(buffer, ";"));

    int year, month, day, hour, min, sec;
    char tz_sym;

    sscanf(string, 
           "%4d-%2d-%2d %2d:%2d:%2d%c", 
           &year, &month, &day, &hour, &min, &sec, &tz_sym);

    if (tz_sym == '+' || tz_sym == '-' || tz_sym == '.') { free(string); u->valid = false; return; }

    strptime(string, "%Y-%m-%d %H:%M:%S", ct);

    ct->tm_year += 1900;
    ct->tm_mon += 1;

    if (year != ct->tm_year || month != ct->tm_mon || day != ct->tm_mday ||
        hour != ct->tm_hour || min != ct->tm_min   || sec != ct->tm_sec   )
        { u->valid = false; free(string); return; }

    if (!verify_date(ct->tm_year, ct->tm_mon, ct->tm_mday)) { u->valid = false; free(string); return; }
}

void remove_first_last(const char *string, char *result)
{
    int len = strlen(string);
    if(len > 0) strcpy(result, ++string);
    if(len > 1) result[len - 2] = '\0';
}

int* string_to_array(char* string, User *user, int *count)
{
    /* Recebe uma string do tipo '[1,2,3,4]' e converte num array de inteiros */
    *count = -1;

    /* Verifica se existe string */
    if (strcmp(string, "") == 0) 
    { 
        user->valid = false; 
        return NULL; 
    }

    /* Verificar se a string começa e acaba com parênteses retos. */
    if (string[0] != '[' || string[strlen(string) - 1] != ']') 
    { 
        user->valid = false; 
        return NULL;
    }

    /* Remover os parênteses da string para poder retirar os números */
    char new_string[strlen(string) - 2];
    remove_first_last(string, new_string);

    /* Verificar o contéudo da string */
    char *final_string = (char*)malloc(sizeof(char) * 200000);
    strcpy(final_string, new_string);

    int* final_array = NULL;

    if (strcmp(final_string, "") == 0) 
    {
        *count = 0;
        return final_array; 
    }
    
    /* Extrair o contéudo da string */
    char *buffer;
    char *endptr;
    
    int i = 0;

    while(((buffer = strsep(&final_string, ","))) != NULL)
    {
        final_array = realloc(final_array, sizeof(int) * MAX_SIZE);
        int number = strtol(buffer, &endptr, 10);

        if (number > 0 && *endptr == '\0')
        {
            final_array[i] = number;

            i++;
        }
        else 
        {
            user->valid = false;
            return NULL;
        }

    }

    *count = i;

    free(final_string);
    return final_array;
}

User init_user(char* buffer)
{
    User u;
    struct tm t = {0};

    u.valid = true;
    u.follower_count = -1; u.following_count = -1;

    u.id = fetch_number(&buffer, &u);
    u.user_name = fetch_text(&buffer, &u);
    u.type = fetch_type(&buffer, &u);

    fetch_date(&buffer, &t, &u);
    u.created_at = t;

    u.followers = fetch_number(&buffer, &u);

    char *tmp = fetch_text(&buffer, &u); int c1;
    u.follower_list = string_to_array(tmp, &u, &c1);

    u.follower_count = c1;

    u.following = fetch_number(&buffer, &u);

    char* tmp_2 = fetch_text(&buffer, &u); int c2;
    u.following_list = string_to_array(tmp_2, &u, &c2);

    u.following_count = c2;

    u.gists = fetch_number(&buffer, &u);
    u.public_repo = fetch_number(&buffer, &u);

    return u;

}

bool valid(User user)
{
    if ((user.valid) && (user.follower_count == user.followers) && (user.following_count == user.following)) return true;
    else return false;
}

bool fusers(char* filename)
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
    FILE *output_file = fopen("./saida/users-ok.csv", "w");

    if (!file_pointer) { fprintf(stderr, "Could not open %s!\n", filename); return false; }
    if (!output_file)  { fprintf(stderr, "Could not open ./saida/users-ok.csv !\n"); return false; }

    printf("Opened file: %s\n",filename);

    /**
     * @brief Max line size on the file. Might have to change it some other time.
     */
    char* buffer = (char*)malloc(sizeof(char) * MAX_SIZE);
    char* keep = (char*)malloc(sizeof(char) * MAX_SIZE);

    int i = 0, not_valid = 0;

    /**
     * @brief Copypasting the file header to the output file.
     */
    char* USER_HEADER = fgets(buffer, MAX_SIZE, file_pointer);
    fputs(USER_HEADER, output_file);

    printf("> Reading data ...\n");

    /**
     * @brief Looping through the file and parsing the data.
     */
    while(fgets(buffer, MAX_SIZE, file_pointer))
    {
        keep = strcpy(keep, buffer);

        User user = init_user(buffer);

        if (valid(user)) { fputs(keep, output_file); }
        else { not_valid++; }

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