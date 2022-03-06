#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>

#include "../includes/ex1/fetch_users.h"
#include "../includes/ex1/fetch_commits.h"
#include "../includes/ex1/fetch_repos.h"
#include "../includes/ex2/clean_up.h"

int main(int argc, char *argv[])
{

    if (argc != 2) 
    {
        fprintf(stderr, "No argument provided.\n");
        return 1;
    }

    if (strcmp(argv[1], "exercicio-1") == 0)
    {
        bool users = fusers("./entrada/users.csv");
        bool commits = fcommits("./entrada/commits.csv");
        bool repos = frepos("./entrada/repos.csv");

        return 0 ? (users && commits && repos) : 1; 
    }
    else if (strcmp(argv[1], "exercicio-2") == 0)
    {
        clean_up();
        return 0;
    }
    else
    {
        fprintf(stderr, "Exercício inválido.\n");
        return 1;
    }
}

