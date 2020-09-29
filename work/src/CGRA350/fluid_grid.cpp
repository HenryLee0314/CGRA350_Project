#include "fluid_grid.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include "vector.h"

#define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

namespace CGRA350 {

FluidGrid* FluidGrid::_instance = nullptr;

FluidGrid* FluidGrid::getInstance()
{
	if (_instance == nullptr) {
		_instance = new FluidGrid();
	}

	return _instance;
}

FluidGrid::FluidGrid()
	: _size(50)
	, _diffusion(0.0)
	, _viscosity(0.001)
	, _dt(1)
	, _vertices(nullptr)
	, _VAO(0)
	, _VBO(0)
#ifdef __CPU_FLUID_SIMULATION__
	, _cube(nullptr)
#else // __GPU_FLUID_SIMULATION__
	, _cube(CGRA_SRCDIR "/res/openCL/fluid_simulation.cl", _size, _diffusion, _viscosity, _dt)
#endif
	, _position()
	, _direction()
	, _velocity_coefficient(10)
{

#ifdef __CPU_FLUID_SIMULATION__
	_cube = CPU_FluidCubeCreate(_size, _diffusion, _viscosity, _dt);
#else // __GPU_FLUID_SIMULATION__
	// nothing
#endif

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	_vertices = (float*)CGRA_CALLOC(_size * _size * _size * (3 + 1 + 3), sizeof(float), CGRA350);
}

FluidGrid::~FluidGrid()
{
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeFree(_cube);
#else // __GPU_FLUID_SIMULATION__
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

int FluidGrid::getIndexFromPosition(float x, float y, float z)
{
	int N = _size;
	float a = (x / _FIELD_RADIUS_ + 1) * (N / 2);
	float b = (y / _FIELD_RADIUS_) * (N);
	float c = (z / _FIELD_RADIUS_ + 1) * (N / 2);

	return IX((int)a, (int)b, (int)c);
}

Vec3 FluidGrid::getVec3IndexFromPosition(float x, float y, float z)
{
	int N = _size;
	float a = (x / _FIELD_RADIUS_ + 1) * (N / 2);
	float b = (y / _FIELD_RADIUS_) * (N);
	float c = (z / _FIELD_RADIUS_ + 1) * (N / 2);

	return Vec3(a, b, c);
}

void FluidGrid::addDensity(int x, int y, int z, float amount)
{
	// CGRA_LOGD("%d %d %d", x, y, z);
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeAddDensity(_cube,  x,  y,  z,  amount);
#else // __GPU_FLUID_SIMULATION__
	_cube.GPU_FluidCubeAddDensity(x,  y,  z,  amount);
#endif
}

void FluidGrid::addVelocity(int x, int y, int z, float amountX, float amountY, float amountZ)
{
	// CGRA_LOGD("%d %d %d %f %f %f", x, y, z, amountX, amountY, amountZ);
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeAddVelocity(_cube,  x,  y,  z,  amountX,  amountY,  amountZ);
#else // __GPU_FLUID_SIMULATION__
	_cube.GPU_FluidCubeAddVelocity(x,  y,  z,  amountX,  amountY,  amountZ);
#endif
}

void FluidGrid::renderGUI()
{
	static float amount = 200;
	static float angle = 0;

#ifdef __CPU_FLUID_SIMULATION__
	if (ImGui::Button("Reset")) CPU_FluidCubeReset(_cube);
#else // __GPU_FLUID_SIMULATION__
	if (ImGui::Button("Reset")) _cube.GPU_FluidCubeReset();
#endif

	ImGui::SliderFloat("Diffusion", &_diffusion, 0.000f, 1.0f);
	ImGui::SliderFloat("Viscosity", &_viscosity, 0.001f, 1.0f);

	ImGui::SliderFloat("Amount", &amount, 0.0f, 500.0f);

	ImGui::SliderFloat("angle", &angle, 0.0f, 2 * 3.1415926535897932);
	_position = Vec3((_FIELD_RADIUS_ - 0.5) * cos(angle), 0.5, (_FIELD_RADIUS_ - 0.5) * sin(angle));
	// CGRA_LOGD("_position %f %f %f", _position.x, _position.y, _position.z);
	Vec3 position = getVec3IndexFromPosition(_position.x, _position.y, _position.z);
	// CGRA_LOGD("position %d %d %d", (int)position.x, (int)position.y, (int)position.z);


	_direction = Vec3(0, 0, 0) - _position;
	_direction = _direction.normalize();
	// CGRA_LOGD("_direction %f %f %f", _direction.x, _direction.y, _direction.z);


	ImGui::SliderFloat("_velocity_coefficient", &_velocity_coefficient, 0, 50.0f);

	addDensity((int)position.x, (int)position.y, (int)position.z, amount);
	addVelocity((int)position.x, (int)position.y, (int)position.z,  _direction.x * _velocity_coefficient, _direction.y * _velocity_coefficient, _direction.z * _velocity_coefficient);

#ifdef __CPU_FLUID_SIMULATION__
	_cube->diff = _diffusion;
	_cube->visc = _viscosity;
#else // __GPU_FLUID_SIMULATION__
	_cube.setDiffusion(_diffusion);
	_cube.setViscosity(_viscosity);
#endif


}

void FluidGrid::update()
{
	CGRA_ACTIVITY_START(CALCULATE_VOL);
#ifdef __CPU_FLUID_SIMULATION__
	CPU_FluidCubeStep(_cube);
#else // __GPU_FLUID_SIMULATION__
	_cube.GPU_FluidCubeStep();
#endif
	CGRA_ACTIVITY_END(CALCULATE_VOL);

	CGRA_ACTIVITY_START(GL_SET_RENDER_DATA);
	for (int i = 0; i < _size; ++i) {
		for (int j = 0; j < _size; ++j) {
			for (int k = 0; k < _size; ++k) {
				int N = _size;
				int index = IX(i, j, k);
				_vertices[(3 + 1 + 3) * index + 0] = (float(i) / (_size / 2) - 1) * _FIELD_RADIUS_;
				_vertices[(3 + 1 + 3) * index + 1] = (float(j) / (_size)) * _FIELD_RADIUS_;
				_vertices[(3 + 1 + 3) * index + 2] = (float(k) / (_size / 2) - 1) * _FIELD_RADIUS_;

#ifdef __CPU_FLUID_SIMULATION__
				_vertices[(3 + 1 + 3) * index + 3] = _cube->density[index];
				_vertices[(3 + 1 + 3) * index + 4] = _cube->Vx[index];
				_vertices[(3 + 1 + 3) * index + 5] = _cube->Vy[index];
				_vertices[(3 + 1 + 3) * index + 6] = _cube->Vz[index];
#else // __GPU_FLUID_SIMULATION__
				_vertices[(3 + 1 + 3) * index + 3] = _cube.getDensity()[index];
				_vertices[(3 + 1 + 3) * index + 4] = _cube.getVx()[index];
				_vertices[(3 + 1 + 3) * index + 5] = _cube.getVy()[index];
				_vertices[(3 + 1 + 3) * index + 6] = _cube.getVz()[index];
#endif
			}
		}
	}
	CGRA_ACTIVITY_END(GL_SET_RENDER_DATA);

	CGRA_ACTIVITY_START(GL_SET_OPENGL_BUFFER);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _size * _size * _size * (3 + 1 + 3) * sizeof(float), _vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 1 + 3) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, (3 + 1 + 3) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (3 + 1 + 3) * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);
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
