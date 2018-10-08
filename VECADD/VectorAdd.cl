/**
 * This is accelerated Vector addition using OpenCL.
 * Refer to below codes just for OpenCL compilation.
 *
 * 2DConvolution.cl: This file is part of the PolyBench/GPU 1.0 test suite.
 *
 *
 * Contact: Scott Grauer-Gray <sgrauerg@gmail.com>
 * Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://www.cse.ohio-state.edu/~pouchet/software/polybench/GPU
 */

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

/* Can switch DATA_TYPE between float and double */
typedef float DATA_TYPE;

__kernel void VectorAdd_kernel(__global DATA_TYPE *A, __global DATA_TYPE *B, __global DATA_TYPE *C) 
{
	int i = get_global_id(0);
	C[i] = (DATA_TYPE)(A[i] + B[i]);
}
