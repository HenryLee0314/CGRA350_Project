#ifndef FLUID_GRID_H
#define FLUID_GRID_H

#include <cstdint>

// #define __CPU_FLUID_SIMULATION__

#ifdef __CPU_FLUID_SIMULATION__
#include "cpu_fluid_simulation.h"
#else
#include "gpu_fluid_simulation.h"
#endif

namespace CGRA350 {

class FluidGrid
{
public:
	FluidGrid();
	~FluidGrid();

	void addDensity(int x, int y, int z, float amount);

	void addVelocity(int x, int y, int z, float amountX, float amountY, float amountZ);

	void update();

	void render();

	void renderGUI();

private:
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCube* _cube;
#else
	GPU_FluidCube _cube;
#endif

	int _size;
	float _diffusion;
	float _viscosity;
	float _dt;

	float* _vertices;

	uint32_t _VAO;
	uint32_t _VBO;

};

} // namespace CGRA350

#endif // FLUID_GRID_H
