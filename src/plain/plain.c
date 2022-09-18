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

int plain(int binary_mod, FILE *fp1, FILE *fp2, int count, int length, float *result)
{
    float *array1 = malloc(length);
    float *array2 = malloc(length);
    for (int k = 0; k < count; k++) {
        ReadData(binary_mod, length, &array1, fp1);
        ReadData(binary_mod, length, &array2, fp2);
        result[k] = process(length, array1, array2);
    }
    free(array1);
    free(array2);
    return 0;
}

void clean_process(float *array1, float *array2, int count, int length, float *result)
{
    for (int k = 0; k < count; k++) {
        result[k] = process(length, array1 + k * length, array2 + k * length);
    }
}