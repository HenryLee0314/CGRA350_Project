#include "vector.h"

namespace CGRA350 {

float norm2(Vec3 a)
{
	return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

float norm2(Vec3 a, Vec3 b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

Vec2::Vec2()
	: x(0.0)
	, y(0.0)
{

}

Vec2::Vec2(float _x, float _y)
	: x(_x)
	, y(_y)
{

}

Vec3::Vec3()
	: x(0.0)
	, y(0.0)
	, z(0.0)
{

}


Vec3::Vec3(float _x, float _y, float _z)
	: x(_x)
	, y(_y)
	, z(_z)
{

}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3& Vec3::operator+=(const Vec3& vec3) {
	x += vec3.x;
	y += vec3.y;
	z += vec3.z;
	return *this;
}

Vec3& Vec3::operator*=(const float t) {
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

Vec3& Vec3::operator/=(const float t) {
	x /= t;
	y /= t;
	z /= t;
	return *this;
}

float Vec3::length() const {
	return norm2(*this);
}

Vec3 Vec3::normalize() const {
	return (*this) / length();
}

}
