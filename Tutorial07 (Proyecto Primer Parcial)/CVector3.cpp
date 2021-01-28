#pragma once
#include "CVector3.h"

CVector3::CVector3() : m_posx{ 0 }, m_posy{ 0 }, m_posz{ 0 }, m_a{ 0 }
{
}

CVector3::CVector3(float x, float y, float z) : m_a{ 0 }
{
	m_posx = x;
	m_posy = y;
	m_posz = z;
}

CVector3::~CVector3()
{
}

CVector3 CVector3::Normalize(CVector3& v)
{
	float lenght = sqrt(((v.getX()) * v.getX()) + (v.getY() * v.getY()) + (v.getZ() * v.getZ()));

	return CVector3(v.getX() / lenght, v.getY() / lenght, v.getZ() / lenght);
}

CVector3 CVector3::CrossProduct(CVector3& v1, CVector3& v2)
{
	CVector3 crossproduct;
	crossproduct.setValues((v1.getY() * v2.getZ()) - (v2.getY() * v1.getZ()), -((v1.getX() * v2.getZ()) - (v2.getX() * v1.getZ())),
							((v1.getX() * v2.getY()) - (v2.getX() * v1.getY())));
	return crossproduct;
}

float CVector3::DotProduct(CVector3& v1, CVector3& v2)
{
	float product = (v1.getX() * v2.getX()) + (v1.getY() * v2.getY()) + (v1.getZ() * v2.getZ());
	return product;
}

CVector3 CVector3::getValues()
{
	CVector3 position;
	position.setValues(position.getX(), position.getY(), position.getZ());
	return position;
}

