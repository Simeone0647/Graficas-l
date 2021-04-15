#include "Quaternion.h"

Quaternion::Quaternion()
{
}

Quaternion::~Quaternion()
{
}

void Quaternion::SetValues(Vector3 _Vec, float _w)
{
	m_Vector.SetValues(_Vec.GetX(), _Vec.GetY(), _Vec.GetZ());
	m_w = _w;
}
