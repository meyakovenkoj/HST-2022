#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "input.h"
#include "plain.h"

int main(int argc, char **argv)
{
    if (argc != 6) {
        fprintf(stderr, "Bad args\n");
        return EXIT_FAILURE;
    }
    int k = atoi(argv[1]);
    int size = atoi(argv[2]);
    int length = atoi(argv[3]);
    const char *name1 = argv[4];
    const char *name2 = argv[5];
    FILE *fp1, *fp2;
    if ((fp1 = fopen(name1, GetMode(BINARY, READ))) == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return EXIT_FAILURE;
	}


    if ((fp2 = fopen(name2, GetMode(BINARY, READ))) == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return EXIT_FAILURE;
	}
    
    int count = size / length;
    float *result = (float *)calloc(count, sizeof(float));
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    for (int i = 0; i < k; i++) {
        plain(BINARY, fp1, fp2, count, length, result);
        fseek(fp1, 0, SEEK_SET);
        fseek(fp2, 0, SEEK_SET);
    }
    end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("%f sec elapsed\n", cpu_time_used);
    return 0;
}