#include "input.h"
#include <stdio.h>
#include <stdlib.h>

int ReadDataB(int size, float **array, FILE *fd)
{
    float *buffer = NULL;
    int err = 1;
    if (size < 1 || !array || !fd) {
        goto err;
    }
    buffer = (float *)malloc(size);
    if (!buffer) {
        goto err;
    }

    float *tmp = malloc(sizeof(float) * size);
    int res = fread(tmp, sizeof(float), size, fd);
    if (res != size) {
        perror("fread failed");
        err = 12;
        goto err;
    }

    *array = tmp;

    err = 0;
err:
    return err;
}

int WriteDataB(int size, float *array, FILE *fd)
{
    int bytes_wrote = fwrite(array, sizeof(float), size, fd);
    if (bytes_wrote != size) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}