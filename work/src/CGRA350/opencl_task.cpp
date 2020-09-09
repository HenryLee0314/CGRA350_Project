#include "opencl_task.h"

#include "opencl_manager.h"
#include "cgraFile.h"
#include <string>
#include <iostream>

namespace CGRA350 {

OpenclTask::OpenclTask(const char* const fileAddress)
	: program()
	, kernel()
{
	/**Step 5: Create program object */
	std::string fileContent = "";

	cgraFile file(fileAddress);
	file.read(fileContent);

    const char *source = fileContent.c_str();
    size_t sourceSize[] = {(size_t)fileContent.size()};
	program = clCreateProgramWithSource(OpenclManager::getInstance()->getContent(), 1, &source, sourceSize, NULL);

    /**Step 6: Build program. */
	clBuildProgram(program, 1, OpenclManager::getInstance()->getDevices(), NULL, NULL, NULL);

    /**Step 7: Create kernel object */
	kernel = clCreateKernel(program, "helloworld", NULL);
}

void OpenclTask::run() {
    /**Step 8: Initial input,output for the host and create memory objects for the kernel*/
	const int NUM = 16;
	float* input = new float[NUM];
	float* output = new float[NUM];
	for (int i = 0; i < NUM; i++) {
		input[i] = i;
		output[i] = 0;
	}


	cl_mem inputBuffer = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, (NUM) * sizeof(float), (void*)input, NULL);
	cl_mem outputBuffer = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_WRITE_ONLY , NUM * sizeof(float), NULL, NULL);

    /**Step 9: Sets Kernel arguments.*/
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&inputBuffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&outputBuffer);

    /**Step 10: Running the kernel.*/
	size_t global_work_size[1] = {NUM};
	cl_event enentPoint;
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), kernel, 1, NULL, global_work_size, NULL, 0, NULL, &enentPoint);
	clWaitForEvents(1, &enentPoint); ///wait
	clReleaseEvent(enentPoint);

    // *Step 11: Read the cout put back to host memory.
	clEnqueueReadBuffer(OpenclManager::getInstance()->getCommandQueue(), outputBuffer, CL_TRUE, 0, NUM * sizeof(float), output, 0, NULL, NULL);

	for (int i = 0; i < NUM; i++) {
		std::cout << "i:" << i << "-----" << output[i] << std::endl;
	}

	clReleaseMemObject(inputBuffer);
	clReleaseMemObject(outputBuffer);
}

OpenclTask::~OpenclTask()
{
	clReleaseKernel(kernel);
	clReleaseProgram(program);
}

} // namespace CGRA350

