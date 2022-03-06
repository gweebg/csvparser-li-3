#include <stdio.h> 
#include <stdlib.h>
#include <string.h>  
#include <stdbool.h>

#include "../include/querier/users.h"
#include "../include/querier/user_store.h"

#include "../include/querier/commits.h"
#include "../include/querier/commit_store.h"

#include "../include/querier/repos.h"
#include "../include/querier/repo_store.h"

#include "../include/querier/statistics.h"

#include "../include/querier/query_parser.h"
#include "../include/querier/execute.h"
#include "../include/querier/utils.h"

bool read_queries(const char *filename)
{
    stats *statistics = xmalloc(get_size_stats());
    user_cat *users = xmalloc(get_user_size());
    commit_cat *commits = xmalloc(get_commit_size());
    repo_cat *repos = xmalloc(get_repo_size());

    init_stats(statistics);
    store_users(users, statistics, "./entrada/users-final.csv");
    store_commits(commits, statistics, "./entrada/commits-final.csv");
    store_repos(repos, statistics, "./entrada/repos-final.csv");

    printf("(read_queries)\n");

    char *buffer = (char*)xmalloc(BUFFER_SIZE * sizeof(char));

    if (file_exists(filename) && check_ext(filename, ".txt"))
    {
        printf("    > Opening query file... ");
        
        FILE *qfile = fopen(filename, "r");
        int query_num;

        if (!qfile || !buffer) 
        {
            fprintf(stderr, "Failed to open the query file, exiting.\n");
            return false;
        }

        printf("Done!\n");
        printf("    > Reading and executing queries...\n");

        QUERY *q = (QUERY*)xmalloc(sizeof(QUERY)); 
        int counter = 1;

        while(fgets(buffer, BUFFER_SIZE, qfile))
        {
            char *rest = buffer;
            (rest)[strcspn((rest), "\n")] = 0;

            char *id_str = strtok_r(rest, " ", &rest);
            query_num = atoi(id_str);

            if (query_num <= 10 || test_line(rest, query_num))
            {
                q->query_id = query_num; q->param = strdup(rest); q->number = counter;
                counter++;
                if (!execute(q, users, commits, repos, statistics)) return false;
            }        
        }

        free(buffer); free(q); fclose(qfile);

        // destroy_user_catalog(users);
        // destroy_commit_catalog(commits);
        // destroy_repos_catalog(repos);

        return true;
    }

    free(buffer);

    // destroy_user_catalog(users);
    // destroy_commit_catalog(commits);
    // destroy_repos_catalog(repos);

    printf("    > Failed to detect a query file!\n");
    return false;
}