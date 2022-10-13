#include "plain.h"
#include "input.h"
#include <mpi.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

float process(int size, float *first_arr, float *second_arr)
{
    float uv = 0;
    float uu = 0;
    float vv = 0;
    for (int i = 0; i < size; i++) {
        uv += first_arr[i] * second_arr[i];
        uu += first_arr[i] * first_arr[i];
        vv += second_arr[i] * second_arr[i];
    }
    float cosine = uv / (sqrtf(uu) * sqrtf(vv));
    return cosine;
}

void clean_process(float *array1, float *array2, int count, int length, float *result)
{
    for (int k = 0; k < count; k++) {
        result[k] = process(length, array1 + k * length, array2 + k * length);
    }
}