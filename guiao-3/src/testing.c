#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libcpuid.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <time.h>

#include "../include/querier/users.h"
#include "../include/querier/repos.h"
#include "../include/querier/commits.h"

#include "../include/querier/user_store.h"
#include "../include/querier/repo_store.h"
#include "../include/querier/commit_store.h"

#include "../include/querier/querier.h"
#include "../include/querier/execute.h"
#include "../include/ui_helpers.h"
#include "../include/testing.h"

const char *get_platform_name() 
{
    return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
}

void print_box_titled(char *content, char *color, char *title)
{
    char *tok = strtok(strdup(content), "\n");
    int max_length = 0; int current = 0;

    while(tok != NULL) 
    {
        current = strlen(tok);

        if (max_length < current) max_length = current;
        tok = strtok(NULL, "\n");
    }

    printf("%s" "╭", color);
    for (int i = 0; i < max_length + 3 - (int)strlen(title); i++) 
    {
        if (i == 3) printf(RST "%s" "%s", title, color);
        else printf("─");
    }
    printf("╮\n" RST);

    char *token = strtok(strdup(content), "\n");
    int aux = 0;

    while(token != NULL) 
    {
        printf("%s" "│ " RST, color);
        printf("%s", token);

        int white_space = max_length - strlen(token);

        if (aux == 0)
            for (int j = 0; j < white_space + 1; j++) putchar(' ');
        else
            for (int j = 0; j < white_space + 1; j++) putchar(' ');

        printf("%s" "│\n" RST, color);

        token = strtok(NULL, "\n");
        aux++;	
    }

    printf("%s" "╰", color);
    for (int i = 0; i < max_length + 2; i++) printf("─");
    printf("╯" RST);
}

void compare_files(char *file_a, char *file_b, f_error *report)
{
    FILE *a = fopen(file_a, "r");
    FILE *b = fopen(file_b, "r");

    report->line = 0;
    report->position = 0;

    if (!a || !b)
    {
        report->files = false;
        return;
    } 

    report->files = true;
    
    char ca = getc(a), cb = getc(b);
    int pos = 0, line = 1, errors = 0;

    while (ca != EOF && cb != EOF)
    {
        pos++;
        if (ca == '\n' && cb == '\n')
        {
            line++;
            pos = 1;
        }

        if (ca != cb)
        {
            if (errors == 0)
            {
                report->line = line;
                report->position = pos;
            }

            errors++;
        }

        ca = getc(a);
        cb = getc(b);
    }

    fclose(a); fclose(b);
}

void dump_report(f_error *report)
{
    printf("──Report─────────────────\n");
    printf("File status : %d\n", report->files);
    printf("First error at line : %d\n", report->line);
    printf("In position : %d\n", report->position);
    printf("─────────────────────────\n");
}

void init_report(f_error *report)
{
    report->exec_time = 0;
}

void init_test(test *t)
{
    t->id = 0;
    t->passed_tests = 0;
    t->timeout = 0;
    t->total_tests = 0;
}

void system_info()
{
    const char* PLATFORM = get_platform_name();

    if (strcmp(PLATFORM, "windows") == 0) return;

    struct sysinfo info;
    struct cpu_raw_data_t raw;                                            
	struct cpu_id_t data;

    short status = sysinfo(&info);
	if (status < 0) 
    {
        printf("Failed to get system information!\n");
        return;
    }

    const short RAM = (info.totalram * info.mem_unit) / 1024000000;

    if (!cpuid_present() || (cpuid_get_raw_data(&raw) < 0) || (cpu_identify(&raw, &data) < 0)) 
    {                                   
		printf("Your CPU doesn't support CPUID!\n");
		return;
	}

    const char *PROCESSOR = data.brand_str;
    const short CORES = data.num_cores;
    const short LOG_CORES = data.num_logical_cpus;

    char *dump = malloc(sizeof(char) * 1024);
    sprintf(dump, "Platform: %s\nMemory: %d GB\nProcessor: %s\nCores: %d\nLogic Cores: %d\n", PLATFORM, RAM, PROCESSOR, CORES, LOG_CORES);

    print_box_titled(dump, PINK, "System"); printf("\n");

    free(dump);
}

void missing_files(int id, char *query, double time)
{
    printf("[" RED "FAIL" RST "] Test %d (%s) :: Files not found to execute the query : (%fs)\n", id, query, time);
}

void passed(int id, char *query, double time)
{
    printf("[" GREEN "PASS" RST "] Test %d :: %s (%fs)\n", id, query ,time);
}

void failed(int id, char *query, double time, int line, int position)
{
    printf("[" RED "FAIL" RST "] Test %d :: %s (%fs)\n", id, query, time);
    printf("    " RED "+ " RST "Error at line: %d\n", line);
    printf("    " RED "+ " RST "Position: %d\n", position);
}

void running(int id)
{
    printf("[" BLUE "EXEC" RST "] Running test %d\n", id);
}

void results(int total_tests, int passed_tests, int timeout)
{
    printf("[" PINK "====" RST "] Synthesis: Tested: " BLUE "%d " RST "| Passing: " GREEN "%d " RST "| TimedOut: " YELLOW "%d " RST "| Failed: " RED "%d\n\n" RST,
           total_tests, passed_tests, timeout, total_tests - passed_tests - timeout);
}

void time_warning(int id, char *query, double time)
{
    printf("[" YELLOW "PASS" RST "] Test %d :: %s (%fs)\n", id, query ,time);
    printf("    " YELLOW "+ " RST "Time limit exceeded: %fs\n", time);
}

void query_1(stats *statistics, char *expected, int *passed_tests, int *total, int *id, int *time)
{
    bool pass = true;
    running(*id);

    f_error *r = malloc(sizeof(*r));
    init_report(r);

    clock_t begin = clock();

    qtty_usr_bot_org(statistics, *id);

    clock_t end = clock();
    r->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", *id);

    compare_files(output_name, expected, r); 

    if (!(r->files)) { missing_files(*id, "query 1", r->exec_time); pass = false; }
    else if ((r->line == 0) && (r->exec_time >= 5.0)) { time_warning(*id, "query 1", r->exec_time); (*time)++; }
    else if (r->line == 0) passed(*id, "query 1", r->exec_time);
    else { failed(*id, "query 1", r->exec_time, r->line, r->position); pass = false; }

    if (pass) (*passed_tests)++;
    (*total)++; (*id)++;

    free(output_name); free(r);
}

void query_2(stats *statistics, user_cat *users, commit_cat *commits, char *expected, int *passed_tests, int *total, int *id, int *time)
{
    bool pass = true;
    running(*id);

    f_error *r = malloc(sizeof(*r));
    init_report(r);

    clock_t begin = clock();

    avg_colab_per_repo(statistics, users, commits, *id);

    clock_t end = clock();
    r->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", *id);

    compare_files(output_name, expected, r); 

    if (!(r->files)) { missing_files(*id, "query 2", r->exec_time); pass = false; }
    else if ((r->line == 0) && (r->exec_time >= 5.0)) { time_warning(*id, "query 2", r->exec_time); (*time)++; }
    else if (r->line == 0) passed(*id, "query 2", r->exec_time);
    else { failed(*id, "query 2", r->exec_time, r->line, r->position); pass = false; }

    if (pass) (*passed_tests)++;
    (*total)++; (*id)++;

    free(output_name); free(r);
}

void query_3(stats *statistics, char *expected, int *passed_tests, int *total, int *id, int *time)
{
    bool pass = true;
    running(*id);

    f_error *r = malloc(sizeof(*r));
    init_report(r);

    clock_t begin = clock();

    repos_with_bots(statistics, *id);

    clock_t end = clock();
    r->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", *id);

    compare_files(output_name, expected, r); 

    if (!(r->files)) { missing_files(*id, "query 3", r->exec_time); pass = false; }
    else if ((r->line == 0) && (r->exec_time >= 5.0)) { time_warning(*id, "query 3", r->exec_time); (*time)++; }
    else if (r->line == 0) passed(*id, "query 3", r->exec_time);
    else { failed(*id, "query 3", r->exec_time, r->line, r->position); pass = false; }

    if (pass) (*passed_tests)++;
    (*total)++; (*id)++;

    free(output_name); free(r);
}

void query_4(stats *statistics, char *expected, int *passed_tests, int *total, int *id, int *time)
{
    bool pass = true;
    running(*id);

    f_error *r = malloc(sizeof(*r));
    init_report(r);

    clock_t begin = clock();

    avg_commits_per_usr(statistics, *id);

    clock_t end = clock();
    r->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", *id);

    compare_files(output_name, expected, r); 

    if (!(r->files)) { missing_files(*id, "query 4", r->exec_time); pass = false; }
    else if ((r->line == 0) && (r->exec_time >= 5.0)) { time_warning(*id, "query 4", r->exec_time); (*time)++; }
    else if (r->line == 0) passed(*id, "query 4", r->exec_time);
    else { failed(*id, "query 4", r->exec_time, r->line, r->position); pass = false; }

    if (pass) (*passed_tests)++;
    (*total)++; (*id)++;

    free(output_name); free(r);
}

void query_5(user_cat *users, commit_cat *commits, char *expected, test *test_report)
{
    char params[] = "10 2009-12-12 2011-12-12";
    char *name = "query 5";
    running(test_report->id);

    /* ==== Executar e Tempo de Execução ============================================== */

    f_error *report = malloc(sizeof(f_error));
    init_report(report);

    clock_t begin = clock();
    commit_leaderboard(users, commits, params, test_report->id);

    clock_t end = clock();
    report->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", test_report->id);

    /* ==== Comparar os Ficheiros ===================================================== */

    compare_files(output_name, expected, report); 

    /* ==== Avaliar os Resultados ===================================================== */

    if (!(report->files)) 
    { 
        missing_files(test_report->id, name, report->exec_time); 
    }
    else if ((report->line == 0) && (report->exec_time >= 5.0)) 
    { 
        time_warning(test_report->id, name, report->exec_time); 
        test_report->timeout++;
    }
    else if (report->line == 0) 
    {
        passed(test_report->id, name, report->exec_time);
        test_report->passed_tests++;
    }
    else 
    { 
        failed(test_report->id, name, report->exec_time, report->line, report->position); 
    }

    test_report->total_tests++;
    test_report->id++;

    free(output_name); free(report);
}

void query_6(user_cat *users, commit_cat *commits, repo_cat *repos, char *expected, test *test_report)
{
    char params[] = "10 Rust";
    char *name = "query 6";
    running(test_report->id);

    /* ==== Executar e Tempo de Execução ============================================== */

    f_error *report = malloc(sizeof(f_error));
    init_report(report);

    clock_t begin = clock();
    commit_leaderboard_lang(users, commits, repos, params, test_report->id);

    clock_t end = clock();
    report->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", test_report->id);

    /* ==== Comparar os Ficheiros ===================================================== */

    compare_files(output_name, expected, report); 

    /* ==== Avaliar os Resultados ===================================================== */

    if (!(report->files)) 
    { 
        missing_files(test_report->id, name, report->exec_time); 
    }
    else if ((report->line == 0) && (report->exec_time >= 5.0)) 
    { 
        time_warning(test_report->id, name, report->exec_time); 
        test_report->timeout++;
    }
    else if (report->line == 0) 
    {
        passed(test_report->id, name, report->exec_time);
        test_report->passed_tests++;
    }
    else 
    { 
        failed(test_report->id, name, report->exec_time, report->line, report->position); 
    }

    test_report->total_tests++;
    test_report->id++;

    free(output_name); free(report);
}

void query_7(repo_cat *repos, commit_cat *commits, char *expected, test *test_report)
{
    char params[] = "2011-12-12";
    char *name = "query 7";
    running(test_report->id);

    /* ==== Executar e Tempo de Execução ============================================== */

    f_error *report = malloc(sizeof(f_error));
    init_report(report);

    clock_t begin = clock();
    unactive_repos(commits, repos, params, test_report->id);

    clock_t end = clock();
    report->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", test_report->id);

    /* ==== Comparar os Ficheiros ===================================================== */

    compare_files(output_name, expected, report); 

    /* ==== Avaliar os Resultados ===================================================== */

    if (!(report->files)) 
    { 
        missing_files(test_report->id, name, report->exec_time); 
    }
    else if ((report->line == 0) && (report->exec_time >= 5.0)) 
    { 
        time_warning(test_report->id, name, report->exec_time); 
        test_report->timeout++;
    }
    else if (report->line == 0) 
    {
        passed(test_report->id, name, report->exec_time);
        test_report->passed_tests++;
    }
    else 
    { 
        failed(test_report->id, name, report->exec_time, report->line, report->position); 
    }

    test_report->total_tests++;
    test_report->id++;

    free(output_name); free(report);
}

void query_8(repo_cat *repos, char *expected, test *test_report)
{
    char params[] = "10 2011-12-12";
    char *name = "query 8";
    running(test_report->id);

    /* ==== Executar e Tempo de Execução ============================================== */

    f_error *report = malloc(sizeof(f_error));
    init_report(report);

    clock_t begin = clock();
    top_languages(repos, params, test_report->id);

    clock_t end = clock();
    report->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", test_report->id);

    /* ==== Comparar os Ficheiros ===================================================== */

    compare_files(output_name, expected, report); 

    /* ==== Avaliar os Resultados ===================================================== */

    if (!(report->files)) 
    { 
        missing_files(test_report->id, name, report->exec_time); 
    }
    else if ((report->line == 0) && (report->exec_time >= 5.0)) 
    { 
        time_warning(test_report->id, name, report->exec_time); 
        test_report->timeout++;
    }
    else if (report->line == 0) 
    {
        passed(test_report->id, name, report->exec_time);
        test_report->passed_tests++;
    }
    else 
    { 
        failed(test_report->id, name, report->exec_time, report->line, report->position); 
    }

    test_report->total_tests++;
    test_report->id++;

    free(output_name); free(report);
}

void query_9(user_cat *users, commit_cat *commits, repo_cat *repos, char *expected, test *test_report)
{
    char params[] = "10";
    char *name = "query 9";
    running(test_report->id);

    /* ==== Executar e Tempo de Execução ============================================== */

    f_error *report = malloc(sizeof(f_error));
    init_report(report);

    clock_t begin = clock();
    top_commits_friends(users, commits,repos, params, test_report->id);

    clock_t end = clock();
    report->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", test_report->id);

    /* ==== Comparar os Ficheiros ===================================================== */

    compare_files(output_name, expected, report); 

    /* ==== Avaliar os Resultados ===================================================== */

    if (!(report->files)) 
    { 
        missing_files(test_report->id, name, report->exec_time); 
    }
    else if ((report->line == 0) && (report->exec_time >= 5.0)) 
    { 
        time_warning(test_report->id, name, report->exec_time); 
        test_report->timeout++;
    }
    else if (report->line == 0) 
    {
        passed(test_report->id, name, report->exec_time);
        test_report->passed_tests++;
    }
    else 
    { 
        failed(test_report->id, name, report->exec_time, report->line, report->position); 
    }

    test_report->total_tests++;
    test_report->id++;

    free(output_name); free(report);
}

void query_10(user_cat *users, commit_cat *commits, repo_cat *repos, char *expected, test *test_report)
{
    char params[] = "10";
    char *name = "query 10";
    running(test_report->id);

    /* ==== Executar e Tempo de Execução ============================================== */

    f_error *report = malloc(sizeof(f_error));
    init_report(report);

    clock_t begin = clock();
    top_user_repo_message(users, commits,repos, params, test_report->id);

    clock_t end = clock();
    report->exec_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    char* output_name = malloc(sizeof(char) * 50);
    sprintf(output_name, "./saida/command%d_output.txt", test_report->id);

    /* ==== Comparar os Ficheiros ===================================================== */

    compare_files(output_name, expected, report); 

    /* ==== Avaliar os Resultados ===================================================== */

    if (!(report->files)) 
    { 
        missing_files(test_report->id, name, report->exec_time); 
    }
    else if ((report->line == 0) && (report->exec_time >= 5.0)) 
    { 
        time_warning(test_report->id, name, report->exec_time); 
        test_report->timeout++;
    }
    else if (report->line == 0) 
    {
        passed(test_report->id, name, report->exec_time);
        test_report->passed_tests++;
    }
    else 
    { 
        failed(test_report->id, name, report->exec_time, report->line, report->position); 
    }

    test_report->total_tests++;
    test_report->id++;

    free(output_name); free(report);
}

int main()
{
    stats *statistics = xmalloc(get_size_stats());
    user_cat *users = xmalloc(get_user_size());
    commit_cat *commits = xmalloc(get_commit_size());
    repo_cat *repos = xmalloc(get_repo_size());
    
    store(users, commits, repos, statistics); sleep(1);

    clear_screen(); system_info();
    printf("\n[" PINK "====" RST "] Testing Platform\n");

    /* Testes que vão ser executados */
    test *test_report = malloc(sizeof(test));
    init_test(test_report); 

    int id = 0, total_tests = 0, passed_tests = 0, timeout = 0;

    query_1(statistics, "./entrada/expected/expected_1.txt",
            &passed_tests, &total_tests, &id, &timeout);

    query_2(statistics, users, commits, "./entrada/expected/expected_2.txt",
            &passed_tests, &total_tests, &id, &timeout);

    query_3(statistics, "./entrada/expected/expected_3.txt", 
            &passed_tests, &total_tests, &id, &timeout);

    query_4(statistics, "./entrada/expected/expected_4.txt",
            &passed_tests, &total_tests, &id, &timeout);
    
    test_report->id = id;
    test_report->passed_tests = passed_tests;
    test_report->timeout = timeout;
    test_report->total_tests = total_tests;

    query_5(users, commits, "./entrada/expected/expected_5.txt", test_report);
    query_6(users, commits, repos, "./entrada/expected/expected_6.txt", test_report);
    query_7(repos, commits,  "./entrada/expected/expected_7.txt", test_report);
    query_8(repos, "./entrada/expected/expected_8.txt", test_report);
    query_9(users, commits, repos, "./entrada/expected/expected_9.txt", test_report);
    query_10(users, commits, repos, "./entrada/expected/expected_10.txt", test_report);
   
    results(test_report->total_tests, test_report->passed_tests, test_report->timeout);

    return 0;
}