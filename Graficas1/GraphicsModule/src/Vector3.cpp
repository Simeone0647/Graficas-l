#pragma once
#include "Vector3.h"

Vector3::Vector3() : m_PosX{ 0 }, m_PosY{ 0 }, m_PosZ{ 0 }, m_A{ 0 }
{
}

Vector3::Vector3(float x, float y, float z) : m_A{ 0 }
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

Vector3::~Vector3()
{
}

Vector3 Vector3::Normalize(Vector3& v)
{
	float lenght = sqrt(((v.GetX()) * v.GetX()) + (v.GetY() * v.GetY()) + (v.GetZ() * v.GetZ()));

	return Vector3(v.GetX() / lenght, v.GetY() / lenght, v.GetZ() / lenght);
}

Vector3 Vector3::CrossProduct(Vector3& v1, Vector3& v2)
{
	Vector3 crossproduct;
	crossproduct.SetValues((v1.GetY() * v2.GetZ()) - (v2.GetY() * v1.GetZ()), -((v1.GetX() * v2.GetZ()) - (v2.GetX() * v1.GetZ())),
							((v1.GetX() * v2.GetY()) - (v2.GetX() * v1.GetY())));
	return crossproduct;
}

float Vector3::DotProduct(Vector3& v1, Vector3& v2)
{
	float product = (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY()) + (v1.GetZ() * v2.GetZ());
	return product;
}
