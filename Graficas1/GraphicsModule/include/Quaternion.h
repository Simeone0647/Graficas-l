#pragma once
#include "Vector3.h"

class Quaternion
{
public:

	Quaternion();
	~Quaternion();
private:
	Vector3 m_Vector;
	float m_w;
};

