#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>

#include "cachelab.h"

typedef struct cache_line
{
    int tag_num;
    int valid;
    int lru_counter;

} cache_line;

cache_line **cache;

/*64 bit address space*/
int m = 64;
int b, s;
int lru_counter = 0;

unsigned long long S, B;
unsigned long long E;
int tag; // tag bit size;
int INTMAX = 2147483647;

int hit_count = 0, miss_count = 0, eviction_count = 0;

void initialize_cache()
{
    int i, j;
    for (i = 0; i < S; i++)
    {
        for (j = 0; j < E; j++)
        {
            cache[i][j].tag_num = -1;
            cache[i][j].valid = 0;
            cache[i][j].lru_counter = -1;
        }
    }
}

void cache_operation(unsigned long long address)
{
    unsigned long long tag_num = address >> (s + b);
    unsigned long long set_num = (address >> b) & ((1<<s)-1);
    int hit = 0, empty = 0;

    for (int i = 0; i < E; i++)
    {
        /*cache hit!!*/
        if (!cache[set_num][i].valid)
            continue;
        if (cache[set_num][i].tag_num == tag_num)
        {
            hit = 1;
            hit_count++;
            cache[set_num][i].lru_counter = lru_counter++;
            return;
        }
    }
    if (!hit)
    {
        miss_count++;
        for (int i = 0; i < E; i++)
        {
            if (cache[set_num][i].valid)
                continue;
            cache[set_num][i].tag_num = tag_num;
            cache[set_num][i].valid = 1;
            cache[set_num][i].lru_counter = lru_counter++;
            empty = 1;
            break;
        }
    }
    int lru_temp = INTMAX;
    int lru_idx;
    if(!empty){
        for (int i = 0; i < E; i++)
        {
            if (cache[set_num][i].lru_counter<lru_temp){
                lru_temp = cache[set_num][i].lru_counter;
                lru_idx = i;
            }
        }
        eviction_count++;
        cache[set_num][lru_idx].tag_num = tag_num;
        cache[set_num][lru_idx].valid = 1;
        cache[set_num][lru_idx].lru_counter = lru_counter++;
    }
}

int main(int argc, char **argv)
{

    FILE *pFile; // pointer to FILE object
    char identifier;
    unsigned long long address;
    int size;
    int opt;
    char *fileName;

    while ((opt = getopt(argc, argv, "vhs:E:b:t:")) != -1)
    {
        switch (opt)
        {
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            fileName = optarg;
            break;
        default:
            printf("wrong argument\n");
            break;
        }
    }
    S = powl(2, s);
    B = powl(2, b);
    tag = m - (s + b);
    int i;
    cache = (cache_line **)(malloc(sizeof(cache_line *) * S));
    for (i = 0; i < S; i++)
    {
        *(cache + i) = (cache_line *)malloc(E * sizeof(cache_line));
    }

    initialize_cache();

    pFile = fopen(fileName, "r"); // open file for reading
    while (fscanf(pFile, "%c %llx,%d\n", &identifier, &address, &size) > 0)
    {
        if (identifier == 'M')
        {
            cache_operation(address);
            cache_operation(address);
        }
        else if (identifier == 'L')
        {
            cache_operation(address);
        }
        else if (identifier == 'S')
        {
            cache_operation(address);
        }
    }
    fclose(pFile);
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
