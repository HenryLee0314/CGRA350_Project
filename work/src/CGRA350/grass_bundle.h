#ifndef grass_bundle_h
#define grass_bundle_h

#include "grass.h"

namespace CGRA350 {

class GrassBundle
{
public:
	static GrassBundle* getInstance();

	void render();

private:
	GrassBundle();
	virtual ~GrassBundle();
	GrassBundle(const GrassBundle&);
	GrassBundle& operator = (const GrassBundle&);

private:
	static GrassBundle* _instance;

	Grass _grass;
};

} // namespace CGRA350

#endif // grass_bundle_h