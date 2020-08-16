#include "sphere_model.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"
#include "vector.h"

namespace CGRA350 {

Sphere* Sphere::_instance = nullptr;

Sphere* Sphere::getInstance()
{
	if (_instance == nullptr) {
		_instance = new Sphere();
	}

	return _instance;
}

Sphere::Sphere()
	: _hasChanged(true)
	, _elevationDivisions(10)
	, _azimuthalAnglesDivisions(10)
	, _vertices(nullptr)
	, _indices(nullptr)
	, _VAO(0)
	, _VBO(0)
	, _EBO(0)
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
}

Sphere::~Sphere()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
}

void Sphere::renderGUI()
{
	if (ImGui::SliderInt("Elevation Divisions", &_elevationDivisions, 2, 40)) {
		_hasChanged = true;
	}

	if (ImGui::SliderInt("Azimuthal Angles Divisions", &_azimuthalAnglesDivisions, 3, 40))
	{
		_hasChanged = true;
	}
}

void Sphere::render()
{
	if (_hasChanged) {
		if (_vertices != nullptr) {
			CGRA_FREE(_vertices);
			_vertices = nullptr;
		}

		if (_indices != nullptr) {
			CGRA_FREE(_indices);
			_indices = nullptr;
		}

		_vertices = (float*)CGRA_MALLOC(sizeof(float) * ((_elevationDivisions + 1) * (_azimuthalAnglesDivisions + 1) * (3 + 3)), CGRA350);
		_indices = (uint32_t*)CGRA_MALLOC(sizeof(uint32_t) * (_elevationDivisions * _azimuthalAnglesDivisions * 3 * 2), CGRA350);

		for (int i = 0; i <= _elevationDivisions; ++i) {
			for (int j = 0; j <= _azimuthalAnglesDivisions; ++j) {
				Vec3 point;
				point.x = cos(PI / _elevationDivisions * i - (PI / 2)) * cos(2 * PI / _azimuthalAnglesDivisions * j);
				point.y = cos(PI / _elevationDivisions * i - (PI / 2)) * sin(2 * PI / _azimuthalAnglesDivisions * j);
				point.z = sin(PI / _elevationDivisions * i - (PI / 2));

				point.unit();

                // vertices
				_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 0] = point.x;
				_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 1] = point.y;
				_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 2] = point.z;

				// normal
				_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 3] = point.x;
				_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 4] = point.y;
				_vertices[6 * (i * (_azimuthalAnglesDivisions + 1) + j) + 5] = point.z;


				if (i < _elevationDivisions && j < _azimuthalAnglesDivisions) {
					_indices[6 * (i * _azimuthalAnglesDivisions + j) + 0] = i * (_azimuthalAnglesDivisions + 1) + j;
					_indices[6 * (i * _azimuthalAnglesDivisions + j) + 1] = (i + 1) * (_azimuthalAnglesDivisions + 1) + j;
					_indices[6 * (i * _azimuthalAnglesDivisions + j) + 2] = (i + 1) * (_azimuthalAnglesDivisions + 1) + (j + 1);

					_indices[6 * (i * _azimuthalAnglesDivisions + j) + 3] = i * (_azimuthalAnglesDivisions + 1) + j;
					_indices[6 * (i * _azimuthalAnglesDivisions + j) + 4] = (i + 1) * (_azimuthalAnglesDivisions + 1) + (j + 1);
					_indices[6 * (i * _azimuthalAnglesDivisions + j) + 5] = i * (_azimuthalAnglesDivisions + 1) + (j + 1);
				}


			}
		}

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((_elevationDivisions + 1) * (_azimuthalAnglesDivisions + 1) * (3 + 3)), _vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * (_elevationDivisions * _azimuthalAnglesDivisions * 3 * 2), _indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		_hasChanged = false;
	}

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _elevationDivisions * _azimuthalAnglesDivisions * 2 * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

}