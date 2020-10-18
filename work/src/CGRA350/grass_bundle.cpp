#include "grass_bundle.h"

namespace CGRA350 {

GrassBundle* GrassBundle::_instance = nullptr;

GrassBundle* GrassBundle::getInstance()
{
	if (_instance == nullptr) {
		_instance = new GrassBundle();
	}

	return _instance;
}

GrassBundle::GrassBundle(Object* parent)
	: Object(parent)
{
    Grass* grass = new Grass(Vec3(0, 0, 0), Vec3(2, 2.5, 3), Vec3(3, 3, 3), Vec3(4, 0.5, 4), this);
}

GrassBundle::~GrassBundle()
{

}

} // namespace CGRA350