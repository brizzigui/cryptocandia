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
    raw dataA = from_hex("ABCDEF");
    raw dataB = from_hex("FEDCBA");
    raw result = xor(dataA, dataB);

    char *result_str = to_hex(result);
    printf("Resultado XOR: 0x%s\n", result_str);

    return 0;
}