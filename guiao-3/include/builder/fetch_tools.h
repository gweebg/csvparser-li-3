#ifndef FTOOLS_H
#define FTOOLS_H

void check_number(char **csv_string, long *value, bool *validity);
void check_string(char **csv_string, bool can_be_empty, bool *validity);
void skip_token(char **csv_string);
void check_type(char **csv_string, bool *validity);
void check_date(char **csv_string, bool *validity);
bool is_number(char* str_num);
void check_array(char **csv_string, long *size, bool *validity);
void compare_f(long followers, long actual_followers, long following, long actual_following, bool *validity);
void check_wiki(char **csv_string, bool *validity);
int get_lines_from_file(const char *filename);
bool is_date(char *datetime);

#endif