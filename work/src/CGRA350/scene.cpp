#include "scene.h"

namespace CGRA350 {

Scene* Scene::_instance = nullptr;

Scene* Scene::getInstance()
{
	if (_instance == nullptr) {
		_instance = new Scene();
	}

	return _instance;
}

Scene::Scene()
{

}

Scene::~Scene()
{

}

} // namespace CGRA350
