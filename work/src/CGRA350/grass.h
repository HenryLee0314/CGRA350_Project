#ifndef GRASS_H
#define GRASS_H

#include <cstdint>

#include "vector.h"
#include "object.h"

namespace CGRA350 {

class Grass : public Object
{
public:
	Grass(Vec3 a, Vec3 b, Vec3 c, Vec3 d, Object* parent = nullptr);

	~Grass();

	void update();

	void render();

	virtual void updateGlData();

private:
	const static size_t VERTICES_SIZE = 4;

	float _static_angle;

	Vec3* _vertices;

	Vec3 _G_static;
	Vec3 _Ee_static[VERTICES_SIZE - 1];



	Vec3 _G_current;
	Vec3 _Ee[VERTICES_SIZE - 1];
	Vec3 _Ew[VERTICES_SIZE - 1];
	Vec3 _En[VERTICES_SIZE - 1];

	int _index[VERTICES_SIZE];
	Vec3 _velocity[VERTICES_SIZE];

	Vec3 _Ws[VERTICES_SIZE - 1];

	float _delta_theta_s;
	Vec3 _Rs[VERTICES_SIZE - 1];

	Vec3 _Wb[VERTICES_SIZE - 1];

	Vec3 _Rb[VERTICES_SIZE - 1];

	Vec3 _Wt[VERTICES_SIZE - 1];

	Vec3 _Rt[VERTICES_SIZE - 1];

	Vec3 _F[VERTICES_SIZE - 1];
	Vec3 _N[VERTICES_SIZE - 1];

	uint32_t _VAO;
	uint32_t _VBO;

};

} // namespace CGRA350

#endif // GRASS_H