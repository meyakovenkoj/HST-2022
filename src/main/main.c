#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "input.h"
#include "plain.h"


int main(int argc, char **argv)
{
    char file1[100];
    char file2[100];
    int size;
    int length;
    FILE *fp1;
    FILE *fp2;
    int binary_mod = BINARY;

    if (argc == 2 && strncmp(argv[1], "-h", 2) == 0) {
        binary_mod = HUMAN;
    }


    printf("enter file 1: ");
    scanf("%s", file1);

    if ((fp1 = fopen(file1, GetMode(binary_mod, READ))) == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return EXIT_FAILURE;
	}

    printf("enter file 2: ");
    scanf("%s", file2);


    if ((fp2 = fopen(file2, GetMode(binary_mod, READ))) == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return EXIT_FAILURE;
	}

    scanf("%d %d", &size, &length);
    int count = size / length;
    float *result = (float *)calloc(count, sizeof(float));
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    plain(binary_mod, fp1, fp2, count, length, result);
    end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf(" %f sec elapsed, %d MBs processed\n", cpu_time_used, size >> 18);
    const char *outname = "result.txt";
    FILE *outfp = NULL;
    if ((outfp = fopen(outname, GetMode(binary_mod, WRITE))) == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return EXIT_FAILURE;
	}
    WriteData(binary_mod, count, result, outfp);
    return EXIT_SUCCESS;
}