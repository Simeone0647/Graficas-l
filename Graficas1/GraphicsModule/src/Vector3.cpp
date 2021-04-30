#pragma once
#include "Vector3.h"

Vector3::Vector3() : m_X{ 0 }, m_Y{ 0 }, m_Z{ 0 }, m_A{ 0 }
{
}

Vector3::Vector3(float _X, float _Y, float _Z) : m_A{ 0 }
{
	m_X = _X;
	m_Y = _Y;
	m_Z = _Z;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::Normalize(Vector3& _Vector)
{
	float Lenght = sqrt((_Vector.m_X * _Vector.m_X) + (_Vector.m_Y * _Vector.m_Y) + (_Vector.m_Z * _Vector.m_Z));

	return Vector3(_Vector.m_X / Lenght, _Vector.m_Y / Lenght, _Vector.m_Z / Lenght);
}

Vector3 Vector3::CrossProduct(Vector3& _V1, Vector3& _V2)
{
	Vector3 CrossProduct;
	CrossProduct.m_X = (_V1.m_Y * _V2.m_Z) - (_V2.m_Y * _V1.m_Z);
	CrossProduct.m_Y = -((_V1.m_X * _V2.m_Z) - (_V2.m_X * _V1.m_Z));
	CrossProduct.m_Z = (_V1.m_X * _V2.m_Y) - (_V2.m_X * _V1.m_Y);
	//CrossProduct.SetValues((v1.GetY() * v2.GetZ()) - (v2.GetY() * v1.GetZ()), -((v1.GetX() * v2.GetZ()) - (v2.GetX() * v1.GetZ())),
	//						((v1.GetX() * v2.GetY()) - (v2.GetX() * v1.GetY())));
	return CrossProduct;
}

float Vector3::DotProduct(Vector3& _V1, Vector3& _V2)
{
	float Product = (_V1.m_X * _V2.m_X) + (_V1.m_Y * _V2.m_Y) + (_V1.m_Z * _V2.m_Z);
	return Product;
}

Vector3 Vector3::operator-(const Vector3& _Vector)
{
	Vector3 Vec;

	Vec.m_X = this->m_X - _Vector.m_X;
	Vec.m_Y = this->m_Y - _Vector.m_Y;
	Vec.m_Z = this->m_Z - _Vector.m_Z;

	return Vec;
}
