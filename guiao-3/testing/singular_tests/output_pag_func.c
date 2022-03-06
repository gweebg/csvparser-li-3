#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

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

int count_lines(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) return -1;

    int lines = 0;

    while(!feof(file))
    {
        char c = fgetc(file);
        if(c == '\n') lines++;
    }

    return lines;
}

void pag_output(char *filename)
{
    int threshold = 15, current_line = 1;
    FILE *file = fopen(filename, "r");
    if (!file) { printf("Ficheiro não encontrado!\n"); exit(EXIT_FAILURE); }

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, file);     

    int total_lines = count_lines(filename);
    double pages = (double)total_lines / (double)threshold;
    int current_page = 1;
    int p = (int)ceil(pages);
    char *last_printed[p];

    printf("\e[1;1H\e[2J");
    for (int i = 0; i < p; i++)
    {
        printf("\e[1;1H\e[2J");
        printf("====Resultados====================================\n\n");
        while (line_size >= 0 && current_line <= threshold)
        {
            line_buffer[strcspn(line_buffer, "\r\n")] = 0;
            if (current_line == 1) {last_printed[i] = strdup(line_buffer);}
            printf("%s\n", line_buffer);
            line_size = getline(&line_buffer, &line_buffer_size, file);
            current_line++;
        }
        current_line = 1;

        printf("\n====Página %d/%d==================================\n\n", (current_page), ((int)pages+1));
        printf("Próxima página : n | Salvar como : s | Voltar : q\n");
        printf("Opção : ");
        char op[256];
        if (fgets(op, sizeof(op), stdin))
        {
            if (op[0] == 'n') 
            { 
                printf("\e[1;1H\e[2J"); 
                if (current_page < 3) current_page++; 
                else current_page = 1;
            }
            if (op[0] == 'b') 
            { 
                printf("\e[1;1H\e[2J"); 
                if (current_page > 1) current_page--;
                else current_page = 3;
                printf(">>>>>>>>>> %s\n", last_printed[current_page]);
                sleep(10);
                p++;
            }
            
        }

    }

    fclose(file);
}

void clear_screen()
{
    printf("\e[1;1H\e[2J"); 
}

void line_up()
{
    printf("\033[A");
}

void clear_line()
{
    printf("\33[2K\r");
}

void pag_output_v2(char *filename, int threshold)
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
    int current_page = 1;

    double pages = (double)lines / (double)threshold;
    int p = (int)ceil(pages); int limit = 0; int mark = 0; 

    while(!has_quit)
    {
        printf("---- Resultados -----------------------------------------\n\n");

        for (int u = mark; u < lines; u++)
        {
            printf("%s", lines_array[u]); 
            limit++;
            if (limit == threshold) break;
        }

        if (limit != 15)
            for (int y = 0; y < threshold - limit; y++) 
                printf("\n");

        limit = 0;

        printf("\n---- Página %d/%d -----------------------------------------\n\n", current_page, (int)ceil(pages));
        printf("Próxima página : n | Página anterior : b | Voltar : q\n");
        printf("Opção : ");

        char op[2];
        if (fgets(op, sizeof(op), stdin))
        {
            clear_screen();
            if (op[0] == 'q') has_quit = true;
            if (op[0] == 'n')
            {
                if (current_page < (int)ceil(pages)) { current_page++; mark += threshold; }
                else { current_page = 1; mark = 0; }
            }
            if (op[0] == 'b')
            {
                if (current_page > 1) { current_page--; mark-=threshold; }
                else { current_page = (int)ceil(pages); mark = (int)pages * threshold;} 
            }
            
        }

    }

    for (int i = 0; i < lines; i++) 
        free(lines_array[i]);

    fclose(file);
}

void pag_output_v3(char *filename, int threshold, int above)
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

int main()
{
    clear_screen();
    print_boxed("Texto ilustrativo\nOla adeus ola eu amo a babi!!!");
    printf("\n\n");
    pag_output_v3("test_file.txt", 5, 6);
}

// clear_line();
// for (int i = 0; i < lines_p; i++)
// {
    // line_up();
    // clear_line();
// }