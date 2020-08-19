#include "grass.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

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
