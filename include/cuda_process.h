#ifndef __CUDA_PROCESS_H__
#define __CUDA_PROCESS_H__

void gpu_process(float *array1, float *array2, int count, int length, float *result, float **dev_array1, float **dev_array2, float **dev_result);

void gpu_clean_and_sync(float *dev_array1, float *dev_array2, float *dev_result);

#endif 