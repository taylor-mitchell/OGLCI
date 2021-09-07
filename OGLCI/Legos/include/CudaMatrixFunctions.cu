#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

__global__ void multiplyKernel(int resultWidth, int resultHeight, int leftWidth, int valueColsPerThread, int valueRowsPerThread, double* left, double* right, double* result)
{
    int ROW = blockIdx.y * blockDim.y + threadIdx.y;
    int COL = blockIdx.x * blockDim.x + threadIdx.x;

    int totalGridHeight = blockDim.y * gridDim.y * valueRowsPerThread;
    int totalGridWidth = blockDim.x * gridDim.x * valueColsPerThread;

    for (int i = 0; i < valueColsPerThread; ++i)
    {
        int currentRow = ROW + i;
        for (int j = 0; j < valueRowsPerThread; ++j)
        {
            int currentCol = COL + j;
            if (currentRow < resultHeight && currentCol < resultWidth)
            {
                double tmpSum = 0.0f;

                for (int k = 0; k < leftWidth; ++k)
                {
                    double leftValue = left[(currentRow)*leftWidth + k];
                    double rightValue = right[k * resultWidth + currentCol];
                    tmpSum += leftValue * rightValue;
                }
                result[(currentRow)*resultWidth + currentCol] = tmpSum;
            }

            //outer threads may need to do extra work because the matrix might not fit nicely on the grid
            if (blockIdx.y + 1 == gridDim.y && totalGridHeight < resultHeight)
            {
                int extraRow = currentRow + blockDim.y;

                if (extraRow < resultHeight && currentCol < resultWidth)
                {
                    double tmpSum = 0.0f;

                    for (int k = 0; k < leftWidth; ++k)
                    {
                        double leftValue = left[(extraRow)*leftWidth + k];
                        double rightValue = right[k * resultWidth + currentCol];
                        tmpSum += leftValue * rightValue;
                    }
                    result[(extraRow)*resultWidth + currentCol] = tmpSum;
                }
            }

            if (blockIdx.x + 1 == gridDim.x && totalGridWidth < resultWidth)
            {
                int extraCol = currentCol + blockDim.x;

                if (currentRow < resultHeight && extraCol < resultWidth)
                {
                    double tmpSum = 0.0f;

                    for (int k = 0; k < leftWidth; ++k)
                    {
                        double leftValue = left[(currentRow)*leftWidth + k];
                        double rightValue = right[k * resultWidth + extraCol];
                        tmpSum += leftValue * rightValue;
                    }
                    result[(currentRow)*resultWidth + extraCol] = tmpSum;
                }
            }

            if (blockIdx.x + 1 == gridDim.x && totalGridWidth < resultWidth && blockIdx.y + 1 == gridDim.y && totalGridHeight < resultHeight)
            {
                int extraCol = currentCol + blockDim.x;
                int extraRow = currentRow + blockDim.y;
                if (extraRow < resultHeight && extraCol < resultWidth)
                {
                    float tmpSum = 0.0f;

                    for (int k = 0; k < leftWidth; ++k)
                    {
                        double leftValue = left[(extraRow)*leftWidth + k];
                        double rightValue = right[k * resultWidth + extraCol];
                        tmpSum += leftValue * rightValue;
                    }
                    result[(extraRow)*resultWidth + extraCol] = tmpSum;
                }
            }
        }
    }
}

cudaError launchMultiplyKernel(dim3 blocksPerGrid, dim3 threadsPerBlock, int valueColsPerThread, int valueRowsPerThread, int resultWidth, int resultHeight, int leftWidth, double* dev_left, double* dev_right, double* dev_result)
{
    multiplyKernel << <blocksPerGrid, threadsPerBlock >> > (resultWidth, resultHeight, leftWidth, valueColsPerThread, valueRowsPerThread, dev_left, dev_right, dev_result);
    return cudaGetLastError();
}

