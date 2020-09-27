#ifndef GRASS_H
#define GRASS_H

#include <cstdint>

#include "vector.h"

namespace CGRA350 {

class Grass
{
public:
	Grass();

	~Grass();

	void renderGUI();

	void render();

private:
	void update();

private:
	bool _hasChanged;

	Vec3* _vertices;

	Vec3 _G;
	Vec3 _Ee[3];
	Vec3 _Ew[3];
	Vec3 _En[3];


	uint32_t _VAO;
	uint32_t _VBO;

	const static size_t VERTICES_SIZE;
};

} // namespace CGRA350

#endif // GRASS_H