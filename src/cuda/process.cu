extern "C" {
#include <stdio.h>
#include "cuda_process.h"
}

#define CUDA_ERROR() \
    do { \
        cudaError_t __err = cudaGetLastError(); \
        if (__err) { \
            printf("File: %s, Line: %d, error code: %d\n", __FILE__, __LINE__, __err); \
            return; \
        } \
    } while(0)

__global__ void cosine(const float *array1, const float *array2, int count, int length, float *result)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= count) {
        return;
    }
    const float *first_arr = array1 + tid * length;
    const float *second_arr = array2 + tid * length;
    float uv = 0;
    float uu = 0;
    float vv = 0;
    for (int i = 0; i < length; i++) {
        uv += first_arr[i] * second_arr[i];
        uu += first_arr[i] * first_arr[i];
        vv += second_arr[i] * second_arr[i];
    }
    result[tid] = uv / (sqrtf(uu) * sqrtf(vv));
}

void gpu_process(float *array1, float *array2, int count, int length, float *result, float **dev_array1, float **dev_array2, float **dev_result)
{
    cudaMalloc((void**)dev_array1, count * length * sizeof(float));
    CUDA_ERROR();
    cudaMalloc((void**)dev_array2, count * length * sizeof(float));
    CUDA_ERROR();
    cudaMalloc((void**)dev_result, count * sizeof(float));
    CUDA_ERROR();

    cudaMemcpyAsync(*dev_array1, array1, count * length * sizeof(float), cudaMemcpyHostToDevice);
    CUDA_ERROR();
    cudaMemcpyAsync(*dev_array2, array2, count * length * sizeof(float), cudaMemcpyHostToDevice);
    CUDA_ERROR();

    int block_size = 1024;
    int grid_size = (count + block_size) / block_size;
    cosine<<<grid_size,block_size>>>(*dev_array1, *dev_array2, count, length, *dev_result);

    cudaMemcpyAsync(result, *dev_result, sizeof(float) * count, cudaMemcpyDeviceToHost);
    CUDA_ERROR();
}

void gpu_clean_and_sync(float *dev_array1, float *dev_array2, float *dev_result)
{
    cudaDeviceSynchronize();
    cudaFree(dev_array1);
    cudaFree(dev_array2);
    cudaFree(dev_result);
}
