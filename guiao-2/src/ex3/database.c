#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>

#include "../include/ex3/commitdata.h"
#include "../include/ex3/userdata.h"
#include "../include/ex3/database.h"
#include "../include/ex3/repodata.h"

bool generate_catalogs(user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics)
{
    /* ===GENERATOR============================================================= */

    bool user_done = build_user_catalog(users, statistics, USERS);
    bool commits_done = build_commits_catalog(commits, statistics, COMMITS);
    bool repos_done = build_repos_catalog(repos, statistics, REPOS);

    return (user_done && commits_done && repos_done);
}
