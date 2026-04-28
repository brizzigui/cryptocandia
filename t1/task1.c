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
    printf("Conversao BASE64 -> HEX:\n");
    raw dataA = from_base64("QWNvcmRhUGVkcmluaG9RdWVob2pldGVtY2FtcGVvbmF0bw==");
    char *result_A = to_hex(dataA);
    printf("Resultado: %s\n", result_A);

    printf("Conversao HEX -> BASE64:\n");
    raw dataB = from_hex("41636f72646150656472696e686f517565686f6a6574656d63616d70656f6e61746f");
    char *result_B = to_base64(dataB);
    printf("Resultado: %s\n", result_B);

    printf("XOR entre dois dados:\n");
    raw plain = from_hex("41636f72646150656472696e686f517565686f6a6574656d63616d70656f6e61746f");
    raw key = from_hex("0b021e0701003e0a0d060c0807063d1a0b0f0e060a1a020c0f0e03170403010f130e");
    raw cipher = xor(plain, key);

    char *result_cipher_hex = to_hex(cipher);
    printf("Resultado em HEX: %s\n", result_cipher_hex);
    free(result_cipher_hex);
    char *result_cipher_base64 = to_base64(cipher);
    printf("Resultado em BASE64: %s\n", result_cipher_base64);

    return 0;
}