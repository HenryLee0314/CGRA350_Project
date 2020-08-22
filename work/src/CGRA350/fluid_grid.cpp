#include "fluid_grid.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

namespace CGRA350 {

FluidGrid::FluidGrid()
	: _cube(nullptr)
	, _size(20)
	, _diffusion(0.2)
	, _viscosity(0.1)
	, _dt(1)
	, _vertices(nullptr)
	, _VAO(0)
	, _VBO(0)
{
	_cube = FluidCubeCreate(_size, _diffusion, _viscosity, _dt);

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	_vertices = (float*)calloc(_size * _size * _size * 4, sizeof(float));
}

FluidGrid::~FluidGrid()
{
	FluidCubeFree(_cube);

	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);

	if (_vertices != nullptr)
	{
		free(_vertices);
		_vertices = nullptr;
	}
}

void FluidGrid::addDensity(int x, int y, int z, float amount)
{
	FluidCubeAddDensity(_cube,  x,  y,  z,  amount);
}

void FluidGrid::addVelocity(int x, int y, int z, float amountX, float amountY, float amountZ)
{
	FluidCubeAddVelocity(_cube,  x,  y,  z,  amountX,  amountY,  amountZ);
}

void FluidGrid::renderGUI()
{
	static float amount = 500;
	static float vx = 10;
	static float vy = 10;
	static float vz = 10;

	ImGui::SliderFloat("Diffusion", &_diffusion, 0.0f, 10.0f);
	ImGui::SliderFloat("Viscosity", &_viscosity, 0.001f, 2.0f);
	ImGui::SliderFloat("Amount", &amount, 0.0f, 1000.0f);

	ImGui::SliderFloat("Velocity X", &vx, -50.0f, 50.0f);
	ImGui::SliderFloat("Velocity Y", &vy, -50.0f, 50.0f);
	ImGui::SliderFloat("Velocity Z", &vz, -50.0f, 50.0f);


	_cube->diff = _diffusion;
	_cube->visc = _viscosity;
	addDensity(_size / 2, _size / 2, _size / 2, amount);
	addVelocity(_size / 2, _size / 2, _size / 2, vx, vy, vz);

	if (ImGui::Button("Reset")) FluidCubeReset(_cube);


}

void FluidGrid::update()
{
	FluidCubeStep(_cube);

	for (int i = 0; i < _size; ++i) {
		for (int j = 0; j < _size; ++j) {
			for (int k = 0; k < _size; ++k) {
				int N = _size;
				int index = IX(i, j, k);
				_vertices[4 * index + 0] = i - (_size / 2);
				_vertices[4 * index + 1] = j - (_size / 2);
				_vertices[4 * index + 2] = k - (_size / 2);
				_vertices[4 * index + 3] = _cube->density[index];
			}
		}
	}

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _size * _size * _size * 4 * sizeof(float), _vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void FluidGrid::render()
{
	glBindVertexArray(_VAO);
	glDrawArrays(GL_POINTS, 0, _size * _size * _size);
	glBindVertexArray(0);
}



} // namespace CGRA350
