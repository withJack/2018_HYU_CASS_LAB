/******************************************************************* 
 * Second project in CASS Lab, Hanyang Univ.
 * Implementation of blurring and edge detecting in 2Dconvolution.
 * Used OpenCL to accelerate matrix operations.
 * 
 * Edge Detecting operator: Sobel Operator 
 *
 *******************************************************************/

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

/* Can switch DATA_TYPE between float and double */
typedef float DATA_TYPE;

__kernel void SobelOperator_kernel(__global DATA_TYPE *A, __global DATA_TYPE *B, int ni, int nj) 
{
	int j = get_global_id(0);
	int i = get_global_id(1);

	DATA_TYPE cx11, cx12, cx13, cx21, cx22, cx23, cx31, cx32, cx33;
	DATA_TYPE cy11, cy12, cy13, cy21, cy22, cy23, cy31, cy32, cy33;
	cx11 = -1;  cx21 = +0;  cx31 = +1;
	cx12 = -2;  cx22 = +0;  cx32 = +2;
	cx13 = -1;  cx23 = +0;  cx33 = +1;

	cy11 = +1;  cy21 = +2;  cy31 = +1;
	cy12 = +0;  cy22 = +0;  cy32 = +0;
	cy13 = -1;  cy23 = -2;  cy33 = -1;

	if ((i < (ni-1)) && (j < (nj - 1)) && (i > 0) && (j > 0))
	{
		DATA_TYPE gx, gy;

		gx = cx11 * A[(i - 1) * nj + (j - 1)]  + cx21 * A[(i - 1) * nj + (j + 0)] + cx31 * A[(i - 1) * nj + (j + 1)] 
			+ cx12 * A[(i + 0) * nj + (j - 1)]  + cx22 * A[(i + 0) * nj + (j + 0)] + cx32 * A[(i + 0) * nj + (j + 1)]
			+ cx13 * A[(i + 1) * nj + (j - 1)]  + cx23 * A[(i + 1) * nj + (j + 0)] + cx33 * A[(i + 1) * nj + (j + 1)];

		gy = cy11 * A[(i - 1) * nj + (j - 1)]  + cy21 * A[(i - 1) * nj + (j + 0)] + cy31 * A[(i - 1) * nj + (j + 1)] 
			+ cy12 * A[(i + 0) * nj + (j - 1)]  + cy22 * A[(i + 0) * nj + (j + 0)] + cy32 * A[(i + 0) * nj + (j + 1)]
			+ cy13 * A[(i + 1) * nj + (j - 1)]  + cy23 * A[(i + 1) * nj + (j + 0)] + cy33 * A[(i + 1) * nj + (j + 1)];

		B[i*nj + j] = sqrt((gx*gx) + (gy*gy));
	}
}
