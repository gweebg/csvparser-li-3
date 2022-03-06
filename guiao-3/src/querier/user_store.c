#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <glib.h>

#include "../include/querier/storing_tools.h"
#include "../include/querier/commit_store.h"
#include "../include/querier/user_store.h"
#include "../include/querier/statistics.h"
#include "../include/querier/utils.h"
#include "../include/querier/users.h"

typedef struct user_catalog
{
    GHashTable *users;
    user_reg **user_sorted_array;
    int user_sorted_array_size;

} user_cat;

void create_user(char* csv_line, user_reg *new_user)
{
    // id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos

    long id, followers, following, gists, public_repo;
    type t;
    datetime *created_at;
    char *username;

    csv_to_number(&csv_line, &id);               // id
    csv_to_string(&csv_line, &(username));       // username
    csv_to_type(&csv_line, &t);                  // type
    csv_to_datetime(&csv_line, &(created_at));   // created_at

    csv_to_number(&csv_line, &(followers));                 // followers
    int *follower_list = malloc(sizeof(int) * followers);   
    csv_to_array(&csv_line, follower_list, followers);      // follower_list

    csv_to_number(&csv_line, &(following));                 // following
    int *following_list = malloc(sizeof(int) * following);
    csv_to_array(&csv_line, following_list, following);     // following_list

    csv_to_number(&csv_line, &(gists));          // gists
    csv_to_number(&csv_line, &(public_repo));    // public_repos

    set_id(new_user, id);
    set_username(new_user, username);
    set_type(new_user, t);
    set_created_at(new_user, created_at);
    set_followers(new_user, followers);
    set_follower_list(new_user, follower_list);
    set_following(new_user, following);
    set_following_list(new_user, following_list);
    set_gists(new_user, gists);
    set_public_repo(new_user, public_repo);

    // free_datetime(created_at);
    // free(username);
    free(follower_list); 
    free(following_list);
}

void type_counter(user_reg *user, int *bots, int *orgs, int *users)
{
    type t = get_type(user);

    if (t == 0) (*bots)++;
    else if (t == 1) (*users)++;
    else if (t == 2) (*orgs)++;
    else return;
}

bool store_users(user_cat *user_catalog, stats *statistics, const char *filename)
{
    // printf("(store_users)\n");
    // printf("    > Opening file %s... ", filename);

    FILE *ufile = fopen(filename, "r");
    if (!ufile)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    // printf("Done!\n");
    // printf("    > Reading %s... ", filename);

    int bots = 0, orgs = 0, users = 0;

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, ufile);
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, ufile);   

    user_catalog->users = g_hash_table_new(g_int_hash, g_int_equal);  

    while (line_size >= 0)
    {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0;

        size_t user_size = get_user_size();
        user_reg *user = g_malloc(user_size + sizeof(datetime));
        
        create_user(line_buffer, user);

        int *id = malloc(sizeof(int));
        *id = get_id(user);

        g_hash_table_insert(user_catalog->users, GINT_TO_POINTER(id), user);
        type_counter(user, &bots, &orgs, &users);

        line_size = getline(&line_buffer, &line_buffer_size, ufile);
    }

    set_user_count(statistics, users);
    set_bot_count(statistics, bots);
    set_org_count(statistics, orgs);

    // printf("Done!\n");
    // printf("    > Data stored successfully!\n");

    fclose(ufile);
    if (line_buffer) free(line_buffer);

    return true;
}

void compute_colaborators(stats *statistics, user_cat *users, commit_cat *commits)
{
    set_total_colabs_bots(statistics, 0);

    int total_colab_bots = 0;
    int total_colab = 0;

    int total_commits = get_total_commits(statistics);
    int *commiter_ids = xmalloc(sizeof(int) * total_commits);
    int *author_ids   = xmalloc(sizeof(int) * total_commits);

    get_committer_id_list(commits, commiter_ids, total_commits);
    get_author_id_list(commits, author_ids, total_commits);

    user_reg *val;
    GHashTableIter iter;
    int *key_;

    g_hash_table_iter_init(&iter, users->users);
    while (g_hash_table_iter_next(&iter, (gpointer)&key_, (gpointer)&val))
    {
        type user_type = get_type(val);
        if ((search_int(commiter_ids, 0, total_commits, (int)*key_) != -1) || 
            (search_int(author_ids, 0, total_commits, (int)*key_)  != -1)   )
        {
            if (user_type == TYPE_BOT) 
                total_colab_bots++;

            total_colab++;
        }
    }

    free(commiter_ids);
    free(author_ids);

    set_total_colabs_bots(statistics, total_colab_bots);
    set_total_colabs(statistics, total_colab);
}

char* get_username_by_id(user_cat *users, long id)
{
    int *uid = xmalloc(sizeof(int));
    *uid = (int)id;

    user_reg *user = g_hash_table_lookup(users->users, (gpointer)uid);

    if (user)
        return get_username(user);
    else 
        return "UNKNOWN";

    free(uid);
    free_user(user);
}

bool is_friend(user_cat *users, int id, int owner_id)
{
    int *user_id = xmalloc(sizeof(int));
    *user_id = id;

    user_reg *user = g_hash_table_lookup(users->users, (gpointer)user_id);
    if (user)
    {
        int followers = get_followers(user), 
            following = get_following(user);

        int *followers_list = xmalloc(sizeof(int) * followers),
            *following_list = xmalloc(sizeof(int) * following);

        get_follower_list(user, followers_list, followers);
        get_follower_list(user, following_list, following);

        if (search_int(followers_list, 0, followers, owner_id) != -1 && search_int(following_list, 0, following, owner_id) != -1) 
        {
            free(user_id); free(followers_list); free(following_list);
            return true;
        }
        else 
        {
            free(user_id); free(followers_list); free(following_list);
            return false;
        }
    }   
    else return false;
}