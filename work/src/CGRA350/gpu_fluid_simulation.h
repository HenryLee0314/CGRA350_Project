#ifndef GPU_FLUID_SIMULATION_H
#define GPU_FLUID_SIMULATION_H

#include "opencl_task.h"
#include "vector.h"

namespace CGRA350 {

class GPU_FluidCube : public OpenclTask
{
public:
	GPU_FluidCube(const char* const fileAddress, int size, float diffusion, float viscosity, float dt);

	virtual ~GPU_FluidCube();

	virtual void run();

	void GPU_FluidCubeAddDensity(int x, int y, int z, float amount);

	void GPU_FluidCubeAddVelocity(int x, int y, int z, float amountX, float amountY, float amountZ);

	void GPU_FluidCubeStep();

	void GPU_FluidCubeStepWait();

	void GPU_FluidCubeReset();

	void setDiffusion(float diff);

	void setViscosity(float visc);

	float* getDensity() {
		return density;
	}

	float* getVx() {
		return Vx;
	}

	float* getVy() {
		return Vy;
	}

	float* getVz() {
		return Vz;
	}

	Vec3 getVelocity(int index) {
		return Vec3(Vx[index], Vy[index], Vz[index]);
	}

private:
	int _size;
	float _dt;
	float _diff;
	float _visc;

	int _bnd_0;
	int _bnd_1;
	int _bnd_2;
	int _bnd_3;
	int _iter;

	float* density;

	float* Vx;
	float* Vy;
	float* Vz;

	cl_event* _enentPoint;

	cl_mem _cl_mem_s;
	cl_mem _cl_mem_density;

	cl_mem _cl_mem_Vx;
	cl_mem _cl_mem_Vy;
	cl_mem _cl_mem_Vz;

	cl_mem _cl_mem_Vx0;
	cl_mem _cl_mem_Vy0;
	cl_mem _cl_mem_Vz0;

	cl_mem _cl_mem_p;
	cl_mem _cl_mem_div;

	cl_kernel k_diffuse_0;
	cl_kernel k_diffuse_1;
	cl_kernel k_diffuse_2;
	cl_kernel k_diffuse_3;

	cl_kernel k_advect_0;
	cl_kernel k_advect_1;
	cl_kernel k_advect_2;
	cl_kernel k_advect_3;

	cl_kernel k_project_0;
	cl_kernel k_project_1;

	cl_kernel k_addDensity;
	cl_kernel k_addVelocity;
};

} // namespace CGRA350

#endif // GPU_FLUID_SIMULATION_H