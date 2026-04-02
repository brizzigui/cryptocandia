#ifndef REPRESENTATION_H
#define REPRESENTATION_H

typedef unsigned char byte;
struct raw 
{
    int size;
    byte *data;
};

typedef struct raw raw;

raw from_hex(char *string);
raw from_base64(char *string);
char *to_hex(raw data);
char *to_base64(raw data);
char *from_file(const char* path);
void print_ascii(raw data);

#endif