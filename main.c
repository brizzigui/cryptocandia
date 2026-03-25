#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char byte;

struct raw 
{
    int size;
    byte *data;
};

typedef struct raw raw;

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

raw from_hex(char *string)
{
    raw data;
    data.size = strlen(string) / 2;
    data.data = (byte *)malloc(sizeof(byte));

    for (int i = 0; i < data.size; i += 2)
    {
        sscanf(&string[i], "%x", &data.data[i]);
    }
    
    return data;
}

byte to_number(char c)
{
    if (c == '+')   
    {
        return (byte)62;
    }
    
    if (c == '/')   
    {
        return (byte)63;
    }

    if (c >= '0' && c <= '9')   
    {
        return (byte)(c - '0' + 52);
    }

    return (byte)(c - 'A');   
}

raw from_base64(char *string)
{
    raw data;
    data.size = strlen(string) * 2;
    data.data = (byte *)malloc(sizeof(byte));

    int mask = 0b11111100;

    for (int i = 0; i < (int)strlen(string); i++)
    {
        byte val = to_number(string[i]);
        byte mask = 0b11111100;

        printf("%x", mask);
    }
    
}

char *to_hex(raw data)
{
    char *string = (char *)malloc(data.size * sizeof(char));
    
    for (int i = 0; i < data.size; i += 2)
    {
        sprintf(&string[i], "%x", data.data[i]);
    }

    return string;
}

char *to_base64(raw data)
{

}

int main()
{
    raw data = from_hex("1F");
    char *string = to_hex(data);
    printf("%s\n", string);
    raw data_A = from_hex("FFFFFFFF");
    raw data_B = from_hex("FFFFFFFF");

    raw result = xor(data_A, data_B);
    string = to_hex(result);
    printf("%s\n", string);

    return 0;
}