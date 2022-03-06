#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>  
#include <glib.h>

#include "../include/ex3/execute.h"
#include "../include/ex3/utils.h"
#include "../include/ex3/database.h"
#include "../include/ex3/query_parser.h"

void qtty_usr_bot_org(stats *statistics, int number)
{
    printf("        > Running query 1... ");
    int users = get_user_count(statistics),
        bots  = get_bot_count(statistics),
        orgs  = get_org_count(statistics);

    char* out = "Bot: %d\nOrganization: %d\nUser: %d\n";
    char* put = malloc(sizeof(char) * 100);
    char* filename = malloc(sizeof(char) * 40);

    sprintf(filename, "./saida/command%d_output.txt",number);
    sprintf(put, out, bots, orgs, users);
    
    write_to_file(filename, put);
    free(put); free(filename);

    printf("Done!\n");
}

void avg_colab_per_repo(stats *statistics, int number)
{
    printf("        > Running query 2... ");
    int total_colab = get_total_colabs(statistics);
    int total_repos = get_total_repos(statistics);
    double avg = ((double)total_colab / (double)total_repos); 

    // Número médio de colaboradores por repositório
    char* out = "Average collaborator per repository: %.2f\n";
    char* put = malloc(sizeof(char) * 50);
    char* filename = malloc(sizeof(char) * 40);

    sprintf(filename, "./saida/command%d_output.txt", number);

    sprintf(put, out, avg);
    write_to_file(filename, put); 
    free(put); free(filename);

    printf("Done!\n");
}

void repos_with_bots(user_cat *users, commit_cat *commits, stats *statistics, int type, int number)
{
    printf("        > Running query 3... ");

    int try = get_repos_with_bots(statistics);
    if (try != -1)
    {
        char* out = "Number of Repositories with Bot as collaborator: %.d repository.\n";
        char* put = malloc(sizeof(char) * 70);
        char* filename = malloc(sizeof(char) * 40);

        sprintf(filename, "./saida/command%d_output.txt", number);

        sprintf(put, out, try);
        write_to_file(filename, put); 
        free(put); free(filename);
    }
    else
    {
        long id, 
            repo_id;

        int total = 0,
            size = get_committer_ids_size(commits),
            to_ignore_size = 0;

        long *to_ignore = malloc(sizeof(long) * size);

        for (int i = 0; i < size; i++)
        {
            id = get_committer_id(commits, i);
            repo_id = get_repo_id(commits, i);

            if (!linear_search(to_ignore, repo_id, to_ignore_size) && 
                is_collaborator(id, commits) && 
                check_user_type(users, type, (int)id))
            {
                to_ignore[to_ignore_size] = repo_id;
                to_ignore_size++;
                total++;
            }  
        }

        free(to_ignore);
        update_repos_with_bots(statistics, total);

        // Número de repositórios com bots como colaboradores.
        char* out = "Number of Repositories with Bot as collaborator: %.d repository.\n";
        char* put = malloc(sizeof(char) * 70);
        char* filename = malloc(sizeof(char) * 40);

        sprintf(filename, "./saida/command%d_output.txt", number);

        sprintf(put, out, total);
        write_to_file(filename, put); 
        free(put); free(filename);

    }

    printf("Done!\n");
}

void avg_commits_per_usr(stats *statistics, int number)
{
    printf("        > Running query 4... ");

    double try = get_avg_commits(statistics);
    if (try != (double)-1) 
    {
        char* out = "Average commit per user: %.2f%\n";
        char* put = malloc(sizeof(char) * 50);
        char* filename = malloc(sizeof(char) * 40);

        sprintf(filename, "./saida/command%d_output.txt", number);
        sprintf(put, out, try);
        write_to_file(filename, put); 
        free(put); free(filename);
    }
    else
    {
        int total_commits = get_total_commits(statistics);
        int total_users = get_total_users(statistics);
        double avg= ((double)total_commits / (double)total_users) * 100;
        update_avg_commits(statistics, avg);

        // Número médio de commits por utilizador
        char* out = "Average commit per user: %.2f%\n";
        char* put = malloc(sizeof(char) * 50);
        char* filename = malloc(sizeof(char) * 40);

        sprintf(filename, "./saida/command%d_output.txt", number);

        sprintf(put, out, avg);
        write_to_file(filename, put); 
        free(put); free(filename);
    }
    
    printf("Done!\n");
}

void commit_leaderboard(user_cat *users, commit_cat *commits, char* params, int number)
{
    printf("        > Running query 5... ");
    // Parametros nesta query estão separados por espaços.
    int user_threshold = 0, counter = 0;
    char *date_a = malloc(sizeof(char) * 16);
    char *date_b = malloc(sizeof(char) * 16);

    char *param = strtok(params, " ");
    while(param) 
    {
        if (counter == 0) { user_threshold = atoi(param); counter++; }
        else if (counter == 1)
        {
            strcpy(date_a, param);
            counter++;
        }
        else if (counter == 2)
        {
            strcpy(date_b, param);
            counter++;
        }
        param = strtok(NULL, " ");
    }

    int lower_bound = bound_date_index(commits, date_a);
    int upper_bound = bound_date_index(commits, date_b);

    triplet **temp_array = malloc(sizeof(temp_array) * get_committer_ids_size(commits));
    int ctr = 0;

    for (int i = lower_bound; i <= upper_bound; i++)
    {
        long current_id = get_committer_id(commits, i);
        char *username = get_username_by_id(users, current_id);

        triplet *temp = malloc(sizeof(*temp));

        temp->username = username;
        temp->id = current_id;
        temp->total_commits = 1;

        int index = triplet_search(temp_array, current_id, ctr);

        if (index == -1)
        {
            temp_array[ctr] = temp; 
            ctr++;
        }
        else
        {
            temp_array[index]->total_commits++;
        }
    }

    qsort(temp_array,
          ctr,
          sizeof(triplet*),
          compare_triplet);

    char* filename = malloc(sizeof(char) * 40);
    sprintf(filename, "./saida/command%d_output.txt", number);

    FILE *output = fopen(filename, "w+");
    if (!output) return;

    char* out = "%ld;%s;%d\n";
    char* put = NULL;

    for (int j = 0; j < user_threshold; j++)
    {
        put = malloc(sizeof(char) * 50);
        sprintf(put, out, temp_array[j]->id, temp_array[j]->username, temp_array[j]->total_commits);
        fputs(put, output);
    }

    printf("Done!\n");

    for (int i = 0; i < ctr; i++) free(temp_array[i]);
    free(temp_array);

    fclose(output);

    free(date_a); free(date_b);
    free(filename); free(put);
}

void commit_leaderboard_lang(user_cat *users, commit_cat *commits, repo_cat *repos, char *parameters, int number)
{
    printf("        > Running query 6... ");

    /* Parsing dos parametros. */

    char *language = parameters;
    char *user_str = strtok_r(language, " ", &language);
    int user_count = -1;

    if (user_str) user_count = atoi(user_str);
    else
    {
        fprintf(stderr, "\nInvalid query : number of users bad formatted.\n");
        exit(EXIT_FAILURE);
    }

    // string_to_lower(language);

    /* Execução da query. */

    int commits_size = get_committer_ids_size(commits);
    int size = 0;
    triplet **data_array = malloc(sizeof(triplet) * commits_size);

    for (int i = 0; i < commits_size; i++)
    {
        long repo_id = get_repo_id(commits, i);
        char *repo_lang = get_repo_language(repos, repo_id);

        if (repo_lang && strcmp(repo_lang, language) == 0) 
        {
            string_to_lower(repo_lang);

            long current_id = get_committer_id(commits, i);
            char *username = get_username_by_id(users, current_id);

            triplet *data = malloc(sizeof(*data));

            data->username = username;
            data->id = current_id;
            data->total_commits = 1;

            //TODO Verificar se este repo é repetido, se não for adicionar à struct else aumentar o total_commits.
            int index = triplet_search(data_array, current_id, size);
            if (index == -1) { data_array[size] = data; size++;}
            else data_array[index]->total_commits++; 
        }
    }

    qsort(data_array, size, sizeof(triplet*), compare_triplet);

    // for (int i = 0; i < size; i++) 
        // printf("%s - %d\n", data_array[i]->username, data_array[i]->total_commits);

    /* Writing to file. */

    char* filename = malloc(sizeof(char) * 40);
    sprintf(filename, "./saida/command%d_output.txt", number);

    FILE *output = fopen(filename, "w+");
    if (!output) return;

    char* out = "%ld;%s;%d\n";

    for (int j = 0; j < user_count; j++)
    {
        char *put = malloc(sizeof(char) * 50);
        sprintf(put, out, data_array[j]->id, data_array[j]->username, data_array[j]->total_commits);
        fputs(put, output);
        free(put);
    }

    printf("Done!\n");

    for (int i = 0; i < size; i++) free(data_array[i]);
    free(data_array); free(filename);

    fclose(output);
}

void unactive_repos(repo_cat *repos, char *parameters, int number)
{
    printf("        > Running query 7... ");

    int year = 0, month = 0, day = 0;
    sscanf(parameters, "%4d-%2d-%2d", &year, &month, &day);

    datetime *date = malloc(sizeof(datetime));
    date->year = year; date->month = month; date->day = day;
    date->hours = 0; date->minutes = 0; date->seconds = 0;

    int size = get_repo_array_size(repos), total = 0;

    char* filename = malloc(sizeof(char) * 30);
    sprintf(filename, "./saida/command%d_output.txt", number); 

    FILE* file = fopen(filename, "w+");

    int year_b = 0, month_b = 0, day_b = 0;

    for(int i = 0; i < size; i++)
    {
        char* current_date_string = get_updated_at(repos, i);
        datetime* current_date = malloc(sizeof(datetime));

        sscanf(current_date_string, "%4d-%2d-%2d", &year_b, &month_b, &day_b);
        current_date->year = year_b; current_date->month = month_b; current_date->day = day_b;
        current_date->hours = 0; current_date->minutes = 0; current_date->seconds = 0;

        if (compare_datetimes(current_date, date) < 0)
        {
            long repo_id = get_repo_id_by_index(repos, i);
            char* description = get_repo_description(repos, i);
            char* output = malloc(sizeof(char) * (strlen(description) + 20));

            sprintf(output,"%ld;%s\n", repo_id, description);
            fputs(output, file);

            total++;
            free(output);
        }
    }

    printf("Done!\n");

    fclose(file);
    free(date); free(filename);
}

void top_languages(repo_cat *repos, char *parameter, int number)
{
    printf("        > Running query 8... ");

    /* Parsing the parameter. */

    char *date_string = parameter;
    char *number_str = strtok_r(date_string, " ", &date_string);
    int num = atoi(number_str);

    /* Computing top languages. */

    int date_start = future_date_index(repos, date_string);
    int size = get_repo_array_size(repos);
    int counter = 0;

    triplet **language_storage = malloc(sizeof(triplet) * 500);

    for (int i = date_start; i < size; i++)
    {
        char *lang = get_repo_lang_by_index(repos, i);

        if (strcmp(lang, "None") != 0)
        {
            int find = triplet_search_language(language_storage, lang, counter);

            if (find == -1) 
            {
                triplet *current_language = malloc(sizeof(triplet));
                current_language->username = lang;
                current_language->total_commits = 1;

                language_storage[counter] = current_language;
                counter++;
            }
            else language_storage[find]->total_commits++;
        }

    }

    qsort(language_storage, counter, sizeof(triplet*), compare_triplet);

    // for (int i = 0; i < counter; i++) printf("\n%s - %ld\n", language_storage[i]->username, language_storage[i]->total_commits);

    /* Writing output to file. */

    char* filename = malloc(sizeof(char) * 40);
    sprintf(filename, "./saida/command%d_output.txt", number);

    FILE* file = fopen(filename, "w+");
    for (int g = 0; g < num; g++) 
    {
        char *output = malloc(sizeof(char) * (strlen(language_storage[g]->username) + 4));
        sprintf(output, "%s\n", language_storage[g]->username);
        fputs(output, file);
        free(output);
    }

    printf("Done!\n");

    for (int i = 0; i < counter; i++) free(language_storage[i]);
    free(language_storage); 
    fclose(file); free(filename);
}

void top_commits_friends(user_cat *users, commit_cat *commits, repo_cat *repos, char *param, int number)
{
    printf("        > Running query 9... ");

    int user_count = atoi(param);

    /* Computing results. */

    int commits_size = get_committer_ids_size(commits);
    int counter = 0;

    q9 **data = malloc(sizeof(q9) * commits_size);

    for (int i = 0; i < commits_size; i++)
    {
        int commiter_id = get_committer_id(commits, i);
        long repo_id = get_repo_id(commits, i);
        int owner_id = get_owner_by_id(repos, repo_id);

        q9 *elem = malloc(sizeof(q9));

        if (owner_id != -1 && is_friend(users, commiter_id, owner_id))
        {
            int index = search_q9(data, commiter_id, counter);
            if (index == -1)
            {
                elem->username = get_username_by_id(users, commiter_id);
                elem->total_commits = 1;
                elem->user_id = commiter_id;
                data[counter] = elem;
                counter++;
            }
            else data[index]->total_commits++;
        }
    }

    qsort(data, counter, sizeof(q9*), compare_q9_commits);

    /* Writing to file. */

    char* filename = malloc(sizeof(char) * 40);
    sprintf(filename, "./saida/command%d_output.txt", number);

    FILE *file = fopen(filename, "w+");
    if (!file) exit(EXIT_FAILURE);

    for (int u = 0; u < user_count; u++)
    {
        char* output = malloc(sizeof(char) * 50);
        sprintf(output, "%ld;%s\n", data[u]->user_id, data[u]->username);
        fputs(output, file);
        free(output);
    }

    printf("Done!\n");

    for (int u = 0; u < counter; u++) free(data[u]);
    free(data); free(filename);
    fclose(file);
}

// void biggest_messages (user_cat *users, commit_cat *commits, repo_cat *repos, stats* statistics, char* params, int number)  
// {
//     printf("        > Running query 10... ");

//     /* Parsing the parameter. */

//     int user_count = atoi(params); 
    
//     /* Copying every username into an hash table. */
//     GHashTable *usernames = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, NULL);
//     int total_users = get_total_users(statistics);

//     for (int u = 0; u < total_users; u++)
//     {
//         int *key = g_new0(gint, 1); *key = get_user_id_by_index(users, u);
//         char *value = get_username_by_index(users, u);
//         g_hash_table_insert(usernames, key, g_strdup(value));
//     }

//     /* Computing list */
    
//     int commit_size = get_committer_ids_size(commits);
//     int repos_size = get_repo_array_size(repos);
//     int counter = 0;

//     q10 **data = malloc(sizeof(q10) * repos_size);

//     for (int i = 0; i < repos_size; i++)
//     {
//         int repo_id = get_repo_id_by_index(repos, i);
//         // GHashTable *temp_data = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, g_free);

//         for (int t = 0; t < commit_size; t++)
//         {
//             GHashTable *temp_data = g_hash_table_new_full(g_int_hash, g_int_equal, g_free, g_free);
//             int commit_repo_id = get_repo_id(commits, t);
//             int *user_id = g_new0(gint, 1); 
//             *user_id = (int)get_committer_id(commits, t);

//             if ((repo_id == commit_repo_id) && !g_hash_table_lookup(temp_data, user_id))
//             {
//                 // q10 *elem = (q10*)malloc(sizeof(q10));

//                 // elem->id = (long)user_id;

//                 // char* uname = g_hash_table_lookup(usernames, user_id);
//                 // elem->username = malloc(sizeof(char) * strlen(uname));
//                 // strncpy(elem->username, uname, strlen(uname));

//                 // elem->message_size = get_message_size(commits, t);
//                 // elem->repo_id = (long)repo_id;

//                 // g_hash_table_insert(temp_data, user_id, elem);

//             }
            
//             g_hash_table_destroy(temp_data);
        
//         }

//         counter = 0;
//     }

//     printf("Done!\n");

// }

bool execute(QUERY *query, user_cat *users, commit_cat *commits, repo_cat *repos, stats *statistics)
{
    int id = query->query_id;
    int num = query->number;

    if (id == 1)  qtty_usr_bot_org(statistics, num);
    if (id == 2)  avg_colab_per_repo(statistics, num);
    if (id == 3)  repos_with_bots(users, commits, statistics, 2, num);
    if (id == 4)  avg_commits_per_usr(statistics, num);
    if (id == 5)  commit_leaderboard(users, commits, strdup(query->param), num);
    if (id == 6)  commit_leaderboard_lang(users, commits, repos, strdup(query->param), num);
    if (id == 7)  unactive_repos(repos, strdup(query->param), num);
    if (id == 8)  top_languages(repos, strdup(query->param), num);
    if (id == 9)  top_commits_friends(users, commits, repos, strdup(query->param), num);
    if (id == 10) printf("Toto, I've got a feeling we're not in Kansas anymore.\n");

    return true;
}

