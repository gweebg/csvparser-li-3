#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "../../include/builder/fetch_commits.h"
#include "../../include/builder/fetch_repos.h"
#include "../../include/builder/fetch_users.h"
#include "../../include/builder/clean_up.h"

#include "../../include/ui_helpers.h"

void build_from_raw()
{
    cursor(false); clear_screen();

    print_boxed("Status dos ficheiros : RAW\nA correr o guiao 1, aguarde!");
    printf("\n");

    printf(" Ficheiro ./entrada/users.csv... ");
    build_users("./entrada/users.csv");
    printf(" Ficheiro ./entrada/commits.csv... ");
    build_commits("./entrada/commits.csv");
    printf(" Ficheiro ./entrada/repos.csv... ");
    build_repos("./entrada/repos.csv");

    printf(" A retocar os ficheiros... ");
    clean_up();
    
    sleep(1); cursor(true);
}

void build_from_ok()
{
    cursor(false); clear_screen();

    print_boxed("Status dos ficheiros : OK\nA correr o exercício 2 do guiao 1, aguarde!");
    printf("\n");

    printf("A retocar os ficheiros... ");
    clean_up();

    sleep(1); cursor(true);
}

void build_from_final()
{
    cursor(false); clear_screen();

    print_boxed("Status dos ficheiros : FINAL\nA correr o guiao 2, aguarde!");
    printf("\n");

    cursor(true);
}
