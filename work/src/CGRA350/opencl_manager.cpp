#include "opencl_manager.h"

#include <stdio.h>
#include <stdlib.h>

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

namespace CGRA350 {

OpenclManager* OpenclManager::_instance = nullptr;

OpenclManager* OpenclManager::getInstance()
{
	if (_instance == nullptr) {
		_instance = CGRA_NEW(OPENCL_INIT) OpenclManager();
	}

	return _instance;
}

OpenclManager::OpenclManager()
	: platform()
	, devices(nullptr)
	, context()
	, commandQueue()
{
	CGRA_ACTIVITY_START(OPENCL_INIT);

	/**Step 1: Getting platforms and choose an available one(first).*/
	cl_uint numPlatforms = 0;
	if (clGetPlatformIDs(0, NULL, &numPlatforms) != CL_SUCCESS)
	{
		CGRA_LOGE();
	}

	/**For clarity, choose the first available platform. */
	if (numPlatforms > 0)
	{
		cl_platform_id* platforms = (cl_platform_id* )CGRA_MALLOC(numPlatforms * sizeof(cl_platform_id), OPENCL_INIT);
		clGetPlatformIDs(numPlatforms, platforms, NULL);
		platform = platforms[0];

		CGRA_FREE(platforms);
	}

	/**Step 2:Query the platform and choose the first GPU device if has one.*/
	cl_uint numDevices = 0;
	clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	if (numDevices > 0) //GPU available.
	{
		devices = (cl_device_id*)CGRA_MALLOC(numDevices * sizeof(cl_device_id), OPENCL_INIT);
		clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
	}

	/**Step 3: Create context.*/
	context = clCreateContext(NULL, 1, devices, NULL, NULL, NULL);

	/**Step 4: Creating command queue associate with the context.*/
	commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);

	CGRA_ACTIVITY_END(OPENCL_INIT);
}


OpenclManager::~OpenclManager()
{
	/**Step 12: Clean the resources.*/
	clReleaseCommandQueue(commandQueue);
	clReleaseContext(context);

	if (devices != NULL)
	{
		CGRA_FREE(devices);
		devices = NULL;
	}
}

} // namespace CGRA350