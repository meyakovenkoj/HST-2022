#ifndef __PLAIN_H__
#define __PLAIN_H__

#include <stdlib.h>
#include <stdio.h>

struct MpiConfig;

struct RunConfig
{
    struct MpiConfig *config;
    float *array1;
    float *array2;
    float *result;
    int *scatter_sizes;
    int *scatter_offsets;
    int *gather_counts;
    int *gather_offsets;
    int length;
    int count;
    int size;
    int vectors_count;
};

void clean_process(float *array1, float *array2, int count, int length, float *result);

#endif