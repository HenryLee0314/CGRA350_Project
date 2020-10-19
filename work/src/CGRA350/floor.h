#ifndef FLOOR_H
#define FLOOR_H

#include <stdint.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "vector.h"

namespace CGRA350 {

class Floor
{
public:
	static Floor* getInstance();

	void render();

private:
	Floor();
	virtual ~Floor();
	Floor(const Floor&);
	Floor& operator = (const Floor&);

private:
	static Floor* _instance;

private:
	Vec3 a;
	Vec3 b;
	Vec3 c;
	Vec3 d;

	uint32_t _size;

	float* _vertices;
	uint32_t* _indices;

	uint32_t _VAO;
	uint32_t _VBO;
	uint32_t _EBO;
};

} // namespace CGRA350

#endif // FLOOR_H