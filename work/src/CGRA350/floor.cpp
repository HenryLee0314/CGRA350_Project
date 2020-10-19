#include "floor.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"
#include "vector.h"

namespace CGRA350 {

static const Vec3 FLOOR_VERTICES_A(10.0f, 0.0f, -10.0f);
static const Vec3 FLOOR_VERTICES_B(10.0f, 0.0f, 10.0f);
static const Vec3 FLOOR_VERTICES_C(-10.0f, 0.0f, 10.0f);
static const Vec3 FLOOR_VERTICES_D(-10.0f, 0.0f, -10.0f);

Floor* Floor::_instance = nullptr;

Floor* Floor::getInstance()
{
	if (_instance == nullptr) {
		_instance = new Floor();
	}

	return _instance;
}

Floor::Floor()
	: a(FLOOR_VERTICES_A)
	, b(FLOOR_VERTICES_B)
	, c(FLOOR_VERTICES_C)
	, d(FLOOR_VERTICES_D)
	, _size(1)
	, _vertices(nullptr)
	, _indices(nullptr)
	, _VAO(0)
	, _VBO(0)
	, _EBO(0)
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);

	if (_vertices != nullptr) {
		CGRA_FREE(_vertices);
		_vertices = nullptr;
	}

	if (_indices != nullptr) {
		CGRA_FREE(_indices);
		_indices = nullptr;
	}

	_vertices = (float*)CGRA_MALLOC(sizeof(float) * ((_size + 1) * (_size + 1) * (3 + 3)), CGRA350);
	_indices = (uint32_t*)CGRA_MALLOC(sizeof(uint32_t) * (_size * _size * 3 * 2), CGRA350);
	for (int i = 0; i <= _size; ++i) {
		for (int j = 0; j <= _size; ++j) {
			Vec3 X1 = (b - c) / _size * j + c;
			Vec3 X2 = (a - d) / _size * j + d;
			Vec3 point = (X2 - X1) / _size * i + X1;

			_vertices[6 * (i * (_size + 1) + j) + 0] = point.x;
			_vertices[6 * (i * (_size + 1) + j) + 1] = point.y;
			_vertices[6 * (i * (_size + 1) + j) + 2] = point.z;


			_vertices[6 * (i * (_size + 1) + j) + 3] = 0.0f;
			_vertices[6 * (i * (_size + 1) + j) + 4] = 1.0f;
			_vertices[6 * (i * (_size + 1) + j) + 5] = 0.0f;


			if (i < _size && j < _size) {
				_indices[6 * (i * _size + j) + 0] = i * (_size + 1) + j;
				_indices[6 * (i * _size + j) + 1] = (i + 1) * (_size + 1) + j;
				_indices[6 * (i * _size + j) + 2] = (i + 1) * (_size + 1) + (j + 1);

				_indices[6 * (i * _size + j) + 3] = i * (_size + 1) + j;
				_indices[6 * (i * _size + j) + 4] = (i + 1) * (_size + 1) + (j + 1);
				_indices[6 * (i * _size + j) + 5] = i * (_size + 1) + (j + 1);

			}
		}
	}

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((_size + 1) * (_size + 1) * (3 + 3)), _vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * (_size * _size * 3 * 2), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

}

Floor::~Floor()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
}

void Floor::render()
{
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _size * _size * 2 * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

} // namespace CGRA350