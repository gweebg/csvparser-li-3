#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


void remove_first_last(const char *string, char *result)
{
    int len = strlen(string);
    if(len > 0) strcpy(result, ++string);
    if(len > 1) result[len - 2] = '\0';
}

int *string_to_array_test(char *string)
{
    /* Recebe uma string do tipo '[1,2,3,4]' e converte num array de inteiros */
    
    /* Verifica se existe string */
    if (strcmp(string, "") == 0) {printf("String vazia.           "); return NULL;}

    /* Verificar se a string começa e acaba com parênteses retos. */
    if (string[0] != '[' || string[strlen(string) - 1] != ']') {printf("String não delimitada.  "); return NULL;}

    /* Remover os parênteses da string para poder retirar os números */
    char new_string[strlen(string) - 2];
    remove_first_last(string, new_string);

    /* Verificar o contéudo da string */
    char *final_string = (char*)malloc(sizeof(char) * 200000);
    strcpy(final_string, new_string);

    int* final_array = NULL;

    if (strcmp(final_string, "") == 0) {printf("Array vazio.            "); return final_array;}
    
    /* Extrair o contéudo da string */
    char *buffer;
    char *endptr;
    
    int i = 0;

    while(((buffer = strsep(&final_string, ","))) != NULL)
    {
        final_array = realloc(final_array, sizeof(int) * 100);
        int number = strtol(buffer, &endptr, 10);

        if (number > 0 && *endptr == '\0')
        {
            final_array[i] = number;
            i++;
        }
        else 
        {
            printf("Letras no array         ");
            return NULL;
        }

    }

    free(final_string);
    return final_array;
}

int main(void)
{
    char list[12][50] = { "[1, 2, 3, 4]", "[1,2,3,4]", "[]", "hello", "30/07/21", "[a, b, 3, 6]", 
                          "[a ,b, c, d]", "", "-1", "[1, 2, 3", "1, 2, 3, 4]", "[12a, 23, 90, b1]"};

    char final[100000] = "[12377647, 26179453, 16600112, 3076393, 30059233, 397632, 32831059, 31426614, 30850314, 27717988, 28847055, 5789813, 26656464, 10634210, 21250813, 490943, 29587192, 26271749, 7715231, 11429067, 35736821, 418638, 33871591, 1308397, 37555946, 11368523, 37696110, 38815324, 31944736]";

    for (int i = 0; i < 12; i++)
    {
        int *converted = string_to_array_test(list[i]);

        if (converted)
        {
            printf("Valid :   ");
            for (int j = 0; j < 4; j++)
            {
                printf("%d   ", converted[j]);
            } 
            printf("\n");
        }
        else
        {
            printf("Invalid : %s\n", list[i]);
        }

    }

    int *f = string_to_array_test(final);
    
    for (int k = 0; k < 29; k++)
    {
        printf("%d ", f[k]);
    }

    return 0;
}