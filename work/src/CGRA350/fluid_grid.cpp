#include "fluid_grid.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

namespace CGRA350 {

FluidGrid::FluidGrid()
#ifdef __CPU_FLUID_SIMULATION__
	: _cube(nullptr)
#else
	: _cube(CGRA_SRCDIR "/res/openCL/fluid_simulation.cl", 50, 0.0, 0.001, 1)
#endif
	, _size(50)
	, _diffusion(0.0)
	, _viscosity(0.001)
	, _dt(1)
	, _vertices(nullptr)
	, _VAO(0)
	, _VBO(0)
{
#ifdef __CPU_FLUID_SIMULATION__
	_cube = CPU_FluidCubeCreate(_size, _diffusion, _viscosity, _dt);
#else
	// nothing
	_cube.run();
#endif
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	_vertices = (float*)CGRA_CALLOC(_size * _size * _size * 4, sizeof(float), CGRA350);


}

FluidGrid::~FluidGrid()
{
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeFree(_cube);
#else
	// nothing
#endif

	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);

	if (_vertices != nullptr)
	{
		CGRA_FREE(_vertices);
		_vertices = nullptr;
	}
}

void FluidGrid::addDensity(int x, int y, int z, float amount)
{
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeAddDensity(_cube,  x,  y,  z,  amount);
#else
	_cube.GPU_FluidCubeAddDensity(x,  y,  z,  amount);
#endif
}

void FluidGrid::addVelocity(int x, int y, int z, float amountX, float amountY, float amountZ)
{
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeAddVelocity(_cube,  x,  y,  z,  amountX,  amountY,  amountZ);
#else
	_cube.GPU_FluidCubeAddVelocity(x,  y,  z,  amountX,  amountY,  amountZ);
#endif
}

void FluidGrid::renderGUI()
{
	static float amount = 200;
	static float vx = 10;
	static float vy = 10;
	static float vz = 10;

	ImGui::SliderFloat("Diffusion", &_diffusion, 0.000f, 1.0f);
	ImGui::SliderFloat("Viscosity", &_viscosity, 0.001f, 1.0f);
	ImGui::SliderFloat("Amount", &amount, 0.0f, 500.0f);

	ImGui::SliderFloat("Velocity X", &vx, -50.0f, 50.0f);
	ImGui::SliderFloat("Velocity Y", &vy, -50.0f, 50.0f);
	ImGui::SliderFloat("Velocity Z", &vz, -50.0f, 50.0f);

#ifdef __CPU_FLUID_SIMULATION__
	_cube->diff = _diffusion;
	_cube->visc = _viscosity;
#else
	_cube.setDiffusion(_diffusion);
	_cube.setViscosity(_viscosity);
#endif
	addDensity(_size / 2, _size / 2, _size / 2, amount);
	addVelocity(_size / 2, _size / 2, _size / 2, vx, vy, vz);
#ifdef __CPU_FLUID_SIMULATION__
	if (ImGui::Button("Reset")) CPU_FluidCubeReset(_cube);
#else
	if (ImGui::Button("Reset")) _cube.GPU_FluidCubeReset();
#endif

}

void FluidGrid::update()
{
	CGRA_ACTIVITY_START(CALCULATE_VOL);
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeStep(_cube);
#else
	_cube.GPU_FluidCubeStep();
#endif
	CGRA_ACTIVITY_END(CALCULATE_VOL);

	CGRA_ACTIVITY_START(GL_SET_RENDER_DATA);
	for (int i = 0; i < _size; ++i) {
		for (int j = 0; j < _size; ++j) {
			for (int k = 0; k < _size; ++k) {
				int N = _size;
				int index = IX(i, j, k);
				_vertices[4 * index + 0] = (float(i) / (_size / 2) - 1) * 10;
				_vertices[4 * index + 1] = (float(j) / (_size / 2) - 1) * 10;
				_vertices[4 * index + 2] = (float(k) / (_size / 2) - 1) * 10;
				_vertices[4 * index + 3] = 0;
#ifdef __CPU_FLUID_SIMULATION__
				_vertices[4 * index + 3] = _cube->density[index];
#else
				_vertices[4 * index + 3] = _cube.getDensity()[index];
#endif
			}
		}
	}
	CGRA_ACTIVITY_END(GL_SET_RENDER_DATA);

	CGRA_ACTIVITY_START(GL_SET_OPENGL_BUFFER);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _size * _size * _size * 4 * sizeof(float), _vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	CGRA_ACTIVITY_END(GL_SET_OPENGL_BUFFER);
}

void FluidGrid::render()
{
	CGRA_ACTIVITY_START(GL_RENDER_VOL_DATA);
	glBindVertexArray(_VAO);
	glDrawArrays(GL_POINTS, 0, _size * _size * _size);
	glBindVertexArray(0);
	CGRA_ACTIVITY_END(GL_RENDER_VOL_DATA);
}



} // namespace CGRA350
