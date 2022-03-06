#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "../include/ex3/userdata.h"
#include "../include/ex3/database_tools.h"
#include "../include/ex3/statistics.h" 
#include "../include/ex3/commitdata.h"

#include "../include/ex3/utils.h"

typedef struct user_registry
{
    int* following_list; /**< array of integers that has other user ids that follow the original user. */
    int* follower_list; /**< array of integers that has other user ids whose are followed by the original user. */
    char* username; /**< string that represents the login of the user. */

    datetime* created_at; /**< struct that represents the datetime on which the user account was created. */
    type type; /**< enum that represents the User Type. */

    long public_repo, /**< long int that represents the number of public repos of a user. */
         followers, /**< long int that represents the number of followers of a user. */
         following, /**< long int that represents the number of following users of a user. */
         gists, /**< long int that represents the number of gists of a user. */
         id; /**< long int that represents the id number of a user. */

} user_reg;

typedef struct user_catalog
{

    user_reg **user_sorted_array;
    int user_sorted_array_size;

} user_cat;

int compare_data(const void* a, const void* b) 
{
    user_reg *user_a = *(user_reg**)a;
    user_reg *user_b = *(user_reg**)b;

    if (user_a->id < user_b->id) return -1;
    else if (user_a->id > user_b->id) return 1;
    else return 0;
}

void sort_user_array_by_id(user_reg **user_array, int first, int last)
{
    qsort(user_array, (last - first), sizeof(user_reg*), &compare_data);
}

void create_user(char* csv_line, user_reg *new_user)
{
    // id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos
    // new_user->follower_list = NULL;
    // new_user->following_list = NULL;

    csv_to_number(&csv_line, &(new_user->id));
    csv_to_string(&csv_line, &(new_user->username));
    csv_to_type(&csv_line, &(new_user->type));
    csv_to_datetime(&csv_line, &(new_user->created_at));
    csv_to_number(&csv_line, &(new_user->followers));
    new_user->follower_list = malloc(sizeof(int) * new_user->followers);
    csv_to_array(&csv_line, new_user->follower_list, new_user->followers);
    csv_to_number(&csv_line, &(new_user->following));
    new_user->following_list = malloc(sizeof(int) * new_user->following);
    csv_to_array(&csv_line, new_user->following_list, new_user->following);
    csv_to_number(&csv_line, &(new_user->gists));
    csv_to_number(&csv_line, &(new_user->public_repo));
}

bool build_users(user_cat *user_catalog, stats *statistics, const char *filename)
{
    printf("(build_users)\n");

    int counter = 0, bot_count = 0, user_count = 0, org_count = 0;

    printf("    > Opening file %s... ", filename);

    FILE *ufile = fopen(filename, "r");
    if (!ufile)
    {
        fprintf(stderr, "\nCouldn't open %s file. Aborting.\n", filename);
        exit(EXIT_FAILURE);
    }

    printf("Done!\n");
    printf("    > Reading %s... ", filename);

    char* line_buffer = NULL;
    size_t line_buffer_size = 0;
    
    getline(&line_buffer, &line_buffer_size, ufile); // Skips the header line.
    ssize_t line_size = getline(&line_buffer, &line_buffer_size, ufile);     

    while (line_size >= 0)
    {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0; // Remove o \n ou \r\n

        user_reg *user = (user_reg*)malloc(sizeof(*user));
        create_user(line_buffer, user); /* <-- Cria e popula o user */

        user_catalog->user_sorted_array[counter] = user;
        // dump_user(user_catalog->user_sorted_array[counter]);

        switch(user->type)
        {
            case TYPE_BOT:
                bot_count++;
                break;
            case TYPE_ORG:
                org_count++;
                break;
            case TYPE_USR:
                user_count++;
                break;
            default:
                fprintf(stderr, "Switch on 'user.type' to default on 'build_users' @ userdata.c .\n");
                exit(EXIT_FAILURE);
        }

        line_size = getline(&line_buffer, &line_buffer_size, ufile);
        counter++; 
    }

    update_user_count(statistics, user_count);
    update_bot_count(statistics, bot_count);
    update_org_count(statistics, org_count);

    user_catalog->user_sorted_array_size = counter;

    printf("Done!\n");
    printf("    > Data stored successfully!\n");

    fclose(ufile);
    if (line_buffer) free(line_buffer);

    return true;
}

bool build_user_catalog(user_cat *new_catalog, stats *statistics, const char *filename)
{

    /* ==STRUCT============================================================================= */

    /*
        user_cat *new_catalog
                | user_reg *user_sorted_array
                | int user_sorted_array_size
    */

    /* ==INITIALIZATION============================================================================= */

    int user_count = get_lines_from_file(filename);
    update_total_users(statistics, user_count - 1);

    new_catalog->user_sorted_array = malloc(sizeof(user_reg) * user_count);
    if (new_catalog->user_sorted_array == NULL)
    {
        fprintf(stderr, "Could not allocate memory for 'user_sorted_array' in 'build_user_catalog' @ userdata.c\n");
        exit(EXIT_FAILURE);
    }

    /* ==BUILDING=================================================================================== */

    build_users(new_catalog, statistics, filename); /* <-- Popular o array e struct stats */

    printf("    > Sorting array... ");
    sort_user_array_by_id(new_catalog->user_sorted_array, 0, user_count - 1);
    printf("Done!\n\n");

    //mdemare, 14, index = 25
    return true;
}

int user_bin_search(user_reg **user_array, int start, int end, int key)
{
    if (end >= start)
    {
        int mid = start + (end - start) / 2;

        if (user_array[mid]->id == key) return mid;

        if (user_array[mid]->id > key) return user_bin_search(user_array, start, mid - 1, key);
        else return user_bin_search(user_array, mid + 1, end, key);
    }

    return -1;
}

char* get_username_by_id(user_cat *users, long id)
{
   int index =  user_bin_search(users->user_sorted_array, 0, users->user_sorted_array_size, (int)id);
   if (index != -1) return strdup(users->user_sorted_array[index]->username);
   else return "NOT FOUND";
}

bool check_user_type(user_cat *catalog, int compare_type, int id)
{
    type t;
    switch(compare_type)
    {
        case 1: 
            t = TYPE_USR;
            break;
        case 2:
            t = TYPE_BOT;
            break;
        case 3:
            t = TYPE_ORG;
            break;
        default:
            fprintf(stderr, "Switch on 'compare_type' to default on 'check_user_type' @ userdata.c .\n");
            exit(EXIT_FAILURE);        
    }

    int where_is = user_bin_search(catalog->user_sorted_array, 0, catalog->user_sorted_array_size, id);
    if (where_is == -1) 
    { 
        fprintf(stderr, "   > User not found in 'check_user_type' @ userdata.c .\n"); 
        return false;
    }

    if (catalog->user_sorted_array[where_is]->type == t) return true;
    return false;
}

void destroy_user_catalog(user_cat *catalog)
{
    for (int i= 0; i < catalog->user_sorted_array_size; i++)
    {
        // free(catalog->user_sorted_array[i]->follower_list);
        // free(catalog->user_sorted_array[i]->following_list);
        free(catalog->user_sorted_array[i]->username);
        free(catalog->user_sorted_array[i]);
    }

    free(catalog);
}

int compute_total_colabs(user_cat *users, commit_cat *commits)
{
    int total_colab = 0;
    int size = users->user_sorted_array_size;

    for (int i = 0; i < size; i++)
    {
        bool x = is_collaborator((users->user_sorted_array[i])->id, commits);
        if (x) total_colab++;
    }

    return total_colab;
}

bool is_friend(user_cat *users, long my_id, long owner_id)
{
    int index = user_bin_search(users->user_sorted_array, 0, users->user_sorted_array_size, my_id);
    user_reg *user = users->user_sorted_array[index];

    if (user->followers * user->following != 0) return false;

    int in_follower = search(user->follower_list, 0, user->followers, owner_id);
    int in_following = search(user->following_list, 0, user->following, owner_id);

    if (in_follower != -1 || in_following != -1) return false;
    else return true;
}

void dump_user(user_reg *user)
{
    // id;login;type;created_at;followers;follower_list;following;following_list;public_gists;public_repos
    printf("===USER================\n");
    printf("Id : %ld\n", user->id);
    printf("Login : %s\n", user->username);
    printf("Type : %d\n", user->type);
    printf("Followers : %ld\n", user->followers);
    printf("Following : %ld\n", user->following);
    printf("Public Gists : %ld\n", user->gists);
    printf("Public Repos : %ld\n", user->public_repo);
    printf("Created at: %s\n", user->created_at->str_repr);
    printf("=======================\n");
}

int get_user_id(user_reg *user)
{
    return user->id;
}

int get_user_array_size(user_cat *cat)
{
    return cat->user_sorted_array_size;
}

int get_user_size()
{
    return (int)sizeof(user_reg);
}

int get_user_cat_size()
{
    return (int)sizeof(user_cat);
}

int get_user_id_by_index(user_cat *users, int index)
{
    return users->user_sorted_array[index]->id;
}

char* get_username_by_index(user_cat *users, int index)
{
    return strdup(users->user_sorted_array[index]->username);
}