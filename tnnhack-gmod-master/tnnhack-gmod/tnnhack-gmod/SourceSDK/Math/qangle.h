#pragma once

#include <algorithm>
#include <functional>
#include "vector.h"

class QAngle
{
public:
	float pitch, yaw, roll;
public:
	QAngle(float p, float y, float r)
	{
		this->pitch = p;
		this->yaw = y;
		this->roll = r;
	}

	QAngle()
	{
		this->pitch = this->yaw = this->roll = 0.f;
	}

	inline QAngle& QAngle::Normalize()
	{
		this->pitch = std::clamp(this->pitch, -89.f, 89.f);
		this->yaw = std::clamp(std::remainder(this->yaw, 360.f), -180.f, 180.f);
		this->roll = 0.f;

		return *this;
	}

	QAngle operator-(const QAngle& other)
	{
		QAngle r;
		r.pitch = this->pitch - other.pitch;
		r.yaw = this->yaw - other.yaw;
		r.roll = this->roll - other.roll;
		return r;
	}

	QAngle& operator-=(const Vector3f& other)
	{
		this->pitch -= other.x;
		this->yaw -= other.y;
		this->roll -= other.z;

		return *this;
	}
};