#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glib.h>

#include "../include/ex3/query_parser.h"
#include "../include/ex3/database.h"
#include "../include/ex3/utils.h"

int main(int argc, char *argv[])
{

    if (argc != 2) 
    {
        fprintf(stderr, "No argument provided.\n");
        return 1;
    }

    char* query_file = argv[1];

    if (file_exists(query_file))
    {
        bool q = read_queries(query_file);
        if (q) return q;
        else
        {
            fprintf(stderr, "An error has occured while executing the program.\n");
            return 1;
        }
    } 
    else
    {
        fprintf(stderr, "File does not exist.\n");
        return 1;
    }

}

