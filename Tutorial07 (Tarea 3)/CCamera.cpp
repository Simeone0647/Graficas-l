#pragma once
#include "CCamera.h"

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

XMMATRIX CCamera::getViewMatrix()
{
	
	CVector3 zaxis = CVector3::Normalize(m_at - m_eye);
	CVector3 xaxis = CVector3::Normalize(CVector3::CrossProduct(m_up, zaxis));
	CVector3 yaxis = CVector3::CrossProduct(zaxis, xaxis);

	//m_translationmatrix = { 1.0f, 0.0f, 0.0f, -m_eye.getX(),
	//						0.0f, 1.0f, 0.0f, -m_eye.getY(),
	//						0.0f, 0.0f, 1.0f, -m_eye.getZ(),
	//						0.0f, 0.0f, 0.0f, 1.0f };
	//
	//XMMATRIX rotationM = { xaxis.getX(), xaxis.getY(), xaxis.getZ(), 0.0f,
	//				 yaxis.getX(), yaxis.getY(), yaxis.getZ(), 0.0f,
	//				 zaxis.getX(), zaxis.getY(), zaxis.getZ(), 0.0f,
	//				 0.0f,		   0.0f,		 0.0f,		   1.0f };

	//m_viewmatrix = rotationM * m_translationmatrix;
	//m_viewmatrix = { xaxis.getX(), xaxis.getY(), xaxis.getZ(), -CVector3::DotProduct(xaxis, m_eye),
	//				 yaxis.getX(), yaxis.getY(), yaxis.getZ(), -CVector3::DotProduct(yaxis, m_eye),
	//				 zaxis.getX(), zaxis.getY(), zaxis.getZ(), -CVector3::DotProduct(zaxis, m_eye),
	//				 0.0f,		   0.0f,		 0.0f,		   1.0f };

	m_viewmatrix = { xaxis.getX(), yaxis.getX(), zaxis.getX(), 0.0f,
					 xaxis.getY(), yaxis.getY(), zaxis.getY(), 0.0f,
					 xaxis.getZ(), yaxis.getZ(), zaxis.getZ(), 0.0f,
					 -CVector3::DotProduct(xaxis, m_eye), -CVector3::DotProduct(yaxis, m_eye), -CVector3::DotProduct(zaxis, m_eye),  1.0f };
	


	return m_viewmatrix;
}

XMMATRIX CCamera::getPerspectiveProjectionMatrix(float angle_y, float ratio, float nearp, float farp)
{

	float halfangle = angle_y / 2;
	float senhalfangle = sin(halfangle);
	float coshalfangle = cos(halfangle);
	float y = coshalfangle / senhalfangle;
	float x = y / ratio;
	float z = farp / (farp - nearp);
	float zt = (-farp * nearp) / (farp - nearp);

	m_projectionmatrix = { x, 0, 0, 0,
						   0, y, 0, 0,
						   0, 0, z, 1,
						   0, 0, zt, 0 };

	return m_projectionmatrix;
}

XMMATRIX CCamera::getOrtographicProjectionMatrix(float left, float right, float bottom, float top, float nearp, float farp)
{
	float x = 2.0f / right - left;
	float y = 2.0f / bottom - top;
	float z = 1.0f / (farp - nearp);

	//float xx = -(right + left) / (right - left);
	//float yy = -(top + bottom) / (bottom - top);
	float zz = -z * nearp;

	m_projectionmatrix = { x, 0.0f, 0.0f, 0.0f,
						   0.0f, y, 0.0f, 0.0f,
						   0.0f, 0.0f, z, 0.0f,
						   0.0f, 0.0f, zz, 1.0f };

	return m_projectionmatrix;
}


