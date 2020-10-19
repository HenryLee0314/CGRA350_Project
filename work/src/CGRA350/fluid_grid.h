#ifndef FLUID_GRID_H
#define FLUID_GRID_H

#include <cstdint>
#include "vector.h"

#include "gpu_fluid_simulation.h"

namespace CGRA350 {

class FluidGrid
{
public:
	void addDensity(int x, int y, int z, float amount);

	void addVelocity(int x, int y, int z, float amountX, float amountY, float amountZ);

	Vec3 getVelocity(int index) {
		return _cube.getVelocity(index) * _cube.getDensity()[index];
	}

	void update();

	void wait();

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

	GPU_FluidCube _cube;

	Vec3 _position;
	Vec3 _direction;
	float _velocity_coefficient;

	bool _renderFluidGrid;

};

} // namespace CGRA350

#endif // FLUID_GRID_H
