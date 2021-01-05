#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Array2D.h"

#define cudaCall(x) if (x != cudaSuccess){ return false;}

template<class T>
T* copyToDevice(Array2D<T>& data)
{
	T* devicePointer;
	cudaCall(cudaSetDevice(0));
	cudaCall(cudaMalloc((void**)&devicePointer, data.getHeight() * data.getWidth() * sizeof(T)));
	cudaCall(cudaMemcpy(devicePointer, data.getData(), data.getWidth() * data.getHeight() * sizeof(T), cudaMemcpyHostToDevice));
	return devicePointer;
}

template<class T>
T* copyToHost(Array2D<T>& data, unsigned int width, unsigned int height)
{
	T* devicePointer;
	cudaCall(cudaSetDevice(0));
	cudaCall(cudaMalloc((void**)&devicePointer, data.getHeight() * data.getWidth() * sizeof(T)));
	cudaCall(cudaMemcpy(devicePointer, data.getData(), data.getWidth() * data.getHeight() * sizeof(T), cudaMemcpyHostToDevice));
	return devicePointer;
}