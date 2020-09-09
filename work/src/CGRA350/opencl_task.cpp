#include "opencl_task.h"

#include "opencl_manager.h"
#include "cgraFile.h"
#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include <string>
#include <iostream>

namespace CGRA350 {

OpenclTask::OpenclTask(const char* const fileAddress)
	: program()
{
	CGRA_ACTIVITY_START(OPENCL_TASK);
	/**Step 5: Create program object */
	std::string fileContent = "";

	cgraFile file(fileAddress);
	file.read(fileContent);

	const char *source = fileContent.c_str();
	size_t sourceSize[] = {(size_t)fileContent.size()};
	program = clCreateProgramWithSource(OpenclManager::getInstance()->getContent(), 1, &source, sourceSize, NULL);

	/**Step 6: Build program. */
	uint32_t err = clBuildProgram(program, 1, OpenclManager::getInstance()->getDevices(), NULL, NULL, NULL);
	if (err != 0) {
		size_t logSize;

		clGetProgramBuildInfo(program, *OpenclManager::getInstance()->getDevices(), CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char* log = nullptr;
		log = (char*)CGRA_MALLOC((logSize+1)*sizeof(char), OPENCL_TASK);
		log[logSize] = '\0';
		clGetProgramBuildInfo(program, *OpenclManager::getInstance()->getDevices(), CL_PROGRAM_BUILD_LOG, logSize, log, NULL);

		CGRA_LOGE("error:\n%s", log);
		CGRA_FREE(log);
	}

	CGRA_ACTIVITY_END(OPENCL_TASK);
}

void OpenclTask::run() {

}

OpenclTask::~OpenclTask()
{
	clReleaseProgram(program);
}

} // namespace CGRA350

