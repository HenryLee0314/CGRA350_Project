#ifndef FLUID_GRID_H
#define FLUID_GRID_H

#include <cstdint>
#include "vector.h"

//#define __CPU_FLUID_SIMULATION__

#ifdef __CPU_FLUID_SIMULATION__
#include "cpu_fluid_simulation.h"
#else
#include "gpu_fluid_simulation.h"
#endif

namespace CGRA350 {

class FluidGrid
{
public:
	void addDensity(int x, int y, int z, float amount);

	void addVelocity(int x, int y, int z, float amountX, float amountY, float amountZ);

	Vec3 getVelocity(int index) {
#ifdef __CPU_FLUID_SIMULATION__
		return Vec3(_cube->Vx[index], _cube->Vy[index], _cube->Vz[index]) * _cube->density[index];
#else // __GPU_FLUID_SIMULATION__
		return _cube.getVelocity(index) * _cube.getDensity()[index];
#endif
	}

	void update();

	void render();

	void renderGUI();

	int getIndexFromPosition(float x, float y, float z);

	Vec3 getVec3IndexFromPosition(float x, float y, float z);

public:
	static FluidGrid* getInstance();

private:
	FluidGrid();
	virtual ~FluidGrid();
	FluidGrid(const FluidGrid&);
	FluidGrid& operator = (const FluidGrid&);

private:
	static FluidGrid* _instance;

private:
	const static unsigned int _FIELD_RADIUS_ = 10;
	int _size;
	float _diffusion;
	float _viscosity;
	float _dt;

	float* _vertices;

	uint32_t _VAO;
	uint32_t _VBO;

#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCube* _cube;
#else // __GPU_FLUID_SIMULATION__
	GPU_FluidCube _cube;
#endif

	Vec3 _position;
	Vec3 _direction;
	float _velocity_coefficient;

};

} // namespace CGRA350

#endif // FLUID_GRID_H
