/*
Tarefa 2 de Criptografia para Segurança de Dados
Decodificação força bruta de cifra xor com chave de apenas 1 byte
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "representation.h"
#include "attack.h"

int main()
{
    char *cypher_text = from_file("./data/task2data.txt");
    raw data = from_hex(cypher_text);
    brute_force(data, 0.5);

    return 0;
}