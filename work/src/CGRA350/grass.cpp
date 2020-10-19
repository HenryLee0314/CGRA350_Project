#include "grass.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "fluid_grid.h"
#include "grass_parameters.h"

namespace CGRA350 {

Grass::Grass(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Object* parent)
	: Object(parent)
	, _vertices(nullptr)
	, _VAO(0)
	, _VBO(0)
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);


	_vertices = (Vec3*)CGRA_MALLOC(sizeof(Vec3) * VERTICES_SIZE, CGRA350);

	_vertices[0] = a;
	_vertices[1] = b;
	_vertices[2] = c;
	_vertices[3] = d;

	_Ee_static[0] = (_vertices[1] - _vertices[0]);
	_Ee_static[1] = (_vertices[2] - _vertices[1]);
	_Ee_static[2] = (_vertices[3] - _vertices[2]);

	_G_static = _vertices[3] - _vertices[0];
	_G_static = Vec3(_G_static.x, 0, _G_static.z);

	// _static_angle = atan(_G_static.z/_G_static.x);

	updateGlData();
}

Grass::~Grass()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);

	CGRA_FREE(_vertices);
	_vertices = nullptr;
}

void Grass::update()
{
	_G_current = _vertices[3] - _vertices[0];
	_G_current = Vec3(_G_current.x, 0, _G_current.z);

	_Ee[0] = (_vertices[1] - _vertices[0]);
	_Ee[1] = (_vertices[2] - _vertices[1]);
	_Ee[2] = (_vertices[3] - _vertices[2]);

	_Ew[0] = cross(_Ee[0], Vec3(0, 1, 0)).normalize();
	_Ew[1] = cross(_Ee[1], Vec3(0, 1, 0)).normalize();
	_Ew[2] = cross(_Ee[2], Vec3(0, 1, 0)).normalize();

	_En[0] = cross(_Ee[0], _Ew[0]).normalize();
	_En[1] = cross(_Ee[1], _Ew[1]).normalize();
	_En[2] = cross(_Ee[2], _Ew[2]).normalize();

	for (size_t i = 0; i < VERTICES_SIZE; ++i) {
		// CGRA_LOGD("[position] %f %f %f", _vertices[i].x, _vertices[i].y, _vertices[i].z);
		_index[i] = FluidGrid::getInstance()->getIndexFromPosition(_vertices[i].x, _vertices[i].y, _vertices[i].z);
		// CGRA_LOGD("[index] %d", _index[i]);

		_velocity[i] = FluidGrid::getInstance()->getVelocity(_index[i]);
		// CGRA_LOGD("[velocity] %f %f %f", _velocity[i].x, _velocity[i].y, _velocity[i].z);
	}

	// ========== Ws ==========
	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		_Ws[i] = GrassParameters::getInstance()->getWsCoefficient() * dot(_velocity[i + 1], _Ew[i]) * _Ew[i];
		// CGRA_LOGD("[Ws] %f %f %f", _Ws[i].x , _Ws[i].y, _Ws[i].z);
	}

	// ========== Rs ==========
	float dotResult = dot(_G_static.normalize(), _G_current.normalize());
	// CGRA_LOGD("dot result: %f", dotResult);
	if (dotResult <= -1) {
		_delta_theta_s = 3.1415926;
	}
	else if (dotResult >= 1) {
		_delta_theta_s = 0;
	}
	else {
		_delta_theta_s = acos(dotResult);
	}
	// CGRA_LOGD("_delta_theta_s %f", _delta_theta_s);

	Vec3 deltaG = _G_static - _G_current;
	// CGRA_LOGD("deltaG: %f %f %f", deltaG.x, deltaG.y, deltaG.z);

	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		if (deltaG.length() == 0 || !_delta_theta_s) {
			_Rs[i] = Vec3(0, 0, 0);
		}
		else {
			_Rs[i] = GrassParameters::getInstance()->getRsCoefficient() * _delta_theta_s * deltaG.normalize();
		}
		// CGRA_LOGD("[Rs] %d %f %f %f", i, _Rs[i].x , _Rs[i].y, _Rs[i].z);
	}

	// ========== Wb ==========
	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		_Wb[i] = GrassParameters::getInstance()->getWbCoefficient() * dot(_velocity[i + 1], _En[i]) * _En[i];
		// CGRA_LOGD("[Wb] %f %f %f", _Wb[i].x , _Wb[i].y, _Wb[i].z);
	}

	// ========== Rb ==========
	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		// CGRA_LOGD("[static] -> [current] %f %f %f -> %f %f %f", _Ee_static[i].x, _Ee_static[i].y, _Ee_static[i].z, _Ee[i].x, _Ee[i].y, _Ee[i].z);
		float _delta_theta_b;

		float RbDotResult = dot(_Ee_static[i].normalize(), _Ee[i].normalize());
		// CGRA_LOGD("dot result: %f", dotResult);
		if (RbDotResult <= -1) {
			_delta_theta_b = 3.1415926;
		}
		else if (RbDotResult >= 1) {
			_delta_theta_b = 0;
		}
		else {
			_delta_theta_b = acos(RbDotResult);
		}
		_delta_theta_b *= 0.01;

		// CGRA_LOGD("%zu _delta_theta_b %f", i, _delta_theta_b);
		Vec3 temp = _Ee_static[i] - _Ee[i];
		if (temp.length() == 0) {
			_Rb[i] = Vec3(0, 0, 0);
		}
		else {
			_Rb[i] = GrassParameters::getInstance()->getRbCoefficient() * _delta_theta_b * temp.normalize();
		}

		// CGRA_LOGD("[Rb] %f %f %f", _Rb[i].x , _Rb[i].y, _Rb[i].z);
	}

	float mass = 0.1;

	float length[3];
	length[0] = (_vertices[1] - _vertices[0]).length();
	length[1] = (_vertices[2] - _vertices[1]).length();
	length[2] = (_vertices[3] - _vertices[2]).length();

	float I[3];

	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		I[i] = mass * length[i] * length[i];
		_F[i] = _Ws[i] + _Rs[i] + _Wb[i] + _Rb[i];
		// CGRA_LOGD("[F] %f %f %f", _F[i].x, _F[i].y, _F[i].z);

		_F[i] = Vec3(0, -0.1, 0) + _F[i];
		_F[i] = _F[i] / I[i];

		_vertices[i + 1] = _vertices[i + 1] + GrassParameters::getInstance()->getAngleCoefficient() * _F[i];
		Vec3 dir = _vertices[i + 1] - _vertices[i];
		dir = dir.normalize( ) * _Ee_static[i].length();
		_vertices[i + 1] = dir + _vertices[i];
		if (_vertices[i + 1].y < 0) {
			_vertices[i + 1].y = 0;
		}
		// CGRA_LOGD("after update: vertices %f %f %f", _vertices[i].x, _vertices[i].y, _vertices[i].z);
	}

}

void Grass::updateGlData()
{
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * VERTICES_SIZE, _vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glPatchParameteri(GL_PATCH_VERTICES, VERTICES_SIZE);
	glBindVertexArray(0);
}

void Grass::render()
{
	glBindVertexArray(_VAO);
	glDrawArrays(GL_PATCHES, 0, VERTICES_SIZE);
	glBindVertexArray(0);
}

} // namespace CGRA350
