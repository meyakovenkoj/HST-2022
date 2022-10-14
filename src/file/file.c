#include "input.h"
#include <stdio.h>
#include <stdlib.h>

float GetNumber(FILE *fd)
{
    float result = 0;
    if (fscanf(fd, "%f ", &result) != 1) {
        if (fscanf(fd, "%f\n", &result) != 1)
            return 0;
    }
    return result;
}

int ReadDataH(int size, float **array, FILE *fd)
{
    float *buffer = NULL;
    int errorCode = 1;
    if (size < 1 || !array || !fd) {
        goto done;
    }
    buffer = (float *)malloc(size);
    if (!buffer) {
        goto done;
    }
    int i = 0;
    float *tmp = malloc(sizeof(float) * size);
    for (i = 0; i < size; i++) {
        tmp[i] = GetNumber(fd);
    }

    *array = tmp;

    errorCode = 0;
done:
    return errorCode;
}

int WriteDataH(int size, float *array, FILE *fd)
{
    int i = 0;
    for (i = 0; i < size; i++) {
        fprintf(fd, "%.6f", array[i]);
        if (i != size - 1) {
            fprintf(fd, " ");
        }
    }
    fprintf(fd, "\n");
    return 0;
}