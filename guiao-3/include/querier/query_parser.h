#include <stdbool.h>

#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#define BUFFER_SIZE 500

typedef struct query
{
    int query_id;
    char* param;
    int number;
} QUERY;

bool read_queries(const char *filename);

#endif