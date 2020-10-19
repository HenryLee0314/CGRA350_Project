#ifndef grass_bundle_h
#define grass_bundle_h

#include "grass.h"
#include "object.h"

namespace CGRA350 {

class GrassBundle : public Object
{
public:
	static GrassBundle* getInstance();

	virtual void update();

private:
	GrassBundle(Object* parent = nullptr);
	virtual ~GrassBundle();
	GrassBundle(const GrassBundle&);
	GrassBundle& operator = (const GrassBundle&);

private:
	void createGrass(float x, float z);

	float getRandomNumber(float scale = 1.0);

	Vec3 getRandomPoint(float scale = 1.0);

private:
	static GrassBundle* _instance;
};

} // namespace CGRA350

#endif // grass_bundle_h