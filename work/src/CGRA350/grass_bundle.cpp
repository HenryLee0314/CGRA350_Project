#include "grass_bundle.h"

#include <cstdlib>
#include <ctime>

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include "boost/bind.hpp"
#include "boost/asio.hpp"
#include "boost/asio/thread_pool.hpp"

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
	std::srand(std::time(nullptr));

	for (int i = -7; i <= 7; ++i) {
		for (int j = -7; j <= 7; ++j) {
			createGrass(i + getRandomNumber(), j + getRandomNumber());
			createGrass(i + getRandomNumber(), j + getRandomNumber());
			createGrass(i + getRandomNumber(), j + getRandomNumber());
			createGrass(i + getRandomNumber(), j + getRandomNumber());
		}
	}

}

float GrassBundle::getRandomNumber(float scale)
{
	float ret = (float(std::rand()) / (RAND_MAX / 2) - 1) * scale;
	return ret;
}

Vec3 GrassBundle::getRandomPoint(float scale)
{
	return Vec3(getRandomNumber(scale), getRandomNumber(scale), getRandomNumber(scale));
}

void GrassBundle::createGrass(float x, float z)
{
	float temp = 0.4;
	Vec3 offset = Vec3(x, 0, z);
	Grass* grass0 = new Grass(offset + temp * Vec3(0, 0, 0), offset + temp * (getRandomPoint(0.1) + Vec3(2, 2.5, 3)), offset + temp * (getRandomPoint(0.1) + Vec3(3, 3, 3)), offset + temp * (getRandomPoint(0.1) + Vec3(4, 1, 4)), this);
	Grass* grass1 = new Grass(offset + temp * Vec3(0, 0, 0), offset + temp * (getRandomPoint(0.1) + Vec3(-2, 2.5, 3)), offset + temp * (getRandomPoint(0.1) + Vec3(-3, 3, 3)), offset + temp * (getRandomPoint(0.1) + Vec3(-4, 1, 4)), this);
	Grass* grass2 = new Grass(offset + temp * Vec3(0, 0, 0), offset + temp * (getRandomPoint(0.1) + Vec3(2, 2.5, -3)), offset + temp * (getRandomPoint(0.1) + Vec3(3, 3, -3)), offset + temp * (getRandomPoint(0.1) + Vec3(4, 1, -4)), this);
	Grass* grass3 = new Grass(offset + temp * Vec3(0, 0, 0), offset + temp * (getRandomPoint(0.1) + Vec3(-2, 2.5, -3)), offset + temp * (getRandomPoint(0.1) + Vec3(-3, 3, -3)), offset + temp * (getRandomPoint(0.1) + Vec3(-4, 1, -4)), this);
}

void GrassBundle::update()
{
	CGRA_ACTIVITY_START(GRASS_UPDATE);

	boost::asio::thread_pool pool(4);

	std::vector<std::shared_ptr<Object>>::iterator iter;
	for (iter = _children.begin(); iter != _children.end(); iter++) {
		boost::asio::post(pool, std::bind(&Object::update, *iter));
	}
	pool.join();



	// std::vector<std::shared_ptr<Object>>::iterator iter;
	// for (iter = _children.begin(); iter != _children.end(); iter++) {
	// 	(*iter)->update();
	// }
	

	Object::updateGlData();

	CGRA_ACTIVITY_END(GRASS_UPDATE);
}

GrassBundle::~GrassBundle()
{

}

} // namespace CGRA350