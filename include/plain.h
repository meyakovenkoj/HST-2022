#ifndef __PLAIN_H__
#define __PLAIN_H__

#include <stdlib.h>
#include <stdio.h>

int plain(int binary_mod, FILE *fp1, FILE *fp2, int count, int length, float *result);

void clean_process(float *array1, float *array2, int count, int length, float *result);

#endif