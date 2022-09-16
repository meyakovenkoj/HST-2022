#ifndef __INPUT_H__
#define __INPUT_H__
#include <stdio.h>
#include <stdlib.h>


enum BINARY_MOD {
    BINARY = 0,
    HUMAN
};

enum WORK_MOD {
    READ = 0,
    WRITE
};

const char *GetMode(int binary_mod, int isWrite);

int ReadData(int binary_mod, int size, float **array, FILE *fd);

int WriteData(int binary_mod, int size, float *array, FILE *fd);

#endif