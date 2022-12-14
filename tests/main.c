#include "input.h"
#include "plain.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr, "Bad args\n");
        return EXIT_FAILURE;
    }
    int size = atoi(argv[1]);
    int length = atoi(argv[2]);
    const char *name1 = argv[3];
    const char *name2 = argv[4];
    FILE *fp1, *fp2;
    if ((fp1 = fopen(name1, GetMode(BINARY, READ))) == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    if ((fp2 = fopen(name2, GetMode(BINARY, READ))) == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }

    int count = size / length;
    float *result = (float *)calloc(count, sizeof(float));

    float *array1 = NULL;
    float *array2 = NULL;
    ReadData(BINARY, size, &array1, fp1);
    ReadData(BINARY, size, &array2, fp2);
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    clean_process(array1, array2, count, length, result);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%f sec elapsed\n", cpu_time_used);
    const char *outname = "result.txt";
    FILE *outfp = NULL;
    if ((outfp = fopen(outname, GetMode(BINARY, WRITE))) == NULL) {
        fprintf(stderr, "Error opening file\n");
        return EXIT_FAILURE;
    }
    WriteData(BINARY, count, result, outfp);
    return 0;
}