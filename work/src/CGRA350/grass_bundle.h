#ifndef grass_bundle_h
#define grass_bundle_h

#include "grass.h"
#include "object.h"

namespace CGRA350 {

class GrassBundle : public Object
{
public:
	static GrassBundle* getInstance();

private:
	GrassBundle(Object* parent = nullptr);
	virtual ~GrassBundle();
	GrassBundle(const GrassBundle&);
	GrassBundle& operator = (const GrassBundle&);

private:
	static GrassBundle* _instance;
};

} // namespace CGRA350

#endif // grass_bundle_h