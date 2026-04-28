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

int base64_padding_len(char *string)
{
    int last = strlen(string)-1;
    if (string[last-1] == '=')
    {
        return 2;
    }

    else if (string[last] == '=')
    {
        return 1;
    }

    return 0;
}

raw from_base64(char *string)
{
    int string_size = strlen(string) - base64_padding_len(string);

    raw data;
    data.size = (int)(3*string_size/4.0);
    data.data = (byte *)calloc(1, data.size * sizeof(byte));

    for (int i = 0; i < string_size; i++)
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

    string[2 * data.size] = '\0';

    return string;
}

int base64_padding_for_str(int size)
{
    if (size % 3 == 2)
    {
        return 1;
    }

    else if (size % 3 == 1)
    {
        return 2;
    }

    return 0;
}

char *to_base64(raw data)
{
    int str_size = (int)ceil(4 * data.size/3.0) + 1;
    int padding_size = base64_padding_for_str(data.size);
    char *string = (char *)malloc((str_size+padding_size) * sizeof(char));
    byte a = 0, b = 0;

    for (int i = 0; i < str_size-1; i++)
    {
        if(i % 4 == 0)
        {
            if (3*(i/4) >= data.size) break;
            byte val = data.data[3*(i/4)];

            a |= val >> 2;
            string[i] = to_symbol(a);
            a = 0;

            b |= val << 4;
            b &= 0b00110000;
        }

        else if (i % 4 == 1)
        {
            if (3*(i/4)+1 >= data.size)
            {
                string[i] = to_symbol(b);
                break;
            }

            byte val = data.data[3*(i/4) + 1];

            b |= val >> 4;
            string[i] = to_symbol(b);
            b = 0;
            
            a |= val << 2;
            a &= 0b00111100;
        }

        else if (i % 4 == 2)
        {
            if (3*(i/4)+2 >= data.size)
            {
                string[i] = to_symbol(a);
                break;
            }

            byte val = data.data[3*(i/4) + 2];

            a |= val >> 6;
            string[i] = to_symbol(a);
            a = 0;

            i++;

            b = val & 0b00111111;
            string[i] = to_symbol(b);
            b = 0;
        }
    }

    if (padding_size == 2)
    {
        string[str_size+padding_size-3] = '=';
        string[str_size+padding_size-2] = '=';
    }

    if (padding_size == 1)
        string[str_size+padding_size-2] = '=';
    
    
    string[str_size+padding_size-1] = '\0';
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