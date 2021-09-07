#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "Logger.h"

#define CUDA_ASSERT(x)  if (!(x)) __debugbreak();
#define cudaCall(x) CUDA_ASSERT(CudaCheckError(x, #x, __FILE__, __LINE__))
#define USE_CUDA

cudaDeviceProp getGPUProperties(int deviceNum)
{
    //i don't know how fast this is, but i could stash it pretty easily i think
	cudaDeviceProp toReturn;
	if (cudaSetDevice(deviceNum) == cudaSuccess)
	{
		cudaGetDeviceProperties(&toReturn, deviceNum);
	}
	return toReturn;
}

static std::string GetErrorString(cudaError error)
{
    std::string toReturn;
    switch (error)
    {
    case cudaSuccess:
        toReturn = "success";
        break;
    case cudaErrorInvalidValue:
        toReturn = "invalid value";
        break;
    case cudaErrorMemoryAllocation:
        toReturn = "error allocating memory";
        break;
    case cudaErrorInitializationError:
        toReturn = "initialization error";
        break;
    case cudaErrorCudartUnloading:
        toReturn = "error unloading cudart";
        break;
    case cudaErrorProfilerDisabled:
        toReturn = "error profiler disabled";
        break;
    case cudaErrorProfilerNotInitialized:
        toReturn = "error profiler not initialized";
        break;
    case cudaErrorProfilerAlreadyStarted:
        toReturn = "error profiler already started";
        break;
    case cudaErrorProfilerAlreadyStopped:
        toReturn = "error profiler already stopped";
        break;
    case cudaErrorInvalidConfiguration:
        toReturn = "invalid configuration";
        break;
    case cudaErrorInvalidPitchValue:
        toReturn = "invalid pitch value";
        break;
    case cudaErrorInvalidSymbol:
        toReturn = "invalid symbol";
        break;
    case cudaErrorInvalidHostPointer:
        toReturn = "invalid host pointer";
        break;
    case cudaErrorInvalidDevicePointer:
        toReturn = "invalid device pointer";
        break;
    case cudaErrorInvalidTexture:
        toReturn = "invalid texture";
        break;
    case cudaErrorInvalidTextureBinding:
        toReturn = "invalid texture binding";
        break;
    case cudaErrorInvalidChannelDescriptor:
        toReturn = "invalid channel descriptor";
        break;
    case cudaErrorInvalidMemcpyDirection:
        toReturn = "invalid memcpy direction";
        break;
    case cudaErrorAddressOfConstant:
        toReturn = "address of constant error";
        break;
    case cudaErrorTextureFetchFailed:
        toReturn = "failed texture fetch";
        break;
    case cudaErrorTextureNotBound:
        toReturn = "texture not bound";
        break;
    case cudaErrorSynchronizationError:
        toReturn = "sync error";
        break;
    case cudaErrorInvalidFilterSetting:
        toReturn = "invalid filter setting";
        break;
    case cudaErrorInvalidNormSetting:
        toReturn = "invalid norm settingr";
        break;
    case cudaErrorMixedDeviceExecution:
        toReturn = "mixed device execution error";
        break;
    case cudaErrorNotYetImplemented:
        toReturn = "not yet implemented error";
        break;
    case cudaErrorMemoryValueTooLarge:
        toReturn = "memory value too large";
        break;
    case cudaErrorInsufficientDriver:
        toReturn = "insufficient driver";
        break;
    case cudaErrorInvalidSurface:
        toReturn = "invalid surface";
        break;
    case cudaErrorDuplicateVariableName:
        toReturn = "duplicate variable name";
        break;
    case cudaErrorDuplicateTextureName:
        toReturn = "duplicate texture name";
        break;
    case cudaErrorIncompatibleDriverContext:
        toReturn = "incompatible driver context";
        break;
    case cudaErrorMissingConfiguration:
        toReturn = "missing configuration";
        break;
    case cudaErrorPriorLaunchFailure:
        toReturn = "previous kernel launch failed";
        break;
    case cudaErrorLaunchMaxDepthExceeded:
        toReturn = "max grid depth exceeded";
        break;
    case cudaErrorLaunchFileScopedTex:
        toReturn = "file scoped texture error";
        break;
    case cudaErrorLaunchFileScopedSurf:
        toReturn = "file scoped surface error";
        break;
    case cudaErrorSyncDepthExceeded:
        toReturn = "sync depth exceeded";
        break;
    case cudaErrorLaunchPendingCountExceeded:
        toReturn = "launch pending count exceeded";
        break;
    case cudaErrorInvalidDeviceFunction:
        toReturn = "invalid device function";
        break;
    case cudaErrorNoDevice:
        toReturn = "no cuda capable device detected";
        break;
    case cudaErrorInvalidDevice:
        toReturn = "invalid device selected";
        break;
    case cudaErrorStartupFailure:
        toReturn = "cuda startup error";
        break;
    case cudaErrorInvalidKernelImage:
        toReturn = "invalid kernel image";
        break;
    case cudaErrorDeviceUninitialized:
        toReturn = "invalid device context";
        break;
    case cudaErrorMapBufferObjectFailed:
        toReturn = "buffer object could not be mapped";
        break;
    case cudaErrorUnmapBufferObjectFailed:
        toReturn = "buffer object could not be unmapped";
        break;
    case cudaErrorArrayIsMapped:
        toReturn = "array is mapped and cannot be destroyed";
        break;
    case cudaErrorAlreadyMapped:
        toReturn = "resource is already mapped";
        break;
    case cudaErrorNoKernelImageForDevice:
        toReturn = "no kernel image that is suitable for device";
        break;
    case cudaErrorAlreadyAcquired:
        toReturn = "resource has already been acquired";
        break;
    case cudaErrorNotMapped:
        toReturn = "resource is not mapped";
        break;
    case cudaErrorNotMappedAsArray:
        toReturn = "resource is not mapped as array";
        break;
    case cudaErrorNotMappedAsPointer:
        toReturn = "resource is not mapped as pointer";
        break;
    case cudaErrorECCUncorrectable:
        toReturn = "uncorrectable ECC error was detected during execution";
        break;
    case cudaErrorUnsupportedLimit:
        toReturn = "unsupported limit";
        break;
    case cudaErrorDeviceAlreadyInUse:
        toReturn = "device already in use";
        break;
    case cudaErrorPeerAccessUnsupported:
        toReturn = "unsupported peer access";
        break;
    case cudaErrorInvalidPtx:
        toReturn = "ptx compilation failed";
        break;
    case cudaErrorInvalidGraphicsContext:
        toReturn = "invalid graphics context";
        break;
    case cudaErrorNvlinkUncorrectable:
        toReturn = "uncorrectable NVLink error was detected during the execution";
        break;
    case cudaErrorJitCompilerNotFound:
        toReturn = "PTX JIT compitler library not found";
        break;
    case cudaErrorInvalidSource:
        toReturn = "invalid source";
        break;
    case cudaErrorFileNotFound:
        toReturn = "file not found";
        break;
    case cudaErrorSharedObjectSymbolNotFound:
        toReturn = "shared object symbol not found";
        break;
    case cudaErrorSharedObjectInitFailed:
        toReturn = "shared object initialization failure";
        break;
    case cudaErrorOperatingSystem:
        toReturn = "operating system call failed";
        break;
    case cudaErrorInvalidResourceHandle:
        toReturn = "invalid resource handle";
        break;
    case cudaErrorIllegalState:
        toReturn = "resource in illegal state";
        break;
    case cudaErrorSymbolNotFound:
        toReturn = "symbol not found";
        break;
    case cudaErrorNotReady:
        toReturn = "async operations not completed";
        break;
    case cudaErrorIllegalAddress:
        toReturn = "illegal memory address";
        break;
    case cudaErrorLaunchOutOfResources:
        toReturn = "launch error due to lack of resources";
        break;
    case cudaErrorLaunchTimeout:
        toReturn = "launch timeout";
        break;
    case cudaErrorLaunchIncompatibleTexturing:
        toReturn = "kernel launch failed due to incompatible texturing mode";
        break;
    case cudaErrorPeerAccessAlreadyEnabled:
        toReturn = "peer access already enabled";
        break;
    case cudaErrorPeerAccessNotEnabled:
        toReturn = "peer access not enabled";
        break;
    case cudaErrorSetOnActiveProcess:
        toReturn = "set on active process error";
        break;
    case cudaErrorContextIsDestroyed:
        toReturn = "current context destroyed";
        break;
    case cudaErrorAssert:
        toReturn = "assert error";
        break;
    case cudaErrorTooManyPeers:
        toReturn = "too many peers";
        break;
    case cudaErrorHostMemoryAlreadyRegistered:
        toReturn = "host memory already registered";
        break;
    case cudaErrorHostMemoryNotRegistered:
        toReturn = "host memory not registered";
        break;
    case cudaErrorHardwareStackError:
        toReturn = "hardware stack error";
        break;
    case cudaErrorIllegalInstruction:
        toReturn = "illegal instruction";
        break;
    case cudaErrorMisalignedAddress:
        toReturn = "misaligned address";
        break;
    case cudaErrorInvalidAddressSpace:
        toReturn = "invalid address space";
        break;
    case cudaErrorInvalidPc:
        toReturn = "invalid program counter";
        break;
    case cudaErrorLaunchFailure:
        toReturn = "kernel launch failure";
        break;
    case cudaErrorCooperativeLaunchTooLarge:
        toReturn = "cooperative launch too large";
        break;
    case cudaErrorNotPermitted:
        toReturn = "operation not permitted";
        break;
    case cudaErrorNotSupported:
        toReturn = "operation not supported";
        break;
    case cudaErrorSystemNotReady:
        toReturn = "system not ready";
        break;
    case cudaErrorSystemDriverMismatch:
        toReturn = "system driver mismatch";
        break;
    case cudaErrorCompatNotSupportedOnDevice:
        toReturn = "forward compatibility not supported on device";
        break;
    case cudaErrorStreamCaptureUnsupported:
        toReturn = "unsupported stream capture";
        break;
    case cudaErrorStreamCaptureInvalidated:
        toReturn = "stream capture invalidated due to previous error";
        break;
    case cudaErrorStreamCaptureMerge:
        toReturn = "stream capture merge error";
        break;
    case cudaErrorStreamCaptureUnmatched:
        toReturn = "capture not initiated in this stream";
        break;
    case cudaErrorStreamCaptureUnjoined:
        toReturn = "stream capture unjoined";
        break;
    case cudaErrorStreamCaptureIsolation:
        toReturn = "stream capture isolation";
        break;
    case cudaErrorStreamCaptureImplicit:
        toReturn = "stream capture implicit";
        break;
    case cudaErrorCapturedEvent:
        toReturn = "captured event";
        break;
    case cudaErrorStreamCaptureWrongThread:
        toReturn = "stream capture wrong thread";
        break;
    case cudaErrorTimeout:
        toReturn = "timeout";
        break;
    case cudaErrorGraphExecUpdateFailure:
        toReturn = "graph execution update failure";
        break;
    case cudaErrorUnknown:
        toReturn = "unknown cuda error";
        break;
    case cudaErrorApiFailureBase:
        toReturn = "api failure base";
        break;
    default:
        toReturn = "unknown error";
        break;
    }

    return toReturn;
}


static bool CudaCheckError(cudaError error, const char* function, const char* file, int line)
{
	if (error != cudaSuccess)
	{
		ERRORLOC_LOG("cuda error - " + GetErrorString(error) + ": " + std::to_string(error), function, file, line);
		return false;
	}
	return true;
}
