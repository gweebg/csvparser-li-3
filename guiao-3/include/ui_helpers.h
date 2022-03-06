#ifndef UIH_H
#define UIH_H

#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void print_boxed(char *content);
void print_boxed_bold(char *content);
void print_list_item(char *list_number, char *content);
void print_list_item_non(char *list_number, char *content);
void clear_screen();
void update_bar(int i, int bar_length, int max_items);
void cursor(bool show);
void pag_output(char *filename, int threshold, int above);
void line_up();
void clear_line();
void get_num(char *context, int *number);
void get_date(char *context, char **date);
void get_lang(char *context, char **lang);
bool is_lang(char *lang);

#endif