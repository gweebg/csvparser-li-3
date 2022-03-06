#ifndef TUI_H
#define TUI_H

#define INPUT_PATH "./entrada"

typedef enum file_state
{
    EMPTY = 0,
    RAW = 1,
    OK = 2,
    FINAL = 3
} file_state;

bool is_present(char *what, char **where, int size);
file_state available_files(char *input_path);
void build_files(file_state state);
void interface();

#endif