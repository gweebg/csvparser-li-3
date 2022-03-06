#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

long int fetch_size(char *size_str)
{
    size_str[strcspn(size_str, "\t\r\n")] = 0;

    char *endptr = NULL;
    long int size = strtol(size_str, &endptr, 10);

    if (size < 0 || (size_str[0] == '\0') || *endptr != '\0')
    { 
        return -1;
    }

    return size;
}

int main()
{
    char list[20][10000] = {"\n","1937658\n","1002934\n","abcdefg\n","1234abcd4567\n","abc1234def\n","asb123\n",
                          " \n", "  \n", "123acb\n", "123456\n"};

    for (int i = 0; i < 11; i++)
    {
        int y = fetch_size(list[i]);
        if (y == -1) printf("Invalid : %s\n", list[i]);
        else printf("Valid: %s\n", list[i]);
    }

    return 0;
}