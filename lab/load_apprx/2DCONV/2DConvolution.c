/******************************************************************* 
 * Second project in CASS Lab, Hanyang Univ.
 * Implementation of blurring and edge detecting in 2Dconvolution.
 * Used OpenCL to accelerate matrix operations. 
 *
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "polybenchUtilFuncts.h"

//define the error threshold for the results "not matching"
#define PERCENT_DIFF_ERROR_THRESHOLD 1.05

#define MAX_SOURCE_SIZE (0x100000)

/* Problem size */
#define NI 4096
#define NJ 4096

/* Thread block dimensions */
#define DIM_LOCAL_WORK_GROUP_X 32
#define DIM_LOCAL_WORK_GROUP_Y 8

#if defined(cl_khr_fp64)  // Khronos extension available?
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)  // AMD extension available?
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#endif

/* Can switch DATA_TYPE between float and double */
typedef float DATA_TYPE;


char str_temp[1024];

cl_platform_id platform_id;
cl_device_id device_id;   
cl_uint num_devices;
cl_uint num_platforms;
cl_int errcode;
cl_context clGPUContext;
cl_kernel clKernel1;
cl_kernel clKernel2;
cl_kernel clKernel3;
cl_command_queue clCommandQue;
cl_program clProgram1;
cl_program clProgram2;
cl_mem a_mem_obj;
cl_mem b_mem_obj;
cl_mem c_mem_obj;
cl_mem d_mem_obj;
cl_mem e_mem_obj;
cl_mem f_mem_obj;
cl_mem g_mem_obj;
FILE *fp1, *fp2;
char *source_str1, *source_str2;
size_t source_size1, source_size2;
int histogram[256];


void compareResults(DATA_TYPE* B, DATA_TYPE* B_outputFromGpu)
{
	int i, j, fail;
	fail = 0;
	
	// Compare a and b
	for (i=1; i < (NI-1); i++) 
	{
		for (j=1; j < (NJ-1); j++) 
		{
			if (percentDiff(B[i*NJ + j], B_outputFromGpu[i*NJ + j]) > PERCENT_DIFF_ERROR_THRESHOLD) 
			{
				fail++;
			}
		}
	}
	
	// Print results
	printf("Non-Matching CPU-GPU Outputs Beyond Error Threshold of %4.2f Percent: %d\n", PERCENT_DIFF_ERROR_THRESHOLD, fail);
	
}


void read_cl_file()
{
	// Load the kernel source code into the array source_str
	fp1 = fopen("Filters.cl", "r");
	fp2 = fopen("EdgeDetection.cl", "r");
	if (!fp1 || !fp2) {
		fprintf(stdout, "Failed to load kernel.\n");
		exit(1);
	}
	source_str1 = (char*)malloc(MAX_SOURCE_SIZE);
	source_str2 = (char*)malloc(MAX_SOURCE_SIZE);
	source_size1 = fread( source_str1, 1, MAX_SOURCE_SIZE, fp1);
	source_size2 = fread( source_str2, 1, MAX_SOURCE_SIZE, fp2);
	fclose( fp1 );
	fclose( fp2 );
}


void init(DATA_TYPE* A)
{
	int i, j;

	for (i = 0; i < NI; ++i)
    	{
		for (j = 0; j < NJ; ++j)
		{
			A[i*NJ + j] = (float)rand()/RAND_MAX;
        	}
    	}
}


void cl_initialization()
{
	
	// Get platform and device information
	errcode = clGetPlatformIDs(1, &platform_id, &num_platforms);
	if(errcode == CL_SUCCESS) printf("number of platforms is %d\n",num_platforms);
	else printf("Error getting platform IDs\n");

	errcode = clGetPlatformInfo(platform_id,CL_PLATFORM_NAME, sizeof(str_temp), str_temp,NULL);
	if(errcode == CL_SUCCESS) printf("platform name is %s\n",str_temp);
	else printf("Error getting platform name\n");

	errcode = clGetPlatformInfo(platform_id, CL_PLATFORM_VERSION, sizeof(str_temp), str_temp,NULL);
	if(errcode == CL_SUCCESS) printf("platform version is %s\n",str_temp);
	else printf("Error getting platform version\n");

	errcode = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_devices);
	if(errcode == CL_SUCCESS) printf("number of devices is %d\n", num_devices);
	else printf("Error getting device IDs\n");

	errcode = clGetDeviceInfo(device_id,CL_DEVICE_NAME, sizeof(str_temp), str_temp,NULL);
	if(errcode == CL_SUCCESS) printf("device name is %s\n",str_temp);
	else printf("Error getting device name\n");
	
	// Create an OpenCL context
	clGPUContext = clCreateContext( NULL, 1, &device_id, NULL, NULL, &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating context\n");
 
	//Create a command-queue
	clCommandQue = clCreateCommandQueue(clGPUContext, device_id, 0, &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating command queue\n");
}


void cl_mem_init(DATA_TYPE *A, DATA_TYPE *C, DATA_TYPE *E)
{
	a_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_ONLY, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	b_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	c_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_ONLY, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	d_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	e_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_ONLY, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	f_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	g_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	
	if(errcode != CL_SUCCESS) printf("Error in creating buffers\n");

	errcode = clEnqueueWriteBuffer(clCommandQue, a_mem_obj, CL_TRUE, 0, sizeof(DATA_TYPE) * NI * NJ, A, 0, NULL, NULL);
	errcode = clEnqueueWriteBuffer(clCommandQue, c_mem_obj, CL_TRUE, 0, sizeof(DATA_TYPE) * NI * NJ, C, 0, NULL, NULL);
	errcode = clEnqueueWriteBuffer(clCommandQue, e_mem_obj, CL_TRUE, 0, sizeof(DATA_TYPE) * NI * NJ, E, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in writing buffers\n");
}

void cl_mem_init2(DATA_TYPE *A)
{
	a_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	b_mem_obj = clCreateBuffer(clGPUContext, CL_MEM_READ_WRITE, sizeof(DATA_TYPE) * NI * NJ, NULL, &errcode);
	
	if(errcode != CL_SUCCESS) printf("Error in creating buffers\n");

	errcode = clEnqueueWriteBuffer(clCommandQue, a_mem_obj, CL_TRUE, 0, sizeof(DATA_TYPE) * NI * NJ, A, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in writing buffers\n");
}


void cl_load_prog()
{
	// Create a program from the kernel source
	clProgram1 = clCreateProgramWithSource(clGPUContext, 1, (const char **)&source_str1, (const size_t *)&source_size1, &errcode);
	clProgram2 = clCreateProgramWithSource(clGPUContext, 1, (const char **)&source_str2, (const size_t *)&source_size2, &errcode);

	if(errcode != CL_SUCCESS) printf("Error in creating program\n");

	// Build the program
	errcode = clBuildProgram(clProgram1, 1, &device_id, NULL, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in building program\n");

	errcode = clBuildProgram(clProgram2, 1, &device_id, NULL, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in building program\n");
		
	// Create the OpenCL kernel
	clKernel1 = clCreateKernel(clProgram1, "MeanFilter_kernel", &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating kernel\n");
	clKernel2 = clCreateKernel(clProgram1, "GaussianFilter_kernel", &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating kernel\n");
	clKernel3 = clCreateKernel(clProgram2, "SobelOperator_kernel", &errcode);
	if(errcode != CL_SUCCESS) printf("Error in creating kernel\n");
	clFinish(clCommandQue);
}


void cl_launch_kernel()
{
	double t_start, t_end;
	int ni = NI;
	int nj = NJ;

	size_t localWorkSize[2], globalWorkSize[2];
	localWorkSize[0] = DIM_LOCAL_WORK_GROUP_X;
	localWorkSize[1] = DIM_LOCAL_WORK_GROUP_Y;
	globalWorkSize[0] = (size_t)ceil(((float)NI) / ((float)DIM_LOCAL_WORK_GROUP_X)) * DIM_LOCAL_WORK_GROUP_X;
	globalWorkSize[1] = (size_t)ceil(((float)NJ) / ((float)DIM_LOCAL_WORK_GROUP_Y)) * DIM_LOCAL_WORK_GROUP_Y;

	t_start = rtclock();
	
	// Set the arguments of the kernel
	errcode =  clSetKernelArg(clKernel1, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	errcode |= clSetKernelArg(clKernel1, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	errcode |= clSetKernelArg(clKernel1, 2, sizeof(int), &ni);
	errcode |= clSetKernelArg(clKernel1, 3, sizeof(int), &nj);

	if(errcode != CL_SUCCESS) printf("Error in seting arguments\n");
	// Execute the OpenCL kernel
	errcode = clEnqueueNDRangeKernel(clCommandQue, clKernel1, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	// Set the arguments of the kernel
	errcode =  clSetKernelArg(clKernel2, 0, sizeof(cl_mem), (void *)&c_mem_obj);
	errcode |= clSetKernelArg(clKernel2, 1, sizeof(cl_mem), (void *)&d_mem_obj);
	errcode |= clSetKernelArg(clKernel2, 2, sizeof(int), &ni);
	errcode |= clSetKernelArg(clKernel2, 3, sizeof(int), &nj);

	if(errcode != CL_SUCCESS) printf("Error in seting arguments\n");
	// Execute the OpenCL kernel
	errcode = clEnqueueNDRangeKernel(clCommandQue, clKernel2, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	
	errcode =  clSetKernelArg(clKernel3, 0, sizeof(cl_mem), (void *)&e_mem_obj);
	errcode |= clSetKernelArg(clKernel3, 1, sizeof(cl_mem), (void *)&f_mem_obj);
	errcode |= clSetKernelArg(clKernel3, 2, sizeof(int), &ni);
	errcode |= clSetKernelArg(clKernel3, 3, sizeof(int), &nj);

	if(errcode != CL_SUCCESS) printf("Error in seting arguments\n");
	// Execute the OpenCL kernel
	errcode = clEnqueueNDRangeKernel(clCommandQue, clKernel3, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	if(errcode != CL_SUCCESS) printf("Error in launching kernel\n");
	clFinish(clCommandQue);

	t_end = rtclock();
	fprintf(stdout, "GPU Runtime: %0.6lfs\n", t_end - t_start);
}

void cl_additional_launch()
{
	double t_start, t_end;
	int ni = NI;
	int nj = NJ;

	size_t localWorkSize[2], globalWorkSize[2];
	localWorkSize[0] = DIM_LOCAL_WORK_GROUP_X;
	localWorkSize[1] = DIM_LOCAL_WORK_GROUP_Y;
	globalWorkSize[0] = (size_t)ceil(((float)NI) / ((float)DIM_LOCAL_WORK_GROUP_X)) * DIM_LOCAL_WORK_GROUP_X;
	globalWorkSize[1] = (size_t)ceil(((float)NJ) / ((float)DIM_LOCAL_WORK_GROUP_Y)) * DIM_LOCAL_WORK_GROUP_Y;
	
	t_start = rtclock();
	// Set the arguments of the kernel
	errcode =  clSetKernelArg(clKernel3, 0, sizeof(cl_mem), (void *)&b_mem_obj);
	errcode |= clSetKernelArg(clKernel3, 1, sizeof(cl_mem), (void *)&g_mem_obj);
	errcode |= clSetKernelArg(clKernel3, 2, sizeof(int), &ni);
	errcode |= clSetKernelArg(clKernel3, 3, sizeof(int), &nj);

	if(errcode != CL_SUCCESS) printf("Error in seting arguments\n");
	// Execute the OpenCL kernel
	errcode = clEnqueueNDRangeKernel(clCommandQue, clKernel3, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	if(errcode != CL_SUCCESS) printf("Error in launching kernel\n");
	clFinish(clCommandQue);
	
	t_end = rtclock();
	fprintf(stdout, "Additional Runtime: %0.6lfs\n", t_end - t_start);
}

void cl_selective_launch(char* which)
{
	double t_start, t_end;
	int ni = NI;
	int nj = NJ;
	cl_kernel tmpKernel;

	size_t localWorkSize[2], globalWorkSize[2];
	localWorkSize[0] = DIM_LOCAL_WORK_GROUP_X;
	localWorkSize[1] = DIM_LOCAL_WORK_GROUP_Y;
	globalWorkSize[0] = (size_t)ceil(((float)NI) / ((float)DIM_LOCAL_WORK_GROUP_X)) * DIM_LOCAL_WORK_GROUP_X;
	globalWorkSize[1] = (size_t)ceil(((float)NJ) / ((float)DIM_LOCAL_WORK_GROUP_Y)) * DIM_LOCAL_WORK_GROUP_Y;
	
	if (!strcmp(which, "Mean") || !strcmp(which, "mean"))
		tmpKernel = clKernel1;
	else if (!strcmp(which, "Gaussian") || !strcmp(which, "gaussian"))
		tmpKernel = clKernel2;
	else if (!strcmp(which, "Sobel") || !strcmp(which, "sobel"))
		tmpKernel = clKernel3;
	else
	{
		printf("error\n");
		exit(-1);
	}

	t_start = rtclock();
	// Set the arguments of the kernel
	errcode =  clSetKernelArg(tmpKernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
	errcode |= clSetKernelArg(tmpKernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
	errcode |= clSetKernelArg(tmpKernel, 2, sizeof(int), &ni);
	errcode |= clSetKernelArg(tmpKernel, 3, sizeof(int), &nj);

	if(errcode != CL_SUCCESS) printf("Error in seting arguments\n");
	// Execute the OpenCL kernel
	errcode = clEnqueueNDRangeKernel(clCommandQue, tmpKernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	if(errcode != CL_SUCCESS) printf("Error in launching kernel\n");
	clFinish(clCommandQue);
	
	t_end = rtclock();
	fprintf(stdout, "Selective (%s) Runtime: %0.6lfs\n", which, t_end - t_start);
}

void cl_clean_up()
{
	// Clean up
	errcode = clFlush(clCommandQue);
	errcode = clFinish(clCommandQue);
	errcode = clReleaseKernel(clKernel1);
	errcode = clReleaseKernel(clKernel2);
	errcode = clReleaseKernel(clKernel3);
	errcode = clReleaseProgram(clProgram1);
	errcode = clReleaseProgram(clProgram2);
	errcode = clReleaseMemObject(a_mem_obj);
	errcode = clReleaseMemObject(b_mem_obj);
	errcode = clReleaseMemObject(c_mem_obj);
	errcode = clReleaseMemObject(d_mem_obj);
	errcode = clReleaseMemObject(e_mem_obj);
	errcode = clReleaseMemObject(f_mem_obj);
	errcode = clReleaseMemObject(g_mem_obj);
	errcode = clReleaseCommandQueue(clCommandQue);
	errcode = clReleaseContext(clGPUContext);
	if(errcode != CL_SUCCESS) printf("Error in cleanup\n");
}

void cl_clean_up2()
{
	errcode = clFlush(clCommandQue);
	errcode = clFinish(clCommandQue);
	errcode = clReleaseKernel(clKernel1);
	errcode = clReleaseKernel(clKernel2);
	errcode = clReleaseProgram(clProgram1);
	errcode = clReleaseProgram(clProgram2);
	errcode = clReleaseMemObject(a_mem_obj);
	errcode = clReleaseMemObject(b_mem_obj);
	errcode = clReleaseCommandQueue(clCommandQue);
	errcode = clReleaseContext(clGPUContext);
	if(errcode != CL_SUCCESS) printf("Error in cleanup\n");
}

void MeanFilter(DATA_TYPE* A, DATA_TYPE* B)
{
	int i, j;
	DATA_TYPE c11, c12, c13, c21, c22, c23, c31, c32, c33;

	c11 = +1;  c21 = +1;  c31 = +1;
	c12 = +1;  c22 = +2;  c32 = +1;
	c13 = +1;  c23 = +1;  c33 = +1;
	DATA_TYPE coefficient = c11 + c12 + c13 + c21 + c22 + c23 + c31 + c32 + c33;

	for (i = 1; i < NI - 1; ++i) // 0
	{
		for (j = 1; j < NJ - 1; ++j) // 1
		{
			B[i*NJ + j] = (c11 * A[(i - 1)*NJ + (j - 1)]  +  c12 * A[(i + 0)*NJ + (j - 1)]  +  c13 * A[(i + 1)*NJ + (j - 1)]
				+ c21 * A[(i - 1)*NJ + (j + 0)]  +  c22 * A[(i + 0)*NJ + (j + 0)]  +  c23 * A[(i + 1)*NJ + (j + 0)] 
				+ c31 * A[(i - 1)*NJ + (j + 1)]  +  c32 * A[(i + 0)*NJ + (j + 1)]  +  c33 * A[(i + 1)*NJ + (j + 1)]) / coefficient;
		}
	}
}

void GaussianFilter(DATA_TYPE *A, DATA_TYPE *B)
{
	int i, j;
	DATA_TYPE c11, c12, c13, c21, c22, c23, c31, c32, c33;

	c11 = +1;  c21 = +2;  c31 = +1;
	c12 = +2;  c22 = +4;  c32 = +2;
	c13 = +1;  c23 = +2;  c33 = +1;
	DATA_TYPE coefficient = c11 + c12 + c13 + c21 + c22 + c23 + c31 + c32 + c33;

	for (i = 1; i < NI - 1; ++i) // 0
	{
		for (j = 1; j < NJ - 1; ++j) // 1
		{
			B[i*NJ + j] = (c11 * A[(i - 1)*NJ + (j - 1)]  +  c12 * A[(i + 0)*NJ + (j - 1)]  +  c13 * A[(i + 1)*NJ + (j - 1)]
				+ c21 * A[(i - 1)*NJ + (j + 0)]  +  c22 * A[(i + 0)*NJ + (j + 0)]  +  c23 * A[(i + 1)*NJ + (j + 0)] 
				+ c31 * A[(i - 1)*NJ + (j + 1)]  +  c32 * A[(i + 0)*NJ + (j + 1)]  +  c33 * A[(i + 1)*NJ + (j + 1)]) / coefficient;
		}
	}
}

void SobelOperator(DATA_TYPE *A, DATA_TYPE *B)
{
	int i, j;

	DATA_TYPE cx11, cx12, cx13, cx21, cx22, cx23, cx31, cx32, cx33;
	DATA_TYPE cy11, cy12, cy13, cy21, cy22, cy23, cy31, cy32, cy33;
	cx11 = -1;  cx21 = +0;  cx31 = +1;
	cx12 = -2;  cx22 = +0;  cx32 = +2;
	cx13 = -1;  cx23 = +0;  cx33 = +1;

	cy11 = +1;  cy21 = +2;  cy31 = +1;
	cy12 = +0;  cy22 = +0;  cy32 = +0;
	cy13 = -1;  cy23 = -2;  cy33 = -1;

	for (i = 1; i < NI - 1; ++i) // 0
	{
		for (j = 1; j < NJ - 1; ++j) // 1
		{
			DATA_TYPE gx, gy;
			
			gx = cx11 * A[(i - 1)*NJ + (j - 1)]  +  cx12 * A[(i + 0)*NJ + (j - 1)]  +  cx13 * A[(i + 1)*NJ + (j - 1)]
				+ cx21 * A[(i - 1)*NJ + (j + 0)]  +  cx22 * A[(i + 0)*NJ + (j + 0)]  +  cx23 * A[(i + 1)*NJ + (j + 0)] 
				+ cx31 * A[(i - 1)*NJ + (j + 1)]  +  cx32 * A[(i + 0)*NJ + (j + 1)]  +  cx33 * A[(i + 1)*NJ + (j + 1)];

			gy = cy11 * A[(i - 1)*NJ + (j - 1)]  +  cy12 * A[(i + 0)*NJ + (j - 1)]  +  cy13 * A[(i + 1)*NJ + (j - 1)]
				+ cy21 * A[(i - 1)*NJ + (j + 0)]  +  cy22 * A[(i + 0)*NJ + (j + 0)]  +  cy23 * A[(i + 1)*NJ + (j + 0)] 
				+ cy31 * A[(i - 1)*NJ + (j + 1)]  +  cy32 * A[(i + 0)*NJ + (j + 1)]  +  cy33 * A[(i + 1)*NJ + (j + 1)];

			B[i*NJ + j] = sqrt((gx*gx) + (gy*gy));
			
		}
	}
}
/*
void Histogram()
{
	for(int i = 0; i < 256; i++)
	{
		for()
	}
}
*/
int main(int argc, char *argv[])
{
	double t_start, t_end;
	int i;
if( argc == 1 )
{
	DATA_TYPE* A;
	DATA_TYPE* B;  
	DATA_TYPE* B_outputFromGpu;
	DATA_TYPE* C;
	DATA_TYPE* D;
	DATA_TYPE* D_outputFromGpu;
	DATA_TYPE* E;
	DATA_TYPE* F;
	DATA_TYPE* F_outputFromGpu;
	DATA_TYPE* G_formore;
	DATA_TYPE* G_outputFromGpu;

	// Mean filter
	A = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	B = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	B_outputFromGpu = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));

	// Gaussian filter
	C = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	D = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	D_outputFromGpu = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	
	// Edge Operator
	E = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	F = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	F_outputFromGpu = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));

	G_formore = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	G_outputFromGpu = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));

	init(A);
	init(C);
	init(E);

	read_cl_file();
	cl_initialization();
	cl_mem_init(A, C, E);
	cl_load_prog();

	cl_launch_kernel();

	errcode = clEnqueueReadBuffer(clCommandQue, b_mem_obj, CL_TRUE, 0, NI*NJ*sizeof(DATA_TYPE), B_outputFromGpu, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in reading GPU mem\n");

	errcode = clEnqueueReadBuffer(clCommandQue, d_mem_obj, CL_TRUE, 0, NI*NJ*sizeof(DATA_TYPE), D_outputFromGpu, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in reading GPU mem\n");

	errcode = clEnqueueReadBuffer(clCommandQue, f_mem_obj, CL_TRUE, 0, NI*NJ*sizeof(DATA_TYPE), F_outputFromGpu, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in reading GPU mem\n");
	
	cl_additional_launch();

	errcode = clEnqueueReadBuffer(clCommandQue, g_mem_obj, CL_TRUE, 0, NI*NJ*sizeof(DATA_TYPE), G_outputFromGpu, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in reading GPU mem\n");

	t_start = rtclock();
	MeanFilter(A, B);
	GaussianFilter(C, D);
	SobelOperator(E, F);
	SobelOperator(B, G_formore);
	t_end = rtclock(); 
	fprintf(stdout, "CPU Runtime: %0.6lfs\n", t_end - t_start);   
	compareResults(B, B_outputFromGpu);
	compareResults(D, D_outputFromGpu);
	compareResults(F, F_outputFromGpu);
	compareResults(G_formore, G_outputFromGpu);
	free(A);
	free(B);
	free(B_outputFromGpu);
	free(C);
	free(D);
	free(D_outputFromGpu);
	free(E);
	free(F);
	free(F_outputFromGpu);

	cl_clean_up();
    
	return 0;
}

else if( argc == 2 )
{
	DATA_TYPE *A;
	DATA_TYPE *B;
	DATA_TYPE *B_outputFromGpu;

	A = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	B = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));
	B_outputFromGpu = (DATA_TYPE*)malloc(NI*NJ*sizeof(DATA_TYPE));

	init(A);
	read_cl_file();
	cl_initialization();
	cl_mem_init2(A);
	cl_load_prog();
	cl_selective_launch(argv[1]);
	
	errcode = clEnqueueReadBuffer(clCommandQue, b_mem_obj, CL_TRUE, 0, NI*NJ*sizeof(DATA_TYPE), B_outputFromGpu, 0, NULL, NULL);
	if(errcode != CL_SUCCESS) printf("Error in reading GPU mem\n");

	t_start = rtclock();
	if (!strcmp(argv[1], "Mean") || !strcmp(argv[1], "mean"))
		MeanFilter(A, B);
	else if (!strcmp(argv[1], "Gaussian") || !strcmp(argv[1], "gaussian"))
		GaussianFilter(A, B);
	else if (!strcmp(argv[1], "Sobel") || !strcmp(argv[1], "sobel"))
		SobelOperator(A, B);
	else
	{
		fprintf(stdout, "wrong parameter\n");
		return 0;
	}
	t_end = rtclock();
	fprintf(stdout, "CPU (%s) Runtime: %0.6lfs\n", argv[1], t_end - t_start);
	compareResults(B, B_outputFromGpu);
	free(A);
	free(B);
	free(B_outputFromGpu);

	cl_clean_up2();
	
	return 0;
}
else
{
	printf("too much parameters\n");
	return 0;
}

}

