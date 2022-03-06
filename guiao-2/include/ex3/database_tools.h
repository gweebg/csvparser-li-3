#ifndef DB_TOOLS
#define DB_TOOLS

#include "../include/ex3/database.h"

/**
 * @brief Converts the first parameter of a csv string into an long integer, if possible, and stores it on an output parameter.
 * @param csv_string csv string containing the values.
 * @param output, output parameter where we "save" the number. 
 */
void csv_to_number(char **csv_string, long *output);

/**
 * @brief Gets the first parameter of a csv string, if possible, and stores it on an output parameter.
 * @param csv_string csv string containing the values.
 * @param output, output parameter where we "save" the string. 
 */
void csv_to_string(char **csv_string, char **output);

/**
 * @brief Converts the first parameter of a csv string into an enum Type variable, if possible, and stores it on an output parameter.
 * @param csv_string csv string containing the values.
 * @param output, output parameter where we "save" the type. 
 */
void csv_to_type(char **csv_string, type *output);

/**
 * @brief Converts the first parameter of a csv string into a struct datetime, if possible, and stores it on an output parameter.
 * @param csv_string csv string containing the values.
 * @param output, output parameter where we "save" the datetime. 
 */
void csv_to_datetime(char **csv_string, datetime **output);

/**
 * @brief Converts the first parameter of a csv string into an array of long integers, if possible, and stores it on an output parameter.
 * @param csv_string csv string containing the values.
 * @param output, output parameter where we "save" the array. 
 */
void csv_to_array(char **csv_string, int *output, int output_size);

/**
 * @brief Gets the value of the first parameter of a csv string and checks if it's True or False, stores the value on an output parameter.
 * @param csv_string csv string containing the values.
 * @param output, output parameter where we "save" the boolean value. 
 */
void csv_check_wiki(char **csv_string, bool *output);

#endif