#pragma once

class Matrix3x4f
{
public:
	float val[3][4];

	float* operator[](int index)
	{
		return this->val[index];
	}
};

class Matrix4x4f
{
public:
	float val[4][4];

	float* operator[](int index)
	{
		return this->val[index];
	}
};