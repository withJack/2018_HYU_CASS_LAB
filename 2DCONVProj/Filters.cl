/******************************************************************* 
 * Second project in CASS Lab, Hanyang Univ.
 * Implementation of smoothing(blurring), edge detecting in 2Dconvolution.
 * Used OpenCL to accelerate matrix operations. 
 *
 * Smoothing Filter types: Mean, Gaussian filter
 *
 *******************************************************************/

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

/* Can switch DATA_TYPE between float and double */
typedef float DATA_TYPE;

__kernel void MeanFilter_kernel(__global DATA_TYPE *A, __global DATA_TYPE *B, int ni, int nj) 
{
	int j = get_global_id(0);
	int i = get_global_id(1);

	DATA_TYPE c11, c12, c13, c21, c22, c23, c31, c32, c33;
	
	c11 = +1;  c21 = +1;  c31 = +1;
	c12 = +1;  c22 = +2;  c32 = +1;
	c13 = +1;  c23 = +1;  c33 = +1;
	DATA_TYPE coefficient = c11 + c12 + c13 + c21 + c22 + c23 + c31 + c32 + c33;
	
	if ((i < (ni - 1)) && (j < (nj - 1)) && (i > 0) && (j > 0))
	{
		B[i*nj + j] = (c11 * A[(i - 1) * nj + (j - 1)]  + c21 * A[(i - 1) * nj + (j + 0)] + c31 * A[(i - 1) * nj + (j + 1)] 
				+ c12 * A[(i + 0) * nj + (j - 1)]  + c22 * A[(i + 0) * nj + (j + 0)] + c32 * A[(i + 0) * nj + (j + 1)]
				+ c13 * A[(i + 1) * nj + (j - 1)]  + c23 * A[(i + 1) * nj + (j + 0)] + c33 * A[(i + 1) * nj + (j + 1)]) / coefficient;
	}
}

__kernel void GaussianFilter_kernel(__global DATA_TYPE *A, __global DATA_TYPE *B, int ni, int nj)
{
	int j = get_global_id(0);
	int i = get_global_id(1);

	DATA_TYPE c11, c12, c13, c21, c22, c23, c31, c32, c33;

	c11 = +1;  c21 = +2;  c31 = +1;
	c12 = +2;  c22 = +4;  c32 = +2;
	c13 = +1;  c23 = +2;  c33 = +1;
	DATA_TYPE coefficient = c11 + c12 + c13 + c21 + c22 + c23 + c31 + c32 + c33;
	
	if ((i < (ni-1)) && (j < (nj - 1)) && (i > 0) && (j > 0))
	{
		B[i*nj + j] = (c11 * A[(i - 1) * nj + (j - 1)]  + c21 * A[(i - 1) * nj + (j + 0)] + c31 * A[(i - 1) * nj + (j + 1)] 
				+ c12 * A[(i + 0) * nj + (j - 1)]  + c22 * A[(i + 0) * nj + (j + 0)] + c32 * A[(i + 0) * nj + (j + 1)]
				+ c13 * A[(i + 1) * nj + (j - 1)]  + c23 * A[(i + 1) * nj + (j + 0)] + c33 * A[(i + 1) * nj + (j + 1)]) / coefficient;
	}
}

