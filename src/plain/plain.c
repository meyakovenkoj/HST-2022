#include "plain.h"
#include "input.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float multiply(int size, float *first_arr, float *second_arr)
{
    float res = 0;
    for (int i = 0; i < size; i++) {
        res += first_arr[i] * second_arr[i];
    }
    return res;
}

float process(int size, float *first_arr, float *second_arr)
{
    float uv = multiply(size, first_arr, second_arr);
    float uu = multiply(size, first_arr, first_arr);
    float vv = multiply(size, second_arr, second_arr);
    float cosine = uv / (sqrtf(uu) * sqrtf(vv));
    return cosine;
}

void clean_process(float *array1, float *array2, int count, int length, float *result)
{
    for (int k = 0; k < count; k++) {
        result[k] = process(length, array1 + k * length, array2 + k * length);
    }
}