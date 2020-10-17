#include "grass.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "fluid_grid.h"

namespace CGRA350 {

float Grass::_sigma = 10000;
float Grass::_k_tip = 10;
float Grass::_angle_coefficient = 0.5;

Grass::Grass(Vec3 a, Vec3 b, Vec3 c, Vec3 d)
	: _hasChanged(false)
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

	_Ee_static[0] = (_vertices[1] - _vertices[0]).normalize();
	_Ee_static[1] = (_vertices[2] - _vertices[1]).normalize();
	_Ee_static[2] = (_vertices[3] - _vertices[2]).normalize();

	_G_static = _vertices[3] - _vertices[0];
	_G_static = Vec3(_G_static.x, 0, _G_static.z);

	update();
}

Grass::~Grass()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);

	CGRA_FREE(_vertices);
	_vertices = nullptr;
}

void Grass::renderGUI()
{
	ImGui::SliderFloat("drag coefficient [_sigma]", &_sigma, 0, 100000);
	ImGui::SliderFloat("stiffness coefficient [_k_tip]", &_k_tip, 0, 100);
	ImGui::SliderFloat("angle coefficient [_angle_coefficient]", &_angle_coefficient, 0, 1);

	_G_current = _vertices[3] - _vertices[0];
	_G_current = Vec3(_G_current.x, 0, _G_current.z);

	_Ee[0] = (_vertices[1] - _vertices[0]).normalize();
	_Ee[1] = (_vertices[2] - _vertices[1]).normalize();
	_Ee[2] = (_vertices[3] - _vertices[2]).normalize();

	_Ew[0] = cross(_Ee[0], Vec3(0, 1, 0)).normalize();
	_Ew[1] = cross(_Ee[1], Vec3(0, 1, 0)).normalize();
	_Ew[2] = cross(_Ee[2], Vec3(0, 1, 0)).normalize();

	_En[0] = cross(_Ee[0], _Ew[0]).normalize();
	_En[1] = cross(_Ee[1], _Ew[1]).normalize();
	_En[2] = cross(_Ee[2], _Ew[2]).normalize();

	for (size_t i = 0; i < VERTICES_SIZE; ++i) {
		CGRA_LOGD("%zu %f %f %f", i, _vertices[i].x, _vertices[i].y, _vertices[i].z);
		_index[i] = FluidGrid::getInstance()->getIndexFromPosition(_vertices[i].x, _vertices[i].y, _vertices[i].z);
		CGRA_LOGD("index[i] %d", _index[i]);

		_velocity[i] = FluidGrid::getInstance()->getVelocity(_index[i]);
		CGRA_LOGD("velocity[i] %f %f %f", _velocity[i].x, _velocity[i].y, _velocity[i].z);
	}







	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		_Ws[i] = _sigma * dot(_velocity[i + 1], _Ew[i]) * _Ew[i];
		CGRA_LOGD("_Ws %d %f %f %f", i, _Ws[i].x , _Ws[i].y, _Ws[i].z);
	}



	float dotResult = dot(_G_static.normalize(), _G_current.normalize());
	CGRA_LOGD("dot result: %f", dotResult);
	if (dotResult <= -1) {
		_delta_theta_s = 3.1415926;
	}
	else if (dotResult >= 1) {
		_delta_theta_s = 0;
	}
	else {
		_delta_theta_s = acos(dotResult);
	}
	CGRA_LOGD("_delta_theta_s %f", _delta_theta_s);

	Vec3 deltaG = _G_static - _G_current;
	CGRA_LOGD("deltaG: %f %f %f", deltaG.x, deltaG.y, deltaG.z);

	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		if (deltaG.length() == 0 || !_delta_theta_s) {
			_Rs[i] = Vec3(0, 0, 0);
		}
		else {
			_Rs[i] = _k_tip * _delta_theta_s * deltaG.normalize();
		}
		CGRA_LOGD("_Rs %d %f %f %f", i, _Rs[i].x , _Rs[i].y, _Rs[i].z);
	}










	// for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
	// 	_Wb[i] = Vec3(0, 0, 0); // _sigma * dot(_velocity[i], _En[i]) * _En[i];
	// 	CGRA_LOGD("%d %f %f %f", i, _Wb[i].x , _Wb[i].y, _Wb[i].z);
	// }








	// for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
	// 	CGRA_LOGD("(%zu)%f %f %f -> %f %f %f", i, _Ee_static[i].x, _Ee_static[i].y, _Ee_static[i].z, _Ee[i].x, _Ee[i].y, _Ee[i].z);
	// 	float _delta_theta_b = acos(dot(_Ee_static[i], _Ee[i]));
	// 	CGRA_LOGD("%zu _delta_theta_b %f", i, _delta_theta_b);
	// 	_Rb[i] = Vec3(0, 0, 0); // _k_tip * _delta_theta_b * Vec3(_Ee_static[i] -  _Ee[i]).normalize();
	// 	CGRA_LOGD("%d %f %f %f", i, _Rb[i].x , _Rb[i].y, _Rb[i].z);
	// }


	// for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
	// 	Vec3 E_local_n = _En[i];
	// 	_Wt[i] = _sigma * dot(_velocity[i], E_local_n) * E_local_n;
	// 	// CGRA_LOGD("%d %f %f %f", i, _Wt[i].x , _Wt[i].y, _Wt[i].z);
	// }







	float mass = 1;
	float length[3];
	length[0] = (_vertices[1] - _vertices[0]).length();
	length[1] = (_vertices[2] - _vertices[1]).length();
	length[2] = (_vertices[3] - _vertices[2]).length();
	float total_length = length[0] + length[1] + length[2];

	Vec3 N[3];
	float I[3];

	Vec3 w[3];

	float angle[3];
	glm::mat4 rotation[3] = {glm::mat4(1.0), glm::mat4(1.0), glm::mat4(1.0)};

	for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
		I[i] = mass * length[i] * length[i];
		_F[i] = _Ws[i] + _Rs[i];// + _Wb[i] + _Rb[i];

		// Vec3 dir = cross(Vec3(0, 1, 0), _Ee[i]).normalize();
		// if (dot(_F[i], dir) < 0) {
		// 	dir = -dir;
		// }
		// dir = _F[i].length() * dir * dot(_F[i], dir);
		// angle[i] = atan(dir.z / (dir.x != 0 ? dir.x : 0.000001)) * _angle_coefficient;


		N[i] = cross(_Ee[i], _F[i]);
		w[i] = N[i] / I[i];
		CGRA_LOGD("w[i] %f %f %f", w[i].x, w[i].y, w[i].z);
		angle[i] = atan(w[i].z / (w[i].x != 0 ? w[i].x : 0.000001)) * _angle_coefficient;

		CGRA_LOGD("angle %f", angle[i]);
		rotation[i] = glm::rotate(rotation[i], angle[i], glm::vec3(0, 1, 0));
	}

	// Vec3 F = _F[0] + _F[1] + _F[2];
	// F = F / 3;
	// CGRA_LOGD("F %f %f %f", F.x, F.y, F.z);
	Vec3 W = w[0] + w[1] + w[2];
	W = W / 3;

	if (W.length() > 0) {
		// Vec3 N = cross(Vec3(0, 1, 0), F);
		// CGRA_LOGD("N %f %f %f", N.x , N.y, N.z);

		// Vec3 w = N / (mass * total_length * total_length);
		// CGRA_LOGD("w %f %f %f", w.x , w.y, w.z);

		// float angle = atan(W.z / W.x) * _angle_coefficient;
		// CGRA_LOGD("angle %f", angle);


		// glm::mat4 rotation = glm::mat4(1.0);
		// rotation = glm::rotate(rotation, angle, glm::vec3(0, 1, 0));



		glm::vec4 temp[4];
		for (int i = 1; i < 4; ++i) {
			temp[i] = glm::vec4(_vertices[i].x, _vertices[i].y, _vertices[i].z, 1);
			temp[i] = rotation[i - 1] * temp[i];
			_vertices[i] = Vec3(temp[i].x, temp[i].y, temp[i].z);
			CGRA_LOGD("after update: vertices %f %f %f", _vertices[i].x, _vertices[i].y, _vertices[i].z);
		}
	}




	// for (size_t i = 0; i < VERTICES_SIZE - 1; ++i) {
	// 	_F[i] = _Ws[i] + _Rs[i] + _Wb[i] + _Rb[i];
	// 	_N[i] = cross(_Ee[i], _F[i]);

	// 	CGRA_LOGD("_F %d %f %f %f", i, _F[i].x , _F[i].y, _F[i].z);
	// 	CGRA_LOGD("_N %d %f %f %f", i, _N[i].x , _N[i].y, _N[i].z);

	// 	Vec3 w = _N[i] / (m * length[i] * length[i]);

	// 	CGRA_LOGD("%d %f %f %f", i, w.x , w.y, w.z);

	// 	Vec3 newPos = _vertices[i + 1] + w - _vertices[i];
	// 	newPos = newPos.normalize();


	// 	_vertices[i + 1] = _vertices[i] + newPos * length[i];

	// 	if (_vertices[i + 1].y < 0) _vertices[i + 1].y = -_vertices[i + 1].y;
	// }






	update();
}

void Grass::update()
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
