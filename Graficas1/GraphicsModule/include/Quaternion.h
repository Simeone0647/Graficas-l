#pragma once
#include "Vector3.h"

class Quaternion
{
public:

	Quaternion();
	~Quaternion();

	void SetValues(Vector3 _Vec, float _w);
private:
	Vector3 m_Vector;
	float m_w;
};

