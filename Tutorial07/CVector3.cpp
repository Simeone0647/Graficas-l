#pragma once
#include "CVector3.h"

CVector3::CVector3() : m_PosX{ 0 }, m_PosY{ 0 }, m_PosZ{ 0 }, m_A{ 0 }
{
}

CVector3::CVector3(float x, float y, float z) : m_A{ 0 }
{
	m_PosX = x;
	m_PosY = y;
	m_PosZ = z;
}

CVector3::~CVector3()
{
}

CVector3 CVector3::Normalize(CVector3& v)
{
	float lenght = sqrt(((v.GetX()) * v.GetX()) + (v.GetY() * v.GetY()) + (v.GetZ() * v.GetZ()));

	return CVector3(v.GetX() / lenght, v.GetY() / lenght, v.GetZ() / lenght);
}

CVector3 CVector3::CrossProduct(CVector3& v1, CVector3& v2)
{
	CVector3 crossproduct;
	crossproduct.SetValues((v1.GetY() * v2.GetZ()) - (v2.GetY() * v1.GetZ()), -((v1.GetX() * v2.GetZ()) - (v2.GetX() * v1.GetZ())),
							((v1.GetX() * v2.GetY()) - (v2.GetX() * v1.GetY())));
	return crossproduct;
}

float CVector3::DotProduct(CVector3& v1, CVector3& v2)
{
	float product = (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY()) + (v1.GetZ() * v2.GetZ());
	return product;
}
