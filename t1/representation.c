#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "representation.h"

raw from_hex(char *string)
{
    raw data;
    data.size = strlen(string) / 2;
    data.data = (byte *)malloc(sizeof(byte) * data.size);

    for (int i = 0; i < data.size; i++)
    {
        sscanf(&string[2*i], "%02hhx", &data.data[i]);
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

    if (c == '=')
    {
        return (byte)0;
    }

    printf("Invalid char used in conversion from base64. Check string.");
    exit(-1);
    
}


char to_symbol(byte b)
{
    if (b == 62)   
    {
        return '+';
    }
    
    if (b == 63)   
    {
        return '/';
    }

    if (b >= 52 && b <= 61)   
    {
        return (char)('0' + b - 52);
    }

    if (b >= 0 && b <= 25)
    {
        return (char)('A' + b);   
    }

    if (b >= 26 && b <= 51)
    {
        return (char)('a' + b - 26);   
    }



    printf("Invalid char used conversion to base64. Check string.");
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
    char *string = (char *)malloc((2 * data.size + 1) * sizeof(char));
    
    for (int i = 0; i < data.size; i++)
    {
        sprintf(&string[2*i], "%02x", data.data[i]);
    }

    return string;
}

char *to_base64(raw data)
{
    int str_size = (int)ceil(4 * data.size/3.0) + 1;
    char *string = (char *)malloc(str_size * sizeof(char));
    byte a, b = 0;

    for (int i = 0; i < str_size-1; i++)
    {
        if(i % 4 == 0)
        {
            byte val = data.data[3*(i/4)];

            a |= val >> 2;
            string[i] = to_symbol(a);
            a = 0;

            b |= val << 4;
            b &= 0b00110000;
        }

        else if (i % 4 == 1)
        {
            byte val = data.data[3*(i/4) + 1];

            b |= val >> 4;
            string[i] = to_symbol(b);
            b = 0;
            
            a |= val << 2;
            a &= 0b00111100;
        }

        else if (i % 4 == 2)
        {
            byte val = data.data[3*(i/4) + 2];
            a |= val >> 6;
            string[i] = to_symbol(a);
            a = 0;

            i++;

            b |= val;
            b &= 0b00111111;
            string[i] = to_symbol(b);
            b = 0;
        }
    }
    
    string[str_size-1] = '\0';
    return string;
}

char *from_file(const char* path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File not found. Exiting.");
        exit(-1);
    }
    
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *string = (char *)malloc(sizeof(char) * (size + 1));
    fscanf(file, "%s", string);

    return string;
}

void print_ascii(raw data)
{
    for (int i = 0; i < data.size; i++)
    {
        printf("%c", (char)data.data[i]);
    }
}