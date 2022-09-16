#ifndef __FILE_H__
#define __FILE_H__
#include <stdio.h>
#include <stdlib.h>

int ReadDataB(int size, float **array, FILE *fd);

int WriteDataB(int size, float *array, FILE *fd);

int ReadDataH(int size, float **array, FILE *fd);

int WriteDataH(int size, float *array, FILE *fd);

#endif
