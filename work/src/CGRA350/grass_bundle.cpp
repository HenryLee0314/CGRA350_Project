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

GrassBundle::GrassBundle()
	: _grass(Vec3(0, 0, 0), Vec3(2, 2.5, 3), Vec3(3, 3, 3), Vec3(4, 0.5, 4))
{

}

GrassBundle::~GrassBundle()
{

}

void GrassBundle::render()
{
	_grass.render();
}

} // namespace CGRA350