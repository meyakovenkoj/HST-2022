#include "input.h"
#include "file.h"

const char *GetMode(int binary_mod, int isWrite)
{
    if (binary_mod == BINARY) {
        return isWrite == 1 ? "wb" : "rb";
    }
    return isWrite == 1 ? "w" : "r";
}

int ReadData(int binary_mod, int size, float **array, FILE *fd)
{
#ifdef BINARY_ONLY
    return ReadDataB(size, array, fd);
#else
    if (binary_mod == BINARY) {
        return ReadDataB(size, array, fd);
    }
    else {
        return ReadDataH(size, array, fd);
    }
#endif
}

int WriteData(int binary_mod, int size, float *array, FILE *fd)
{
#ifdef BINARY_ONLY
    return WriteDataB(size, array, fd);
#else
    if (binary_mod == BINARY) {
        return WriteDataB(size, array, fd);
    } 
    else {
        return WriteDataH(size, array, fd);
    }
#endif
}