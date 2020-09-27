#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <cstdlib>
#include <limits>

namespace CGRA350 {

const double g_INFINITY = std::numeric_limits<double>::infinity();

const double PI = 3.14159265335897932;

const double E = 2.71828182845904523536;

class Vec2 {
public:
	float x;
	float y;

	Vec2();

	Vec2(float _x, float _y);

	inline bool operator== (const Vec2& vec2) const
	{
		return (this->x == vec2.x && this->y == vec2.y);
	}
};

inline Vec2 operator+(const Vec2 &u, const Vec2 &v) {
	return Vec2(u.x + v.x, u.y + v.y);
}

inline Vec2 operator-(const Vec2 &u, const Vec2 &v) {
	return Vec2(u.x - v.x, u.y - v.y);
}

class Vec3 {
public:
	float x;
	float y;
	float z;

	Vec3();

	Vec3(float _x, float _y, float _z);

	Vec3 operator-() const;

	Vec3& operator+=(const Vec3& vec3);

	Vec3& operator*=(const float t);

	Vec3& operator/=(const float t);

	float length() const;

	Vec3 normalize() const;
};

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(float t, const Vec3 &v) {
	return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3 &v, float t) {
	return t * v;
}

inline Vec3 operator/(Vec3 v, float t) {
	return (1 / t) * v;
}

inline float dot(const Vec3 &u, const Vec3 &v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.y * v.z - u.z * v.y,
	            u.z * v.x - u.x * v.z,
	            u.x * v.y - u.y * v.x);
}

inline Vec3 normalize(Vec3 v) {
	return v / v.length();
}

float norm2(Vec3 a);

float norm2(Vec3 a, Vec3 b);

} // namespace CGRA350

#endif // VECTOR_H