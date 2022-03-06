#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define __USE_XOPEN
#define _GNU_SOURCE

void print_boxed(char *content)
{
    char *tok = strtok(strdup(content), "\n");
    int max_length = 0; int current = 0;

    while(tok != NULL) 
    {
        current = strlen(tok);

        if (max_length < current) max_length = current;
        tok = strtok(NULL, "\n");
    }

    printf("╭");
    for (int i = 0; i < max_length + 2; i++) printf("─");
    printf("╮\n");

    char *token = strtok(strdup(content), "\n");
    int aux = 0;

    while(token != NULL) 
    {
        printf("│ ");
        printf("%s", token);

        int white_space = max_length - strlen(token);

        if (aux == 0)
            for (int j = 0; j < white_space + 1; j++) putchar(' ');
        else
            for (int j = 0; j < white_space + 1; j++) putchar(' ');

        printf("│\n");

        token = strtok(NULL, "\n");
        aux++;	
    }

    printf("╰");
    for (int i = 0; i < max_length + 2; i++) printf("─");
    printf("╯");
}

void line_up()
{
    printf("\033[A"); // up
}

void clear_line()
{
    printf("\33[2K\r"); // clear line
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

void get_number(char *context, int *number)
{
    char num[256];
    bool valid = false;
    while(!valid)
    {
        printf("%s", context);
        if (fgets(num, sizeof(num), stdin))
        {
            if (atoi(num) > 0) 
            {
                *number = atoi(num);
                valid = true;
            }
            else
            {
                line_up(); clear_line();
                valid = false;
            }
        }
    }

    fflush(stdin);
    printf("\n");
}

void get_date(char *context, char **date)
{
    char date1[256];
    bool valid = false;
    while(!valid)
    {
        printf("%s", context);
        if (fgets(date1, sizeof(date1), stdin))
        {
            if (is_date(date1) > 0) 
            {
                *date = strdup(date1);
                (*date)[strcspn(*date, "\r\n")] = 0;
                valid = true;
            }
            else
            {
                line_up(); clear_line();
                valid = false;
            }
        }
    }
    fflush(stdin);
    printf("\n");
}

int main()
{

    print_boxed("Query 5:\nTop N de utilizadores mais ativos num determinado intervalo de datas.");
    printf("\n\n");

    char num[256]; int *number = malloc(sizeof(int));
    char *date1;
    char *date2;

    printf("🮤 Introduza os Parametros 🮥\n\n");

    get_number("    Número desejado de utilizadores (N, inteiro) 🮥 ", number);
    get_date("    Data de início (YYYY-MM-DD) 🮥 ", &date1);
    get_date("    Data de fim (YYYY-MM-DD) 🮥 ", &date2);

    printf("%d - %s - %s\n", number, date1, date2);

}
