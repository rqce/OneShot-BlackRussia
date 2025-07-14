#pragma once

#include <cmath>

struct Vec1 {
	float x;
};

struct Vec2 {
	float x, y;
};

struct Vec3 {
	// constructor
	Vec3(float x_, float y_, float z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	// operator overloads
	Vec3 operator-(const Vec3& other)
	{
		return Vec3{ x - other.x, y - other.y, z - other.z };
	}

	Vec3 operator+(const Vec3& other)
	{
		return Vec3{ x + other.x, y + other.y, z + other.z };
	}

	Vec3 operator/(const float factor)
	{
		return Vec3{ x / factor, y / factor, z / factor };
	}

	Vec3 operator*(const float factor)
	{
		return Vec3{ x * factor, y * factor, z * factor };
	}

	// utils
	Vec3 ToAngle()
	{
		return Vec3{
			atan2(-z, hypot(x, y)) * (180.0f / M_PI),
			atan2(y, x) * (180.0f / M_PI),
			0.0f };
	}

	bool IsZero()
	{
		return x == 0.f && y == 0.f && z == 0.f;
	}

	// struct data
	float x, y, z;
};

struct Vec4 {
	float x,y,z,w;
};
