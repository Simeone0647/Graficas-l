#pragma once
#include "CCamera.h"

CCamera::CCamera()
{
	m_projectionmatrix = new float[16];
	m_translationmatrix = new float[16];
	m_viewmatrix = new float[16];
}

CCamera::~CCamera()
{
	if (m_projectionmatrix != nullptr)
	{
		delete[] m_projectionmatrix;
		m_projectionmatrix = nullptr;
	}
	if (m_viewmatrix != nullptr)
	{
		delete[] m_viewmatrix;
		m_viewmatrix = nullptr;
	}
	if (m_translationmatrix != nullptr)
	{
		delete[] m_translationmatrix;
		m_translationmatrix = nullptr;
	}
}

void CCamera::UpdateViewMatrix()
{
	CVector3 zaxis = CVector3::Normalize(m_at - m_eye);//Front
	m_at.setValues(m_eye.getX() + zaxis.getX(), m_eye.getY() + zaxis.getY(), m_eye.getZ() + zaxis.getZ());
	CVector3 xaxis = CVector3::Normalize(CVector3::CrossProduct(m_up, zaxis));//right
	CVector3 yaxis = CVector3::CrossProduct(zaxis, xaxis);//Up

	m_viewmatrix[0] = xaxis.getX();
	m_viewmatrix[1] = yaxis.getX();
	m_viewmatrix[2] = zaxis.getX();
	m_viewmatrix[3] = 0.0f;

	m_viewmatrix[4] = xaxis.getY();
	m_viewmatrix[5] = yaxis.getY();
	m_viewmatrix[6] = zaxis.getY();
	m_viewmatrix[7] = 0.0f;

	m_viewmatrix[8] = xaxis.getZ();
	m_viewmatrix[9] = yaxis.getZ();
	m_viewmatrix[10] = zaxis.getZ();
	m_viewmatrix[11] = 0.0f;

	m_viewmatrix[12] = -CVector3::DotProduct(xaxis, m_eye);
	m_viewmatrix[13] = -CVector3::DotProduct(yaxis, m_eye);
	m_viewmatrix[14] = -CVector3::DotProduct(zaxis, m_eye);
	m_viewmatrix[15] = 1.0f;

	//m_viewmatrix = { xaxis.getX(), yaxis.getX(), zaxis.getX(), 0.0f,
	//				 xaxis.getY(), yaxis.getY(), zaxis.getY(), 0.0f,
	//				 xaxis.getZ(), yaxis.getZ(), zaxis.getZ(), 0.0f,
	//				 -CVector3::DotProduct(xaxis, m_eye), -CVector3::DotProduct(yaxis, m_eye), -CVector3::DotProduct(zaxis, m_eye),  1.0f };
}

void CCamera::UpdatePerspectiveProjectionMatrix(float angle_y, float ratio, float nearp, float farp)
{
	float halfangle = angle_y / 2;
	float senhalfangle = sin(halfangle);
	float coshalfangle = cos(halfangle);
	float y = coshalfangle / senhalfangle;
	float x = y / ratio;
	float z = farp / (farp - nearp);
	float zt = (-farp * nearp) / (farp - nearp);

	m_projectionmatrix[0] = x;
	m_projectionmatrix[1] = 0.0f;
	m_projectionmatrix[2] = 0.0f;
	m_projectionmatrix[3] = 0.0f;

	m_projectionmatrix[4] = 0.0f;
	m_projectionmatrix[5] = y;
	m_projectionmatrix[6] = 0.0f;
	m_projectionmatrix[7] = 0.0f;

	m_projectionmatrix[8] = 0.0f;
	m_projectionmatrix[9] = 0.0f;
	m_projectionmatrix[10] = z;
	m_projectionmatrix[11] = 1.0f;

	m_projectionmatrix[12] = 0.0f;
	m_projectionmatrix[13] = 0.0f;
	m_projectionmatrix[14] = zt;
	m_projectionmatrix[15] = 0.0f;

	//m_projectionmatrix = { x, 0, 0, 0,
	//					   0, y, 0, 0,
	//					   0, 0, z, 1,
	//					   0, 0, zt, 0 };

}

float* CCamera::getPerspectiveProjectionMatrix()
{
	return m_projectionmatrix;
}

void CCamera::UpdateOrtographicProjectionMatrix(float left, float right, float bottom, float top, float nearp, float farp)
{
	float x = 2.0f / ((right - left) / 100);
	float y = 2.0f / ((bottom - top) / 100);
	float z = 1.0f / (farp - nearp);

	float zz = -z * nearp;

	m_projectionmatrix[0] = x;
	m_projectionmatrix[1] = 0.0f;
	m_projectionmatrix[2] = 0.0f;
	m_projectionmatrix[3] = 0.0f;

	m_projectionmatrix[4] = 0.0f;
	m_projectionmatrix[5] = y;
	m_projectionmatrix[6] = 0.0f;
	m_projectionmatrix[7] = 0.0f;

	m_projectionmatrix[8] = 0.0f;
	m_projectionmatrix[9] = 0.0f;
	m_projectionmatrix[10] = z;
	m_projectionmatrix[11] = 0.0f;

	m_projectionmatrix[12] = 0.0f;
	m_projectionmatrix[13] = 0.0f;
	m_projectionmatrix[14] = zz;
	m_projectionmatrix[15] = 1.0f;

	m_mousepos[0] = 0;
	m_mousepos[1] = 0;
	//m_projectionmatrix = { x, 0.0f, 0.0f, 0.0f,
	//					   0.0f, y, 0.0f, 0.0f,
	//					   0.0f, 0.0f, z, 0.0f,
	//					   0.0f, 0.0f, zz, 1.0f };

}

float* CCamera::getOrtographicProjectionMatrix()
{
	return m_projectionmatrix;
}

void CCamera::MoveCamera(CVector3 mov)
{
	m_eye.setValues(m_eye.getX() + mov.getX(), m_eye.getY() + mov.getY(), m_eye.getZ() + mov.getZ());
	m_at.setValues(m_at.getX() + mov.getX(), m_at.getY() + mov.getY(), m_at.getZ() + mov.getZ());
}

void CCamera::RotateCamera(CVector3 mov)
{
	mov.setValues(mov.getX() / 1000.0f, mov.getY() / 1000.0f, mov.getZ() / 1000.0f);
	//mov = CVector3::Normalize(mov);
	m_at.setValues(m_at.getX() + mov.getX(), m_at.getY() + mov.getY(), m_at.getZ());
}