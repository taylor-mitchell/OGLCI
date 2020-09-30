#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Array2D.h"

#define cudaCall(x) if (x != cudaSuccess){ return false;}


bool matrixMultiply(Array2D<float> left, Array2D<float> right, Array2D<float>& result);

__global__ void multiplyKernel(float* left, int leftHeight, float* right, int rightWidth, float* result)
{
    int ROW = blockIdx.y * blockDim.y + threadIdx.y;
    int COL = blockIdx.x * blockDim.x + threadIdx.x;

    if (ROW < leftHeight && COL < rightWidth) {
        float tmpSum = 0.0f;

        for (int i = 0; i < rightWidth; i++) {
            tmpSum += left[ROW * rightWidth + i] * right[i * rightWidth + COL];
        }
        result[ROW * rightWidth + COL] = tmpSum;
    }
}

bool cudaMultiply(Array2D<float>& left, Array2D<float>& right, Array2D<float>& result)
{
    float* dev_left;
    float* dev_right;
    float* dev_result;

    float* cpu_result = new float[left.getHeight() * right.getWidth()];

    dim3 threadsPerBlock(left.getHeight(), right.getWidth());
    dim3 blocksPerGrid(1, 1);

    if (left.getHeight() * right.getWidth() > 512) {
        threadsPerBlock.x = 512;
        threadsPerBlock.y = 512;
        blocksPerGrid.x = ceil(double(right.getWidth()) / double(threadsPerBlock.x));
        blocksPerGrid.y = ceil(double(left.getHeight()) / double(threadsPerBlock.y));
    }

    cudaCall(cudaSetDevice(0));
    cudaCall(cudaMalloc((void**)&dev_left, left.getHeight() * left.getWidth() * sizeof(float)));
    cudaCall(cudaMalloc((void**)&dev_right, right.getHeight() * right.getWidth() * sizeof(float)));
    cudaCall(cudaMalloc((void**)&dev_result, left.getHeight() * right.getWidth() * sizeof(float)));

    cudaCall(cudaMemcpy(dev_left, left.getData(), left.getWidth() * left.getHeight() * sizeof(float), cudaMemcpyHostToDevice));
    cudaCall(cudaMemcpy(dev_right, right.getData(), right.getWidth() * right.getHeight() * sizeof(float), cudaMemcpyHostToDevice));

    multiplyKernel << <blocksPerGrid, threadsPerBlock >> > (dev_left, left.getHeight(), dev_right, right.getWidth(), dev_result);

    cudaCall(cudaGetLastError());
    cudaCall(cudaDeviceSynchronize());
    cudaCall(cudaMemcpy(cpu_result, dev_result, left.getHeight() * right.getWidth() * sizeof(float), cudaMemcpyDeviceToHost));

    cudaFree(dev_left);
    cudaFree(dev_right);
    cudaFree(dev_result);

    result.setData(right.getWidth(), left.getHeight(), cpu_result);
    return true;
}
