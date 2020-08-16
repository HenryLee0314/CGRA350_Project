#ifndef SPHERE_MODEL_H
#define SPHERE_MODEL_H

#include <stdint.h>

#include "vector.h"

namespace CGRA350 {

class Sphere
{
public:
	static Sphere* getInstance();

	virtual void renderGUI();

	void render();

protected:
	Sphere();
	virtual ~Sphere();
	Sphere(const Sphere&);
	Sphere& operator = (const Sphere&);
	static Sphere* _instance;

private:
	bool _hasChanged;

	int _elevationDivisions; // min 2
	int _azimuthalAnglesDivisions; // min 3

	float* _vertices;
	uint32_t* _indices;

	uint32_t _VAO;
	uint32_t _VBO;
	uint32_t _EBO;
};

} // namespace CGRA350

#endif // SPHERE_MODEL_H