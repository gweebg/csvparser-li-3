#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <unistd.h>

#include "../include/user_interface.h"
#include "../include/ui_helpers.h"
#include "../include/builder/builder.h"
#include "../include/querier/querier.h"

#include "../include/querier/commits.h"
#include "../include/querier/repos.h"
#include "../include/querier/users.h"


bool is_present(char *what, char **where, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(what, where[i]) == 0) return true;
    }

    return false;
}

file_state available_files(char *input_path)
{
    DIR *pwd;
    struct dirent *dir;

    char *raw_files[] = {"users.csv", "commits.csv", "repos.csv"};
    char *ok_files[] = {"users-ok.csv", "commits-ok.csv", "repos-ok.csv"};
    char *final_files[] = {"users-final.csv", "commits-final.csv", "repos-final.csv"};

    pwd = opendir(input_path);
    int raw = 0, ok = 0, final = 0, total = 0;

    if (pwd)
    {
        while((dir = readdir(pwd)) != NULL)
        {
            // printf("%s ; ", dir->d_name);
            total++;

            if (is_present(dir->d_name, raw_files, 3)) raw++;
            else if (is_present(dir->d_name, ok_files, 3)) ok++;
            else if (is_present(dir->d_name, final_files, 3)) final++;
        }
        closedir(pwd);
    }

    file_state state = EMPTY;

    if (total >= 3)
    {
        if (final == 3) state = FINAL;
        else if (ok == 3) state = OK;
        else if (raw == 3) state = RAW;
    }
    
    return state;
}

void build_files(file_state state)
{
    switch(state)
    {
        case EMPTY:
            print_boxed("Erro!\nNao foram encontrados ficheiros de input!\n");
            printf("\n");

            exit(EXIT_FAILURE);
            break;

        case RAW:
            build_from_raw();
            break;

        case OK:
            build_from_ok();
            break;

        case FINAL:
            build_from_final();
            break;

        default:
            print_boxed("Erro!\nSwitch case para default em 'build_files' @ user_interface.c!\n");
            printf("\n");

            exit(EXIT_FAILURE);
            break;    
    }
}

void print_queries(int page)
{
    switch(page)
    {
        case 1:
            printf("\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Queries 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
            print_list_item("1", "Quantidade de bots, organizações e utilizadores.");
            print_list_item("2", "Numero médio de colaboradores por repositório.");
            print_list_item("3", "Quantidade de repositórios com bots.");
            print_list_item("4", "Quantidade média de commits por utilizador.");
            printf("┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Página 1/3 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
            break;
        case 2:
            printf("\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Queries 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
            print_list_item("5", "Top N de utilizadores mais ativos num determinado intervalo de datas.");
            print_list_item("6", "Top N de utilizadores com mais commits em repositórios de uma linguagem.");
            print_list_item("7", "Repositórios inativos a partir de uma data.");
            print_list_item("8", "Top N de linguagens mais utilizadas a partir de uma determinada data.");
            printf("┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Página 2/3 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
            break;
        case 3:
            printf("\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Queries 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
            print_list_item("9", "Top N de utilizadores com mais commits em repositorios cujo owner e um amigo seu.");
            print_list_item("10", "Top N de utilizadores com as maiores mensagens de commit por repositorio.");
            print_list_item("??", "Vazio");
            print_list_item("??", "Vazio");
            printf("┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Página 3/3 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
            break;
        default:
            break;
    }
    
}

void print_help_menu()
{
    printf("┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈🮤 Menu De Ajuda 🮥┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
    printf("\n");
    print_list_item("h", "Mostra este menu.");
    print_list_item("n", "Mostra a próxima página das queries disponíveis.");
    print_list_item("b", "Mostra a página anterior das queries disponíveis.");
    print_list_item("q", "Volta do menu/sai do programa.");
    printf("\n┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈┈\n");
}

void interface()
{
    /* Gerar os catálogos */
    stats *statistics = xmalloc(get_size_stats());
    user_cat *users = xmalloc(get_user_size());
    commit_cat *commits = xmalloc(get_commit_size());
    repo_cat *repos = xmalloc(get_repo_size());
    
    store(users, commits, repos, statistics); sleep(1);

    /* Iniciar a interface */
    bool interface_state = true;
    int current_page = 1; int id = 0;

    while (interface_state)
    {
        clear_screen(); 
        print_boxed("Bem-vindo ao Projeto de LI3!\nPara comecar escolha a query a ser executada, ou escreva 'h' para abrir o menu de ajuda.\n");
        printf("\n");
        print_queries(current_page);
        printf("\n🮤 Opcao 🮥 ");

        char query_number[256];
        if (fgets(query_number, sizeof(query_number), stdin))
        {
            query_number[strcspn(query_number, "\n")] = 0;
            char option = query_number[0];

            if (option == 'h')
            {
                clear_screen();
                print_help_menu();
                cursor(false);
                printf(GRN "\n                  🮤 ENTER para voltar 🮥 " RESET);
                if (fgets(query_number, sizeof(query_number), stdin)) clear_screen();
                cursor(true);
                
            }
            else if (option == 'q')
            {
                clear_screen();
                exit(EXIT_SUCCESS);
            }
            else if (option == 'n')
            {
                clear_screen();
                if (current_page == 3) current_page = 1;
                else current_page++;
            }
            else if (option == 'b')
            {
                clear_screen();
                if (current_page == 1) current_page = 3;
                else current_page--;
            }
            else if (atoi(query_number))
            {
                clear_screen();
                query_handler(users,
                              commits,
                              repos,
                              statistics, 
                              atoi(query_number), 
                              id);
                id++;
            }
            // else
            // {
            //     cursor(false);
            //     printf("\033[%dA\033[%dL", 0, -1);
            //     printf(RED "🮤 Opcao Invalida! 🮥\n" RESET);
            //     printf("\033[%dB", 1);
            //     usleep(5000);
            //     cursor(true);
            // }
        }

        fflush(stdin);
    }

}