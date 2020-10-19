#include "gpu_fluid_simulation.h"

#include <cmath>

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include "opencl_manager.h"
#include <iostream>

#define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

namespace CGRA350 {

GPU_FluidCube::GPU_FluidCube(const char* const fileAddress, int size, float diffusion, float viscosity, float dt)
	: OpenclTask(fileAddress)
	, _size(size)
	, _dt(dt)
	, _diff(diffusion)
	, _visc(viscosity)
	, _bnd_0(0)
	, _bnd_1(1)
	, _bnd_2(2)
	, _bnd_3(3)
	, _iter(1)
	, _enentPoint(nullptr)
{
	/**Step 7: Create kernel object */
	k_diffuse_0 = clCreateKernel(program, "diffuse", NULL);
	k_diffuse_1 = clCreateKernel(program, "diffuse", NULL);
	k_diffuse_2 = clCreateKernel(program, "diffuse", NULL);
	k_diffuse_3 = clCreateKernel(program, "diffuse", NULL);

	k_advect_0 = clCreateKernel(program, "advect", NULL);
	k_advect_1 = clCreateKernel(program, "advect", NULL);
	k_advect_2 = clCreateKernel(program, "advect", NULL);
	k_advect_3 = clCreateKernel(program, "advect", NULL);

	k_project_0 = clCreateKernel(program, "project", NULL);
	k_project_1 = clCreateKernel(program, "project", NULL);

	k_addDensity = clCreateKernel(program, "addDensity", NULL);
	k_addVelocity = clCreateKernel(program, "addVelocity", NULL);

	/**Step 8: Initial input,output for the host and create memory objects for the kernel*/
	const int N = _size;
	density = (float*)CGRA_CALLOC(N * N * N, sizeof(float), GPU_FLUID_SIM);

	Vx = (float*)CGRA_CALLOC(N * N * N, sizeof(float), GPU_FLUID_SIM);
	Vy = (float*)CGRA_CALLOC(N * N * N, sizeof(float), GPU_FLUID_SIM);
	Vz = (float*)CGRA_CALLOC(N * N * N, sizeof(float), GPU_FLUID_SIM);

	_cl_mem_s = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_density = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR , N * N * N * sizeof(float), (void*)density, NULL);

	_cl_mem_Vx = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vy = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vz = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);

	_cl_mem_Vx0 = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vy0 = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vz0 = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);

	_cl_mem_p = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_div = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);

	/**Step 9: Sets Kernel arguments.*/
	clSetKernelArg(k_diffuse_1, 0, sizeof(int), (void*)&_bnd_1);
	clSetKernelArg(k_diffuse_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_diffuse_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_diffuse_1, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_1, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_1, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_1, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_2, 0, sizeof(int), (void*)&_bnd_2);
	clSetKernelArg(k_diffuse_2, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_diffuse_2, 2, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_diffuse_2, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_2, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_2, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_2, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_3, 0, sizeof(int), (void*)&_bnd_3);
	clSetKernelArg(k_diffuse_3, 1, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_diffuse_3, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_diffuse_3, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_3, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_3, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_3, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_project_0, 0, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_project_0, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_project_0, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_project_0, 3, sizeof(cl_mem), (void*)&_cl_mem_p);
	clSetKernelArg(k_project_0, 4, sizeof(cl_mem), (void*)&_cl_mem_div);
	clSetKernelArg(k_project_0, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_project_0, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_1, 0, sizeof(int), (void*)&_bnd_1);
	clSetKernelArg(k_advect_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_advect_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_1, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_1, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_1, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_1, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_1, 7, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_2, 0, sizeof(int), (void*)&_bnd_2);
	clSetKernelArg(k_advect_2, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_advect_2, 2, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_2, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_2, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_2, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_2, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_2, 7, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_3, 0, sizeof(int), (void*)&_bnd_3);
	clSetKernelArg(k_advect_3, 1, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_advect_3, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_3, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_3, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_3, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_3, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_3, 7, sizeof(int), (void*)&_size);

	clSetKernelArg(k_project_1, 0, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_project_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_project_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_project_1, 3, sizeof(cl_mem), (void*)&_cl_mem_p);
	clSetKernelArg(k_project_1, 4, sizeof(cl_mem), (void*)&_cl_mem_div);
	clSetKernelArg(k_project_1, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_project_1, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_0, 0, sizeof(int), (void*)&_bnd_0);
	clSetKernelArg(k_diffuse_0, 1, sizeof(cl_mem), (void*)&_cl_mem_s);
	clSetKernelArg(k_diffuse_0, 2, sizeof(cl_mem), (void*)&_cl_mem_density);
	clSetKernelArg(k_diffuse_0, 3, sizeof(float), (void*)&_diff);
	clSetKernelArg(k_diffuse_0, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_0, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_0, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_0, 0, sizeof(int), (void*)&_bnd_0);
	clSetKernelArg(k_advect_0, 1, sizeof(cl_mem), (void*)&_cl_mem_density);
	clSetKernelArg(k_advect_0, 2, sizeof(cl_mem), (void*)&_cl_mem_s);
	clSetKernelArg(k_advect_0, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_advect_0, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_advect_0, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_advect_0, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_0, 7, sizeof(int), (void*)&_size);
}

GPU_FluidCube::~GPU_FluidCube()
{
	CGRA_FREE(density);

	clReleaseMemObject(_cl_mem_s);
	clReleaseMemObject(_cl_mem_density);

	clReleaseMemObject(_cl_mem_Vx);
	clReleaseMemObject(_cl_mem_Vy);
	clReleaseMemObject(_cl_mem_Vz);

	clReleaseMemObject(_cl_mem_Vx0);
	clReleaseMemObject(_cl_mem_Vy0);
	clReleaseMemObject(_cl_mem_Vz0);

	clReleaseMemObject(_cl_mem_p);
	clReleaseMemObject(_cl_mem_div);

	clReleaseKernel(k_diffuse_0);
	clReleaseKernel(k_diffuse_1);
	clReleaseKernel(k_diffuse_2);
	clReleaseKernel(k_diffuse_3);

	clReleaseKernel(k_advect_0);
	clReleaseKernel(k_advect_1);
	clReleaseKernel(k_advect_2);
	clReleaseKernel(k_advect_3);

	clReleaseKernel(k_project_0);
	clReleaseKernel(k_project_1);

	clReleaseKernel(k_addDensity);
	clReleaseKernel(k_addVelocity);
}

void GPU_FluidCube::run()
{
	// nothing
}

void GPU_FluidCube::GPU_FluidCubeAddDensity(int x, int y, int z, float amount)
{
	clSetKernelArg(k_addDensity, 0, sizeof(int), (void*)&x);
	clSetKernelArg(k_addDensity, 1, sizeof(int), (void*)&y);
	clSetKernelArg(k_addDensity, 2, sizeof(int), (void*)&z);
	clSetKernelArg(k_addDensity, 3, sizeof(float), (void*)&amount);
	clSetKernelArg(k_addDensity, 4, sizeof(cl_mem), (void*)&_cl_mem_density);
	clSetKernelArg(k_addDensity, 5, sizeof(int), (void*)&_size);
}

void GPU_FluidCube::GPU_FluidCubeAddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ)
{
	clSetKernelArg(k_addVelocity, 0, sizeof(int), (void*)&x);
	clSetKernelArg(k_addVelocity, 1, sizeof(int), (void*)&y);
	clSetKernelArg(k_addVelocity, 2, sizeof(int), (void*)&z);
	clSetKernelArg(k_addVelocity, 3, sizeof(float), (void*)&amountX);
	clSetKernelArg(k_addVelocity, 4, sizeof(float), (void*)&amountY);
	clSetKernelArg(k_addVelocity, 5, sizeof(float), (void*)&amountZ);
	clSetKernelArg(k_addVelocity, 6, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_addVelocity, 7, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_addVelocity, 8, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_addVelocity, 9, sizeof(int), (void*)&_size);
}

void GPU_FluidCube::setDiffusion(float diff) {
	_diff = diff;

	clSetKernelArg(k_diffuse_0, 0, sizeof(int), (void*)&_bnd_0);
	clSetKernelArg(k_diffuse_0, 1, sizeof(cl_mem), (void*)&_cl_mem_s);
	clSetKernelArg(k_diffuse_0, 2, sizeof(cl_mem), (void*)&_cl_mem_density);
	clSetKernelArg(k_diffuse_0, 3, sizeof(float), (void*)&_diff);
	clSetKernelArg(k_diffuse_0, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_0, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_0, 6, sizeof(int), (void*)&_size);
}

void GPU_FluidCube::setViscosity(float visc) {
	_visc = visc;

	clSetKernelArg(k_diffuse_1, 0, sizeof(int), (void*)&_bnd_1);
	clSetKernelArg(k_diffuse_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_diffuse_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_diffuse_1, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_1, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_1, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_1, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_2, 0, sizeof(int), (void*)&_bnd_2);
	clSetKernelArg(k_diffuse_2, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_diffuse_2, 2, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_diffuse_2, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_2, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_2, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_2, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_3, 0, sizeof(int), (void*)&_bnd_3);
	clSetKernelArg(k_diffuse_3, 1, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_diffuse_3, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_diffuse_3, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_3, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_3, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_3, 6, sizeof(int), (void*)&_size);
}

void GPU_FluidCube::GPU_FluidCubeStep()
{
	CGRA_ACTIVITY_START(GPU_FLUID_SIM);

	/**Step 10: Running the kernel.*/
	size_t global_work_size[3] = {static_cast<size_t>(_size), static_cast<size_t>(_size), static_cast<size_t>(_size)};
	_enentPoint = (cl_event*)CGRA_MALLOC(1 * sizeof(cl_event), CGRA350);

	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_addDensity, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_addVelocity, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);

	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_diffuse_1, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_diffuse_2, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_diffuse_3, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);

	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_project_0, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);

	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_advect_1, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_advect_2, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_advect_3, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);

	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_project_1, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);

	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_diffuse_0, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);
	clEnqueueNDRangeKernel(OpenclManager::getInstance()->getCommandQueue(), k_advect_0, 3, NULL, global_work_size, NULL, 0, NULL, _enentPoint);


    clEnqueueWaitForEvents(OpenclManager::getInstance()->getCommandQueue(), 1, _enentPoint);
}

void GPU_FluidCube::GPU_FluidCubeStepWait()
{
	clWaitForEvents(1, _enentPoint);
	clReleaseEvent(_enentPoint[0]);
	CGRA_FREE(_enentPoint);

	// *Step 11: Read the cout put back to host memory.
	clEnqueueReadBuffer(OpenclManager::getInstance()->getCommandQueue(), _cl_mem_density, CL_TRUE, 0, _size * _size * _size * sizeof(float), density, 0, NULL, NULL);

	clEnqueueReadBuffer(OpenclManager::getInstance()->getCommandQueue(), _cl_mem_Vx, CL_TRUE, 0, _size * _size * _size * sizeof(float), Vx, 0, NULL, NULL);
	clEnqueueReadBuffer(OpenclManager::getInstance()->getCommandQueue(), _cl_mem_Vy, CL_TRUE, 0, _size * _size * _size * sizeof(float), Vy, 0, NULL, NULL);
	clEnqueueReadBuffer(OpenclManager::getInstance()->getCommandQueue(), _cl_mem_Vz, CL_TRUE, 0, _size * _size * _size * sizeof(float), Vz, 0, NULL, NULL);


	CGRA_ACTIVITY_END(GPU_FLUID_SIM);
}

void GPU_FluidCube::GPU_FluidCubeReset()
{
	CGRA_FREE(density);

	density = (float*)CGRA_CALLOC(_size * _size * _size, sizeof(float), GPU_FLUID_SIM);

	clReleaseMemObject(_cl_mem_s);
	clReleaseMemObject(_cl_mem_density);

	clReleaseMemObject(_cl_mem_Vx);
	clReleaseMemObject(_cl_mem_Vy);
	clReleaseMemObject(_cl_mem_Vz);

	clReleaseMemObject(_cl_mem_Vx0);
	clReleaseMemObject(_cl_mem_Vy0);
	clReleaseMemObject(_cl_mem_Vz0);

	clReleaseMemObject(_cl_mem_p);
	clReleaseMemObject(_cl_mem_div);

	int N = _size;

	_cl_mem_s = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_density = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR , N * N * N * sizeof(float), (void*)density, NULL);

	_cl_mem_Vx = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vy = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vz = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);

	_cl_mem_Vx0 = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vy0 = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_Vz0 = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);

	_cl_mem_p = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);
	_cl_mem_div = clCreateBuffer(OpenclManager::getInstance()->getContent(), CL_MEM_READ_WRITE, N * N * N * sizeof(float), NULL, NULL);

	clSetKernelArg(k_diffuse_1, 0, sizeof(int), (void*)&_bnd_1);
	clSetKernelArg(k_diffuse_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_diffuse_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_diffuse_1, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_1, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_1, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_1, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_2, 0, sizeof(int), (void*)&_bnd_2);
	clSetKernelArg(k_diffuse_2, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_diffuse_2, 2, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_diffuse_2, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_2, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_2, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_2, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_3, 0, sizeof(int), (void*)&_bnd_3);
	clSetKernelArg(k_diffuse_3, 1, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_diffuse_3, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_diffuse_3, 3, sizeof(float), (void*)&_visc);
	clSetKernelArg(k_diffuse_3, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_3, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_3, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_project_0, 0, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_project_0, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_project_0, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_project_0, 3, sizeof(cl_mem), (void*)&_cl_mem_p);
	clSetKernelArg(k_project_0, 4, sizeof(cl_mem), (void*)&_cl_mem_div);
	clSetKernelArg(k_project_0, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_project_0, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_1, 0, sizeof(int), (void*)&_bnd_1);
	clSetKernelArg(k_advect_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_advect_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_1, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_1, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_1, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_1, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_1, 7, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_2, 0, sizeof(int), (void*)&_bnd_2);
	clSetKernelArg(k_advect_2, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_advect_2, 2, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_2, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_2, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_2, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_2, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_2, 7, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_3, 0, sizeof(int), (void*)&_bnd_3);
	clSetKernelArg(k_advect_3, 1, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_advect_3, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_3, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx0);
	clSetKernelArg(k_advect_3, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy0);
	clSetKernelArg(k_advect_3, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz0);
	clSetKernelArg(k_advect_3, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_3, 7, sizeof(int), (void*)&_size);

	clSetKernelArg(k_project_1, 0, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_project_1, 1, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_project_1, 2, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_project_1, 3, sizeof(cl_mem), (void*)&_cl_mem_p);
	clSetKernelArg(k_project_1, 4, sizeof(cl_mem), (void*)&_cl_mem_div);
	clSetKernelArg(k_project_1, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_project_1, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_diffuse_0, 0, sizeof(int), (void*)&_bnd_0);
	clSetKernelArg(k_diffuse_0, 1, sizeof(cl_mem), (void*)&_cl_mem_s);
	clSetKernelArg(k_diffuse_0, 2, sizeof(cl_mem), (void*)&_cl_mem_density);
	clSetKernelArg(k_diffuse_0, 3, sizeof(float), (void*)&_diff);
	clSetKernelArg(k_diffuse_0, 4, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_diffuse_0, 5, sizeof(int), (void*)&_iter);
	clSetKernelArg(k_diffuse_0, 6, sizeof(int), (void*)&_size);

	clSetKernelArg(k_advect_0, 0, sizeof(int), (void*)&_bnd_0);
	clSetKernelArg(k_advect_0, 1, sizeof(cl_mem), (void*)&_cl_mem_density);
	clSetKernelArg(k_advect_0, 2, sizeof(cl_mem), (void*)&_cl_mem_s);
	clSetKernelArg(k_advect_0, 3, sizeof(cl_mem), (void*)&_cl_mem_Vx);
	clSetKernelArg(k_advect_0, 4, sizeof(cl_mem), (void*)&_cl_mem_Vy);
	clSetKernelArg(k_advect_0, 5, sizeof(cl_mem), (void*)&_cl_mem_Vz);
	clSetKernelArg(k_advect_0, 6, sizeof(int), (void*)&_dt);
	clSetKernelArg(k_advect_0, 7, sizeof(int), (void*)&_size);
}

} // namespace CGRA350
