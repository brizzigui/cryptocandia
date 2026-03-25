#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

    if (c >= 'A' && c <= 'Z')
    {
        return (byte)(c - 'A');   
    }

    if (c >= 'a' && c <= 'z')
    {
        return (byte)(c - 'a' + (char)26);   
    }

    printf("Invalid char used in base64 conversion. Check string.");
    exit(-1);
    
}

raw from_base64(char *string)
{
    raw data;
    data.size = (int)ceil(3*strlen(string)/4.0);
    data.data = (byte *)calloc(1, data.size * sizeof(byte));

    for (int i = 0; i < (int)strlen(string); i++)
    {
        byte val = to_number(string[i]);

        if(i % 4 == 0)
        {
            data.data[3*(i/4)] |= val << 2;
        }

        else if (i % 4 == 1)
        {
            data.data[3*(i/4)] |= val >> 4;
            data.data[3*(i/4)+1] |= val << 4;
        }

        else if (i % 4 == 2)
        {
            data.data[3*(i/4)+1] |= val >> 2;
            data.data[3*(i/4)+2] |= val << 6;
        }

        else
        {
            data.data[3*(i/4)+2] |= val;
        }

    }      
    
    return data;
}

char *to_hex(raw data)
{
    char *string = (char *)malloc(2 * data.size * sizeof(char));
    
    for (int i = 0; i < data.size; i++)
    {
        sprintf(&string[2*i], "%x", data.data[i]);
    }

    return string;
}

char *to_base64(raw data)
{

}

int main()
{
    raw data = from_base64("abcdefgh");
    char *string = to_hex(data);
    printf("HEX string (full) = %s\n", string);

    return 0;
}