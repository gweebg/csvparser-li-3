#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

    max_length++;

    printf("╭");
    for (int i = 0; i < max_length; i++) printf("─");
    printf("╮\n");

    char *token = strtok(strdup(content), "\n");
    int aux = 0;

    while(token != NULL) 
    {
        printf("│ ");
        printf("%s", token);

        int white_space = max_length - strlen(token);

        if (aux == 0)
            for (int j = 0; j < white_space - 1; j++) putchar(' ');
        else
            for (int j = 0; j < white_space; j++) putchar(' ');

        printf("│\n");

        token = strtok(NULL, "\n");
        aux++;	
    }

    printf("╰");
    for (int i = 0; i < max_length; i++) printf("─");
    printf("╯");
}

void print_boxed_v2(char* content)
{
    char *tok = strtok(strdup(content), "\n");
    int max_length = 0; int current = 0;

    while(tok != NULL) 
    {
        (tok)[strcspn((tok), "\r\n\0")] = 0;
        current = strlen(tok);
        printf("%s - %d\n", tok, current);

        if (max_length < current) max_length = current;
        tok = strtok(NULL, "\n");
    }

    printf("Length : %d\n", max_length);

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
    printf("╯\n");

}

int main()
{
    print_boxed_v2("Boas pessoal!\nO meu nome e Guilherme.\nEstou a fazer o trabalho de laboratorios de informatica 3.");
}


/* 

╭────────────────────────────────────────────────────────────╮
│ Boas pessoal!                                            │
│ O meu nome e Guilherme.                                   │
│ Estou a fazer o trabalho de laboratorios de informatica 3.│
╰────────────────────────────────────────────────────────────╯

*/