#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Tarefa 1 de Criptografia para Segurança de Dados
Uso de xor de mesmo tamanho para criptografar
*/

#include "representation.h"

raw xor(raw A, raw B)
{
    raw tmp;
    tmp.size = A.size;
    tmp.data = (byte *)malloc(tmp.size * sizeof(byte));
    for (int i = 0; i < tmp.size; i++)
    {
        tmp.data[i] = A.data[i] ^ B.data[i];
    }

    return tmp;
}

void help()
{
    printf("Help:\n");
    printf("\thelp: help menu.\n");
    printf("\thex-to-base64 [DATA]: takes hex input and converts to base64.\n");
    printf("\tbase64-to-hex [DATA]: takes base64 input and converts to hex.\n");
    printf("\txor [input=base64/input=hex] [DATA] [DATA]: takes two inputs and outputs their xor.\n");
}

int main(int argc, char *argv[])
{
    if (argc >= 2 && argc <= 5)
    {
        if (strcmp(argv[1], "help") == 0)
        {
            help();
        }
        
        else if (strcmp(argv[1], "hex-to-base64") == 0 && argc == 3)
        {
            raw data = from_hex(argv[2]);
            char *string = to_base64(data);
            printf("BASE64: %s\n", string);
            free(string);
        }

        else if (strcmp(argv[1], "base64-to-hex") == 0 && argc == 3)
        {
            raw data = from_base64(argv[2]);
            char *string = to_hex(data);
            printf("HEX: 0x%s\n", string);
            free(string);
        }

        else if (strcmp(argv[1], "xor") == 0  && argc == 5)
        {
            raw dataA, dataB;

            if (strcmp(argv[2], "input=base64") == 0)
            {
                dataA = from_base64(argv[3]);
                dataB = from_base64(argv[4]);
            }

            else if (strcmp(argv[2], "input=hex") == 0)
            {
                dataA = from_hex(argv[3]);
                dataB = from_hex(argv[4]);
            }

            else
            {
                printf("Invalid option. Try using 'help' for help.");
                return 0;
            }
            
            raw result = xor(dataA, dataB);
            printf("RESULT:\n");
            char *string = to_hex(result);
            printf("HEX: 0x%s\n", string);
            free(string);
            string = to_base64(result);
            printf("BASE64: %s\n", string);
            free(string);
        }
    }

    else
    {
        printf("Invalid option. Try using 'help' for help.");
    }
    
    return 0;
}