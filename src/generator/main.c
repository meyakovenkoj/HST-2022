#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>
#include "input.h"


enum BYTE_MOD {
    BYTE = 0,
    KILOBYTE,
    MEGABYTE
};



int main(int argc, char **argv)
{
    char *address = NULL;
    int length = 0;
    int size = 0;
    int byte_mod = MEGABYTE;
    int binary_mod = BINARY;
    int c;


    opterr = 0;

    while ((c = getopt (argc, argv, "a:hbkl:s:")) != -1)
        switch (c)
        {
        case 'a':
            address = optarg;
            break;
        case 'h':
            binary_mod = HUMAN;
            break;
        case 'b':
            byte_mod = BYTE;
            break;
        case 'k':
            byte_mod = KILOBYTE;
            break;
        case 'l':
            length = atoi(optarg);
            break;
        case 's':
            size = atoi(optarg);
            break;
        case '?':
            if (optopt == 'a' || optopt == 'l' || optopt == 's')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
            return 1;
        default:
            abort();
        }

    printf ("address = %s, length = %d, size = %d\n",
            address, length, size);
    
    int multiplyer = 1;
    const char *suffix;
    switch (byte_mod)
    {
    case MEGABYTE:
        multiplyer *= 1024*1024;
        suffix = "mb";
        break;
    case KILOBYTE:
        multiplyer *= 1024;
        suffix = "kb";
        break;
    case BYTE:
        multiplyer *= 1;
        suffix = "b";
        break;
    default:
        suffix = "mb";
        break;
    }
    size_t real_amount = size * multiplyer / sizeof(float);
    size_t real_count = real_amount / length;
    real_amount = real_count * length;
    FILE *finput;
    char name[100];
    sprintf(name, "gen_%04d%s_1.txt", size, suffix);

    if ((finput = fopen(name, GetMode(binary_mod, WRITE))) == NULL)
	{
		printf("\nError opening file\n");
		return -1;
	}
    
    srand(time(NULL));
    float *array = malloc(length * sizeof(float));
    for (int n = 0; n < real_count; n++) {
        for (int m = 0; m < length; m++) {
            array[m] = (float)rand() / (float)(RAND_MAX);
        }
        WriteData(binary_mod, length, array, finput);
    }
    printf("Total amount = %zu, line size = %d, row number = %zu\n", real_amount, length, real_count);
    free(array);
	fclose(finput);

    FILE *finput2;
    char name2[100];
    sprintf(name2, "gen_%04d%s_2.txt", size, suffix);

    if ((finput2 = fopen(name2, GetMode(binary_mod, WRITE))) == NULL)
	{
		printf("\nError opening file\n");
		return -1;
	}
    
    // srand(time(NULL));
    srand(12);
    float *array2 = malloc(length * sizeof(float));
    for (int n = 0; n < real_count; n++) {
        for (int m = 0; m < length; m++) {
            array2[m] = (float)rand() / (float)(RAND_MAX);
        }
        WriteData(binary_mod, length, array2, finput2);
    }
    printf("Total amount = %zu, line size = %d, row number = %zu\n", real_amount, length, real_count);
    free(array2);
	fclose(finput2);
    return 0;
}