#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include "../include/user_interface.h"
#include "../include/querier/query_parser.h"
#include "../include/querier/utils.h"

//! Para o professor que esteja a ler isto:
//! O processamento dos ficheiros pelo guião 1 não está a funcionar por razões descritas no relatório.
//! Para testar convém usar os ficheiros finais i.e: users-final.csv ; commits-final.csv ; repos-final.csv

int main(int argc, char *argv[])
{
     if (argc == 1)
     {
          file_state u = available_files(INPUT_PATH);
          build_files(u);
          interface();
     }
     else if (argc == 2)
     {
          if (file_exists(argv[1])) read_queries(argv[1]);
     }
     else
     {
          printf("Argumentos inválidos.\n");
          return 1;
     }
          
   
   return 0;
}   