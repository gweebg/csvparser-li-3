#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../include/ui_helpers.h"
#include "../include/querier/utils.h"
#include "../include/builder/fetch_tools.h"

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

void print_boxed_bold(char *content)
{
    char *tok = strtok(strdup(content), "\n");
    int max_length = 0; int current = 0;

    while(tok != NULL) 
    {
        current = strlen(tok);

        if (max_length < current) max_length = current;
        tok = strtok(NULL, "\n");
    }

    max_length++;

    printf("┏");
    for (int i = 0; i < max_length; i++) printf("━");
    printf("┓\n");

    char *token = strtok(strdup(content), "\n");
    int aux = 0;

    while(token != NULL) 
    {
        printf("┃ ");
        printf("%s", token);

        int white_space = max_length - strlen(token);

        if (aux == 0)
            for (int j = 0; j < white_space - 1; j++) putchar(' ');
        else
            for (int j = 0; j < white_space; j++) putchar(' ');

        printf("┃\n");

        token = strtok(NULL, "\n");
        aux++;	
    }

    printf("┗");
    for (int i = 0; i < max_length; i++) printf("━");
    printf("┛");
}

void print_list_item(char *list_number, char *content)
{
    print_boxed(list_number);
    printf("\033[%dA\033[%dL", 0, -1);
    printf(" %s\n", content);
    printf("\033[%dB", 1);
}

void print_list_item_non(char *list_number, char *content)
{
    print_boxed(list_number);
    printf("\033[%dA\033[%dL", 0, -1);
    printf(" %s", content);
    printf("\033[%dB", 1);
}

void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

void update_bar(int i, int bar_length, int max_items)
{
    // if (i % 2 == 0) return;
    int num_chars = (i * bar_length) / max_items;
    for(int j = 0; (j < num_chars); j++) printf("▓");
}

void cursor(bool show)
{
    #define CSI "\e["
    if (show) {
        fputs(CSI "?25h", stdout);
    }
    else {
        fputs(CSI "?25l", stdout);
    }
    #undef CSI
}

void pag_output(char *filename, int threshold, int above)
{
    int lines = count_lines(filename), current_line = 0;
    char *lines_array[lines];

    FILE *file = fopen(filename, "r");
    if (!file) { printf("Ficheiro não encontrado!\n"); exit(EXIT_FAILURE); }

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, file);

    while (line_size >= 0)
    {
        lines_array[current_line++] = strdup(line_buffer);        
        line_size = getline(&line_buffer, &line_buffer_size, file);
    }

    bool has_quit = false;
    int current_page = 1, lines_p = 5;

    double pages = (double)lines / (double)threshold;
    int limit = 0; int mark = 0;

    while(!has_quit)
    {
        printf(" ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Resultados 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n\n");

        for (int u = mark; u < lines; u++)
        {
            printf(" %s", lines_array[u]); 
            limit++; lines_p++;
            if (limit == threshold) break;
        }

        if (limit != 15)
            for (int y = 0; y < threshold - limit; y++) 
                printf("\n");

        limit = 0;

        printf("\n ┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Página %d/%d 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n\n", current_page, (int)ceil(pages));
        printf(" 🮤 Opção 🮥 ");

        char op[2];
        if (fgets(op, sizeof(op), stdin))
        {
            if (op[0] == 'q') has_quit = true;
            if (op[0] == 'n')
            {
                if (current_page < (int)ceil(pages)) { current_page++; mark += threshold; }
                else { current_page = 1; mark = 0; }
            }
            if (op[0] == 'b' && ((int)ceil(pages) != 1))
            {
                if (current_page > 1) { current_page--; mark -= threshold; }
                else { current_page = (int)ceil(pages); mark = (int)pages * threshold - threshold; } 
            }

            printf("\033[%d;%dH", above, 0);
            clear_line();
            for (int i = 0; i < lines_p + threshold; i++)
            {
                printf("\n");
                clear_line();
            }

            lines_p = 0;
            printf("\033[%d;%dH", above, 0);
        }

    }

    for (int i = 0; i < lines; i++) 
        free(lines_array[i]);

    fclose(file);
}

void line_up()
{
    printf("\033[A");
}

void clear_line()
{
    printf("\33[2K\r");
}

void get_num(char *context, int *number)
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

bool is_lang(char *lang)
{
    (lang)[strcspn(lang, "\n")] = 0;

    FILE* lang_file = fopen("./docs/programming_languages.txt", "r");
    if (!lang_file) 
    {
        clear_screen();
        print_boxed("Nao foi encontrado uma lista com as linguagens disponiveis.\nFix: Criar um ficheiro 'programming_languages.txt' em docs/ e popula-lo com as linguagens em cada linha.");
        printf("\n");
        exit(EXIT_FAILURE);
    }

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, lang_file);

    while (line_size >= 0)
    {
        (line_buffer)[strcspn(line_buffer, "\n")] = 0;
        if (strcmp(lang, line_buffer) == 0) return true;
        line_size = getline(&line_buffer, &line_buffer_size, lang_file);
    }

    fclose(lang_file);
    return false;
}

void get_lang(char *context, char **lang)
{
    char lang1[256];
    bool valid = false;
    while(!valid)
    {
        printf("%s", context);
        if (fgets(lang1, sizeof(lang1), stdin))
        {
            if (is_lang(lang1)) 
            {
                *lang = strdup(lang1);
                (*lang)[strcspn(*lang, "\r\n")] = 0;
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