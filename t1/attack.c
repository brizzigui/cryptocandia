#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "attack.h"

/*
    Checks if a string is likely decrypted plaintext or a useless attempt.
    `locale` is a const char *, accepted = {`"pt-br"`}
    `method` is a const char *, accepted = {`"char_frequency"`}
    `string` is a char *, needs to be `NULL` terminated.
*/
float validate_text(const char *locale, const char *method, char* string)
{
    float score = 0;
    float ngram[26] = {0};
    if (strcmp("pt-br", locale) == 0)
    {
        float pt_br_ngram[] = {0.1463, 0.0104, 0.0388, 0.0499, 0.1257, 0.0102, 0.013, 0.0128, 0.0618, 0.004, 0.0002, 0.0278, 0.0474, 0.0505, 0.1073, 0.0252, 0.1073, 0.0252, 0.012, 0.0653, 0.0781, 0.0434, 0.0463, 0.0167, 0.0001, 0.0021, 0.0001, 0.0047};
        for (int i = 0; i < 26; i++)
        {
            ngram[i] = pt_br_ngram[i];
        }
        
    }

    int str_size = strlen(string);
    float counter[26] = {0};
    for (int i = 0; i < str_size; i++)
    {
        if(string[i] >= 'A' && string[i] <= 'Z')
        {
            counter[(int)(string[i] - 'A')]++;
        }

        else if(string[i] >= 'a' && string[i] <= 'z')
        {
            counter[(int)(string[i] - 'a')]++;
        }
    }
    
    for (int i = 0; i < 26; i++)
    {
        counter[i] = counter[i]/(float)str_size;
        float val = counter[i] - ngram[i];
        val = (val < 0) ? -val : val;
        score += val;
    }
    
    return score;
}

void brute_force(raw data, float threashold)
{
    char *tmp = (char *)malloc(sizeof(char) * (data.size+1));
    tmp[data.size] = '\0';

    for (int c = 0; c < 256; c++)
    {
        for (int b = 0; b < data.size; b++)
        {
            tmp[b] = data.data[b] ^ (byte)c;
        }

        float score = validate_text("pt-br", "char_frequency", tmp);
        if(score < threashold)
        {
            printf("Found possible solution using key '%c'.\n", c);
            printf("Score = %f\n", score);
            printf("Decoded text:\n%s\n\n", tmp);
        }
    }
}