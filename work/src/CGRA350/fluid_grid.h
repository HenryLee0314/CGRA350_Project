#ifndef FLUID_GRID_H
#define FLUID_GRID_H

#include <cstdint>

#include "fluid_simulation.h"

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
	FluidCube* _cube;
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
