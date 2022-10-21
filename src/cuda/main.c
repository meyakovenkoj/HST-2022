#include <unistd.h>
#include <getopt.h>
#include "input.h"
#include "cuda_process.h"
#include "common.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
    int error_code = EXIT_FAILURE;
    int opt;
    int sfnd = 0;
    int lfnd = 0;
    int size = 0;
    int length = 0;
    int binary_mod = BINARY;
    while ((opt = getopt(argc, argv, "hs:l:")) != -1) {
        switch (opt) {
        case 'h':
            binary_mod = HUMAN;
            break;
        case 's':
            size = atoi(optarg);
            sfnd = 1;
            break;
        case 'l':
            length = atoi(optarg);
            lfnd = 1;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s -s size -l length [-h] filename1 filename2\n",
                    argv[0]);
            error_code = EXIT_FAILURE;
            CHECK_ERROR_CODE(error_code);
        }
    }

    if (!(sfnd && lfnd)) {
        fprintf(stderr, "Expected size and length options\n");
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    if (optind != (argc - 2)) {
        fprintf(stderr, "Expected two filenames after options\n");
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    int vectors_count = size / length;
    const char *name1 = argv[optind];
    const char *name2 = argv[optind + 1];
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
    float *dev_array1 = NULL;
    float *dev_array2 = NULL;
    float *dev_result = NULL;
    ReadData(BINARY, size, &array1, fp1);
    ReadData(BINARY, size, &array2, fp2);
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    gpu_process(array1, array2, count, length, result, &dev_array1, &dev_array2, &dev_result);
    gpu_clean_and_sync(dev_array1, dev_array2, dev_result);
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

done:
    return 0;
}