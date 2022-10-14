extern "C" {
#include "cuda_process.h"
}

__global__ void cosine(const float *array1, const float *array2, int count, int length, float *result) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int step = gridDim.x * blockDim.x;
    for(int ti = tid; ti < count; ti += step){
        const float *first_arr = array1 + ti * length;
        const float *second_arr = array2 + ti * length;
        float uv = 0;
        float uu = 0;
        float vv = 0;
        for (int i = 0; i < length; i++) {
            uv += first_arr[i] * second_arr[i];
            uu += first_arr[i] * first_arr[i];
            vv += second_arr[i] * second_arr[i];
        }
        result[blockIdx.x] = uv / (sqrtf(uu) * sqrtf(vv));
    }
}

void gpu_process(float *array1, float *array2, int count, int length, float *result) {
    float *dev_array1, *dev_array2, *dev_result;
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    cudaMalloc((void**)&dev_array1, count * length * sizeof(float));
    cudaMalloc((void**)&dev_array2, count * length * sizeof(float));
    cudaMalloc((void**)&dev_result, count * sizeof(float));

    cudaMemcpy(dev_array1, array1, count * length * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dev_array2, array2, count * length * sizeof(float), cudaMemcpyHostToDevice);

    int block_size = length < prop.maxThreadsPerBlock ? length : prop.maxThreadsPerBlock;
    int grid_size = (count + block_size) / block_size;
    cosine<<<grid_size,block_size>>>(dev_array1, dev_array2, count, length, dev_result);
    cudaDeviceSynchronize();

    cudaMemcpy(result, dev_result, sizeof(float) * count, cudaMemcpyDeviceToHost);

    cudaFree(dev_array1);
    cudaFree(dev_array2);
    cudaFree(dev_result);
}