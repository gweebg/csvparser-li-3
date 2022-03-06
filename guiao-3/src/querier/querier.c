#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/querier/query_parser.h"
#include "../../include/querier/querier.h"
#include "../../include/querier/execute.h"
#include "../../include/querier/utils.h"

#include "../../include/querier/repo_store.h"
#include "../../include/querier/statistics.h"
#include "../../include/querier/user_store.h"
#include "../../include/querier/commit_store.h"
#include "../../include/querier/statistics.h"

#include "../../include/ui_helpers.h"
#include "../../include/builder/fetch_tools.h"

void exec_1(stats *statistics, int id)
{
    print_boxed("Query 1:\nQuantidade de bots, organizacoes e utilizadores.");
    printf("\n\n");

    qtty_usr_bot_org(statistics, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 4, 6);
    free(output_name);
}

void exec_2(stats *statistics, user_cat *users, commit_cat *commits, int id)
{
    print_boxed("Query 2:\nNumero medio de colaboradores por repositorio.");
    printf("\n\n");

    avg_colab_per_repo(statistics, users, commits, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 2, 6);
    free(output_name);
}

void exec_3(stats *statistics, int id)
{
    print_boxed("Query 3:\nQuantidade de repositorios com bots.");
    printf("\n\n");

    repos_with_bots(statistics, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 2, 6);
    free(output_name);
}

void exec_4(stats *statistics, int id)
{
    print_boxed("Query 4:\nQuantidade media de commits por utilizador.");
    printf("\n\n");

    avg_commits_per_usr(statistics, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 2, 6);
    free(output_name);
}

void exec_5(user_cat *users, commit_cat *commits, int id)
{
    print_boxed("Query 5:\nTop N de utilizadores mais ativos num determinado intervalo de datas.");
    printf("\n\n");

    int *number = malloc(sizeof(int));
    char *date1; char *date2;

    printf("🮤 Introduza os Parametros 🮥\n\n");

    get_num("    Número desejado de utilizadores (N, inteiro) 🮥 ", number);
    get_date("    Data de início (YYYY-MM-DD) 🮥 ", &date1);
    get_date("    Data de fim (YYYY-MM-DD) 🮥 ", &date2);

    char *param = malloc(sizeof(char) * 64);
    sprintf(param, "%d %s %s", *number, date1, date2);

    commit_leaderboard(users, commits, param, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 5, 14); 
    free(output_name); free(param);
}

void exec_6(user_cat *users, commit_cat *commits, repo_cat *repos, int id)
{
    print_boxed("Query 6:\nTop N de utilizadores com mais commits em repositorios de uma linguagem.");
    printf("\n\n");

    int *number = malloc(sizeof(int));
    char *lang;

    printf("🮤 Introduza os Parametros 🮥\n\n");

    get_num("    Número desejado de utilizadores (N, inteiro) 🮥 ", number);
    get_lang("    Linguagem 🮥 ", &lang);

    char *param = malloc(sizeof(char) * 64);
    sprintf(param, "%d %s", *number, lang);

    commit_leaderboard_lang(users, commits, repos, param, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 5, 12); 
    free(output_name); free(param);
}

void exec_7(repo_cat *repos, commit_cat *commits, int id)
{
    print_boxed("Query 7:\nRepositorios inativos a partir de uma data.");
    printf("\n\n");

    char *date;

    printf("🮤 Introduza os Parametros 🮥\n\n");

    get_date("    Data de início (YYYY-MM-DD) 🮥 ", &date);

    char *param = malloc(sizeof(char) * 64);
    sprintf(param, "%s", date);

    unactive_repos(commits, repos, param, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 10, 10); 
    free(output_name); free(param);
}

void exec_8(repo_cat *repos, int id)
{
    print_boxed("Query 8:\nTop N de linguagens mais utilizadas a partir de uma determinada data.");
    printf("\n\n");

    int *number = malloc(sizeof(int));
    char *date;

    printf("🮤 Introduza os Parametros 🮥\n\n");
    get_num("    Número desejado de utilizadores (N, inteiro) 🮥 ", number);
    get_date("    Data de início (YYYY-MM-DD) 🮥 ", &date);

    char *param = malloc(sizeof(char) * 64);
    sprintf(param, "%d %s", *number, date);

    top_languages(repos, param, id);

    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 10, 10); 
    free(output_name); free(param);
}

void exec_9(user_cat *users, commit_cat *commits, repo_cat *repos, int id)
{
    print_boxed("Query 9:\nTop N de utilizadores com mais commits em repositorios cujo owner e um amigo seu.");
    printf("\n\n");

    int *number = malloc(sizeof(int));

    printf("🮤 Introduza os Parametros 🮥\n\n");
    get_num("    Número desejado de utilizadores (N, inteiro) 🮥 ", number);

    char *param = malloc(sizeof(char) * 64);
    sprintf(param, "%d", *number);

    top_commits_friends(users, commits, repos, param, id);
    
    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 10, 10); 
    free(output_name); free(param);
}

void exec_10(user_cat *users, commit_cat *commits, repo_cat *repos, int id)
{
    print_boxed("Query 9:\nTop N de utilizadores com mais commits em repositorios cujo owner e um amigo seu.");
    printf("\n\n");

    int *number = malloc(sizeof(int));

    printf("🮤 Introduza os Parametros 🮥\n\n");
    get_num("    Número desejado de utilizadores (N, inteiro) 🮥 ", number);

    char *param = malloc(sizeof(char) * 64);
    sprintf(param, "%d", *number);

    top_user_repo_message(users, commits, repos, param, id);
    
    char* output_name = malloc(sizeof(char) * 30);
    sprintf(output_name, "./saida/command%d_output.txt", id);

    pag_output(output_name, 10, 10); 
    free(output_name); free(param);
}

void query_handler(user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics, int query_id, int id)
{
    if (query_id == 1) exec_1(statistics, id);
    if (query_id == 2) exec_2(statistics, users, commits, id);
    if (query_id == 3) exec_3(statistics, id);
    if (query_id == 4) exec_4(statistics, id);
    if (query_id == 5) exec_5(users, commits, id);
    if (query_id == 6) exec_6(users, commits, repos, id);
    if (query_id == 7) exec_7(repos, commits, id);
    if (query_id == 8) exec_8(repos, id);
    if (query_id == 9) exec_9(users, commits, repos, id);
    if (query_id == 10) exec_10(users, commits, repos, id);
}

void store(user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics)
{
    cursor(false);
    printf(" A gerar os catálogos... ");

    init_stats(statistics);
    store_users(users, statistics, USERS);
    store_commits(commits, statistics, COMMITS);
    store_repos(repos, statistics, REPOS);

    printf(GRN "✔\n" RESET);
    cursor(true);
}