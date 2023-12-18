#include <sys/time.h>
#include <cuda_runtime.h>
#include <iostream>

#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
   if (code != cudaSuccess) 
   {
      fprintf(stdout,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
      if (abort) exit(code);
   }
}

float time_kernel_execution(const void *kernel, int grid_size, int block_size, void **args, size_t shared_memory, cudaStream_t stream) {
    float time;
    cudaEvent_t start, stop;

    gpuErrchk( cudaEventCreate(&start) );
    gpuErrchk( cudaEventCreate(&stop) );

    gpuErrchk( cudaEventRecord(start, stream) );
    gpuErrchk( cudaLaunchKernel(kernel, grid_size, block_size, args, shared_memory, stream) );
    gpuErrchk( cudaEventRecord(stop, stream) );

    gpuErrchk( cudaEventSynchronize(stop) );
    gpuErrchk( cudaEventElapsedTime(&time, start, stop) );

    return time;
}

template <typename FUNCTYPE, typename... ARGTYPES>
float time_function_execution(FUNCTYPE f, ARGTYPES... args) {
    struct timeval start, end;

    gettimeofday(&start, nullptr);
    f(args...);
    gettimeofday(&end, nullptr);

    return (float) (end.tv_usec - start.tv_usec) / 1000.f + (float) (end.tv_sec - start.tv_sec) * 1000.f;
}