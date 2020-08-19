#ifndef GRASS_H
#define GRASS_H

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

	uint32_t _VAO;
	uint32_t _VBO;

	const static size_t VERTICES_SIZE;
};

} // namespace CGRA350

#endif // GRASS_H