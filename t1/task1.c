#include <stdio.h>
#include <stdlib.h>

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

int main()
{
    raw dataA = from_base64("QWNvcmRhUGVkcmluaG9RdWVob2pldGVtY2FtcGVvbmF0bw==");
    char *result_str = to_base64(dataA);
    printf("Resultado reverso: %s\n", result_str);

    return 0;
}