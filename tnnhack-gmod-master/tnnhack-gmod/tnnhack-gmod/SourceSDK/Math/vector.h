#pragma once

#include <math.h>

// 2

class Vector2f
{
public:
	float x, y;
};

class Vector2i
{
public:
	int x, y;
};

class Vector2d
{
public:
	double x, y;
};

// 3

class Vector3f
{
public:
	float x, y, z;
public:
	float Distance3D(const Vector3f& other)
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
	}

	float Distance2D(const Vector3f& other)
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.z - this->z, 2));
	}

	float DotProduct(const Vector3f& other)
	{
		return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
	}

	float Length()
	{
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}

	Vector3f operator-(const Vector3f& other)
	{
		Vector3f r;
		r.x = this->x - other.x;
		r.y = this->y - other.y;
		r.z = this->z - other.z;
		return r;
	}

	Vector3f operator*(const Vector3f& other)
	{
		Vector3f r;
		r.x = this->x * other.x;
		r.y = this->y * other.y;
		r.z = this->z * other.z;
		return r;
	}

	Vector3f operator+(const Vector3f& other)
	{
		Vector3f r;
		r.x = this->x + other.x;
		r.y = this->y + other.y;
		r.z = this->z + other.z;
		return r;
	}

	Vector3f operator*(const float other)
	{
		Vector3f r;
		r.x = this->x * other;
		r.y = this->y * other;
		r.z = this->z * other;
		return r;
	}
};

class Vector3i
{
public:
	int x, y, z;
public:
	float Distance3D(const Vector3i& other)
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
	}

	float Distance2D(const Vector3i& other)
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.z - this->z, 2));
	}

	float DotProduct(const Vector3i& other)
	{
		return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
	}

	float Length()
	{
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}
};

class Vector3d
{
public:
	double x, y, z;
public:
	float Distance3D(const Vector3d& other)
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
	}

	float Distance2D(const Vector3d& other)
	{
		return sqrt(pow(other.x - this->x, 2) + pow(other.z - this->z, 2));
	}

	float DotProduct(const Vector3d& other)
	{
		return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
	}

	float Length()
	{
		return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
	}
};

// 4

class Vector4f
{
public:
	float x, y, z, w;
};

class Vector4i
{
public:
	int x, y, z, w;
};

class Vector4d
{
public:
	double x, y, z, w;
};