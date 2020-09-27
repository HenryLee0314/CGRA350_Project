#include "grass.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include "fluid_grid.h"

#define IX(x, y, z) ((x) + (y) * N + (z) * N * N)

namespace CGRA350 {

const size_t Grass::VERTICES_SIZE = 4;

Grass::Grass()
	: _hasChanged(false)
	, _vertices(nullptr)
	, _VAO(0)
	, _VBO(0)
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);


	_vertices = (Vec3*)CGRA_MALLOC(sizeof(Vec3) * VERTICES_SIZE, CGRA350);

	_vertices[0].x = 0.0;
	_vertices[0].y = 0.0;
	_vertices[0].z = 0.0;

	_vertices[1].x = 1.0;
	_vertices[1].y = 1.0;
	_vertices[1].z = 1.0;

	_vertices[2].x = 2.0;
	_vertices[2].y = 2.0;
	_vertices[2].z = 2.0;

	_vertices[3].x = 3.0;
	_vertices[3].y = 3.0;
	_vertices[3].z = 3.0;

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
	ImGui::SliderFloat("0.x", &_vertices[0].x, -5.0f, 5.0f);
	ImGui::SliderFloat("0.y", &_vertices[0].y, -5.0f, 5.0f);
	ImGui::SliderFloat("0.z", &_vertices[0].z, -5.0f, 5.0f);

	ImGui::SliderFloat("1.x", &_vertices[1].x, -5.0f, 5.0f);
	ImGui::SliderFloat("1.y", &_vertices[1].y, -5.0f, 5.0f);
	ImGui::SliderFloat("1.z", &_vertices[1].z, -5.0f, 5.0f);

	ImGui::SliderFloat("2.x", &_vertices[2].x, -5.0f, 5.0f);
	ImGui::SliderFloat("2.y", &_vertices[2].y, -5.0f, 5.0f);
	ImGui::SliderFloat("2.z", &_vertices[2].z, -5.0f, 5.0f);

	ImGui::SliderFloat("3.x", &_vertices[3].x, -5.0f, 5.0f);
	ImGui::SliderFloat("3.y", &_vertices[3].y, -5.0f, 5.0f);
	ImGui::SliderFloat("3.z", &_vertices[3].z, -5.0f, 5.0f);

	_G = _vertices[3] - _vertices[0];

	_Ee[0] = _vertices[1] - _vertices[0];
	_Ee[1] = _vertices[2] - _vertices[1];
	_Ee[2] = _vertices[3] - _vertices[2];

	_Ew[0] = cross(_Ee[0], Vec3(0, 1, 0)).normalize();
	_Ew[1] = cross(_Ee[1], Vec3(0, 1, 0)).normalize();
	_Ew[2] = cross(_Ee[2], Vec3(0, 1, 0)).normalize();

	_En[0] = cross(_Ee[0], _Ew[0]).normalize();
	_En[1] = cross(_Ee[1], _Ew[1]).normalize();
	_En[2] = cross(_Ee[2], _Ew[2]).normalize();

	int N = 50;

	int index0 = IX((_vertices[0].x / 10 + 1) * (N / 2), (_vertices[0].y / 10 + 1) * (N / 2), (_vertices[0].z / 10 + 1) * (N / 2));
	int index1 = IX((_vertices[1].x / 10 + 1) * (N / 2), (_vertices[1].y / 10 + 1) * (N / 2), (_vertices[1].z / 10 + 1) * (N / 2));
	int index2 = IX((_vertices[2].x / 10 + 1) * (N / 2), (_vertices[2].y / 10 + 1) * (N / 2), (_vertices[2].z / 10 + 1) * (N / 2));
	int index3 = IX((_vertices[3].x / 10 + 1) * (N / 2), (_vertices[3].y / 10 + 1) * (N / 2), (_vertices[3].z / 10 + 1) * (N / 2));

	Vec3 V0 = FluidGrid::getInstance()->getVelocity(index0);
	Vec3 V1 = FluidGrid::getInstance()->getVelocity(index1);
	Vec3 V2 = FluidGrid::getInstance()->getVelocity(index2);
	Vec3 V3 = FluidGrid::getInstance()->getVelocity(index3);

	CGRA_LOGD("0 %f %f %f", V0.x, V0.y, V0.z);
	CGRA_LOGD("1 %f %f %f", V1.x, V1.y, V1.z);
	CGRA_LOGD("2 %f %f %f", V2.x, V2.y, V2.z);
	CGRA_LOGD("3 %f %f %f", V3.x, V3.y, V3.z);

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
	//glDrawArrays(GL_POINTS, 0, VERTICES_SIZE);
	//glDrawArrays(GL_LINE_STRIP, 0, VERTICES_SIZE);

	glDrawArrays(GL_PATCHES, 0, VERTICES_SIZE);
	glBindVertexArray(0);

}

} // namespace CGRA350
