#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>


__global__ void multiplyKernel(int resultWidth, int resultHeight, int leftWidth, float* left, float* right, float* result/*, int valueColsPerThread, int valueRowsPerThread*/)
{
    int ROW = blockIdx.y * blockDim.y + threadIdx.y;
    int COL = blockIdx.x * blockDim.x + threadIdx.x;

    int tempy = blockDim.y * gridDim.y;
    int tempx = blockDim.x * gridDim.x;
    //int height = valueColsPerThread * gridDim.y * blockDim.y;
    //int width = valueRowsPerThread * gridDim.x * blockDim.x;    

    /*for (int i = 0; i < valueColsPerThread; ++i)
    {
        for (int j = 0; j < valueRowsPerThread; ++j)
        {
            if (ROW + i < height && COL + j < width) 
            {

                float tmpSum = 0.0f;
                
                for (int k = 0; k < leftWidth; ++k)
                {
                    float leftValue = left[(ROW + i) * leftWidth + k];
                    float rightValue = right[k * width + COL + j];                    
                    tmpSum += leftValue * rightValue;
                }
                result[(ROW + i) * width + COL + j] = tmpSum;
            }
        }
    }*/
    if (ROW < resultHeight && COL < resultWidth)
    {

        float tmpSum = 0.0f;

        for (int k = 0; k < leftWidth; ++k)
        {
            float leftValue = left[(ROW) * leftWidth + k];
            float rightValue = right[k * resultWidth + COL];
            tmpSum += leftValue * rightValue;
        }
        result[(ROW) *resultWidth + COL] = tmpSum;
    }
   
    if (blockIdx.y + 1 == gridDim.y && tempy < resultHeight)
    {
        
        int ROW2 = ROW + blockDim.y;
        

        if (ROW2 < resultHeight && COL < resultWidth)
        {

            float tmpSum = 0.0f;

            for (int k = 0; k < leftWidth; ++k)
            {
                float leftValue = left[(ROW2)*leftWidth + k];
                float rightValue = right[k * resultWidth + COL];
                tmpSum += leftValue * rightValue;
            }
            result[(ROW2)*resultWidth + COL] = tmpSum;
        }
    }
    if (blockIdx.x + 1 == gridDim.x && tempx < resultWidth)
    {
        int COL2 = COL + blockDim.x;

        if (ROW < resultHeight && COL2 < resultWidth)
        {

            float tmpSum = 0.0f;

            for (int k = 0; k < leftWidth; ++k)
            {
                float leftValue = left[(ROW)*leftWidth + k];
                float rightValue = right[k * resultWidth + COL2];
                tmpSum += leftValue * rightValue;
            }
            result[(ROW)*resultWidth + COL2] = tmpSum;
        }
    }
    if (blockIdx.x + 1 == gridDim.x && tempx < resultWidth && blockIdx.y + 1 == gridDim.y && tempy < resultHeight)
    {
        int COL2 = COL + blockDim.x;
        int ROW2 = ROW + blockDim.y;
        if (ROW2 < resultHeight && COL2 < resultWidth)
        {
            float tmpSum = 0.0f;

            for (int k = 0; k < leftWidth; ++k)
            {
                float leftValue = left[(ROW2)*leftWidth + k];
                float rightValue = right[k * resultWidth + COL2];
                tmpSum += leftValue * rightValue;
            }
            result[(ROW2)*resultWidth + COL2] = tmpSum;
        }
    }
}

void launchMultiplyKernel(dim3 blocksPerGrid, dim3 threadsPerBlock, int resultWidth, int resultHeight, int leftWidth, float* dev_left, float* dev_right, float* dev_result)
{
    multiplyKernel << <blocksPerGrid, threadsPerBlock >> > (resultWidth, resultHeight, leftWidth, dev_left, dev_right, dev_result);
}
