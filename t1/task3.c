#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "representation.h"
#include "attack.h"

#define MAX_KEYSIZE 40

int hamming_distance(raw a, raw b)
{
    int distance = 0;

    for (int i = 0; i < a.size; i++)
    {
        byte tmp = a.data[i] ^ b.data[i];

        for (int bit = 0; bit < 8; bit++)
        {
            if (tmp & (1 << bit))
            {
                distance++;
            }
        }
    }   
    return distance;
}

int min_at_bounded(float arr[], int size, int lower, int upper)
{
    if (upper == -1)
    {
        upper = size;    
    }
    
    float smallest = arr[lower];
    int smallest_at = lower;
    for (int i = lower+1; i < upper; i++)
    {   
        if (arr[i] < smallest)
        {
            smallest = arr[i];
            smallest_at = i;
        }
    }
    
    return smallest_at;
}

raw *generate_blocks(raw data, int keysize, int n_blocks)
{
    raw *blocks = (raw *)malloc(sizeof(raw) * n_blocks);
    
    for (int i = 0; i < n_blocks; i++)
    {
        blocks[i].size = keysize;
        blocks[i].data = (byte *)malloc(sizeof(byte) * keysize);

        for (int b = 0; b < keysize; b++)
        {
            if (i*keysize + b < data.size)
            {
                blocks[i].data[b] = data.data[i*keysize + b];
            }
            
            else
            {
                printf("Out of bounds read attempt at raw *generate_blocks(). Try fewer blocks.\n");
            }
        }
    }
    
    return blocks;
}

void kill_generated_blocks(raw *blocks, int n_blocks)
{
    for (int i = 0; i < n_blocks; i++)
    {
        free(blocks[i].data);
    }

    free(blocks);
}

float pairwise_avg(raw *blocks, int n_blocks, int keysize)
{
    int total = 0;

    for (int i = 0; i < n_blocks; i++)
    {
        for (int j = i+1; j < n_blocks; j++)
        {
            total += hamming_distance(blocks[i], blocks[j]);   
        }
    }

    return total / (float)(n_blocks*keysize);
}

int find_keysize(raw data, int n_blocks)
{
    float keysize_scores[MAX_KEYSIZE] = {0};
    for (int keysize = 2; keysize < MAX_KEYSIZE; keysize++)
    {
        raw *blocks = generate_blocks(data, keysize, n_blocks);
        keysize_scores[keysize] = pairwise_avg(blocks, n_blocks, keysize);
    }
    
    int best_at = min_at_bounded(keysize_scores, MAX_KEYSIZE, 2, -1);
    printf("Found best keysize of %d, with score of %f.\n", best_at, keysize_scores[best_at]);

    return best_at;
}

byte best_key(raw data)
{
    float scores[255] = {0};
    char *tmp = (char *)malloc(sizeof(char) * (data.size+1));
    tmp[data.size] = '\0';

    for (int c = 0; c < 256; c++)
    {
        for (int b = 0; b < data.size; b++)
        {
            tmp[b] = data.data[b] ^ (byte)c;
        }

        float score = validate_text("pt-br", "char_frequency", tmp);
        scores[c] = score;
    }

    return (byte)min_at_bounded(scores, 256, 0, -1);
}

int main()
{
    char *cypher_text = from_file("./data/task3data.txt");
    raw data = from_base64(cypher_text);
    printf("Read data.\n");
   
    int expected_keysize = find_keysize(data, 25);
    raw key;
    key.size = expected_keysize;
    key.data = (byte *)malloc(sizeof(byte) * expected_keysize);

    printf("Attempting bruteforce crack.\n");
    for (int index = 0; index < expected_keysize; index++)
    {
        int transposition_index = 0;
        raw transposed;
        transposed.size = ceil(data.size/(float)expected_keysize);
        transposed.data = (byte *)malloc(sizeof(byte) * data.size);

        for (int pos = 0; pos < data.size; pos += expected_keysize)
        {
            transposed.data[transposition_index] = data.data[pos+index];
            transposition_index++;
        }

        transposed.size = transposition_index;
        key.data[index] = best_key(transposed);
        printf("For index %d, best key is '%c'\n", index, key.data[index]);
    }

    printf("Expected key: ");
    for (int index = 0; index < key.size; index++)
    {
        printf("%c", key.data[index]);
    }
    printf("\n");

    printf("Decoded message: ");
    for (int b = 0; b < data.size; b++)
    {
        printf("%c", data.data[b] ^ key.data[b%key.size]);
    }

    return 0;
}