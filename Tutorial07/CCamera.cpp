#pragma once
#include "CCamera.h"

CCamera::CCamera()
{
	m_ProjectionMatrix = new float[16];
	m_TranslationMatrix = new float[16];
	m_ViewMatrix = new float[16];
}

CCamera::~CCamera()
{
	if (m_ProjectionMatrix != nullptr)
	{
		delete[] m_ProjectionMatrix;
		m_ProjectionMatrix = nullptr;
	}
	if (m_ViewMatrix != nullptr)
	{
		delete[] m_ViewMatrix;
		m_ViewMatrix = nullptr;
	}
	if (m_TranslationMatrix != nullptr)
	{
		delete[] m_TranslationMatrix;
		m_TranslationMatrix = nullptr;
	}
}

void CCamera::UpdateViewMatrix()
{
	CVector3 zaxis = CVector3::Normalize(m_At - m_Eye);//Front
	m_At.SetValues(m_Eye.GetX() + zaxis.GetX(), m_Eye.GetY() + zaxis.GetY(), m_Eye.GetZ() + zaxis.GetZ());
	CVector3 xaxis = CVector3::Normalize(CVector3::CrossProduct(m_Up, zaxis));//right
	CVector3 yaxis = CVector3::CrossProduct(zaxis, xaxis);//Up

	if (m_ViewMatrix != nullptr)
	{
		delete[] m_ViewMatrix;
		m_ViewMatrix = new float[16];
	}

	m_ViewMatrix[0] = xaxis.GetX();
	m_ViewMatrix[1] = yaxis.GetX();
	m_ViewMatrix[2] = zaxis.GetX();
	m_ViewMatrix[3] = 0.0f;

	m_ViewMatrix[4] = xaxis.GetY();
	m_ViewMatrix[5] = yaxis.GetY();
	m_ViewMatrix[6] = zaxis.GetY();
	m_ViewMatrix[7] = 0.0f;

	m_ViewMatrix[8] = xaxis.GetZ();
	m_ViewMatrix[9] = yaxis.GetZ();
	m_ViewMatrix[10] = zaxis.GetZ();
	m_ViewMatrix[11] = 0.0f;

	m_ViewMatrix[12] = -CVector3::DotProduct(xaxis, m_Eye);
	m_ViewMatrix[13] = -CVector3::DotProduct(yaxis, m_Eye);
	m_ViewMatrix[14] = -CVector3::DotProduct(zaxis, m_Eye);
	m_ViewMatrix[15] = 1.0f;

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

	m_ProjectionMatrix[0] = x;
	m_ProjectionMatrix[1] = 0.0f;
	m_ProjectionMatrix[2] = 0.0f;
	m_ProjectionMatrix[3] = 0.0f;

	m_ProjectionMatrix[4] = 0.0f;
	m_ProjectionMatrix[5] = y;
	m_ProjectionMatrix[6] = 0.0f;
	m_ProjectionMatrix[7] = 0.0f;

	m_ProjectionMatrix[8] = 0.0f;
	m_ProjectionMatrix[9] = 0.0f;
	m_ProjectionMatrix[10] = z;
	m_ProjectionMatrix[11] = 1.0f;

	m_ProjectionMatrix[12] = 0.0f;
	m_ProjectionMatrix[13] = 0.0f;
	m_ProjectionMatrix[14] = zt;
	m_ProjectionMatrix[15] = 0.0f;

	//m_projectionmatrix = { x, 0, 0, 0,
	//					   0, y, 0, 0,
	//					   0, 0, z, 1,
	//					   0, 0, zt, 0 };

}

void CCamera::UpdateOrtographicProjectionMatrix(float left, float right, float bottom, float top, float nearp, float farp)
{
	float x = 2.0f / ((right - left) / 100);
	float y = 2.0f / ((bottom - top) / 100);
	float z = 1.0f / (farp - nearp);

	float zz = -z * nearp;

	m_ProjectionMatrix[0] = x;
	m_ProjectionMatrix[1] = 0.0f;
	m_ProjectionMatrix[2] = 0.0f;
	m_ProjectionMatrix[3] = 0.0f;

	m_ProjectionMatrix[4] = 0.0f;
	m_ProjectionMatrix[5] = y;
	m_ProjectionMatrix[6] = 0.0f;
	m_ProjectionMatrix[7] = 0.0f;

	m_ProjectionMatrix[8] = 0.0f;
	m_ProjectionMatrix[9] = 0.0f;
	m_ProjectionMatrix[10] = z;
	m_ProjectionMatrix[11] = 0.0f;

	m_ProjectionMatrix[12] = 0.0f;
	m_ProjectionMatrix[13] = 0.0f;
	m_ProjectionMatrix[14] = zz;
	m_ProjectionMatrix[15] = 1.0f;
	//m_projectionmatrix = { x, 0.0f, 0.0f, 0.0f,
	//					   0.0f, y, 0.0f, 0.0f,
	//					   0.0f, 0.0f, z, 0.0f,
	//					   0.0f, 0.0f, zz, 1.0f };

}

void CCamera::MoveCamera(CVector3& mov)
{
	m_Eye.SetValues(m_Eye.GetX() + mov.GetX(), m_Eye.GetY() + mov.GetY(), m_Eye.GetZ() + mov.GetZ());
	m_At.SetValues(m_At.GetX() + mov.GetX(), m_At.GetY() + mov.GetY(), m_At.GetZ() + mov.GetZ());
}

void CCamera::RotateCamera(CVector3& mov)
{
	mov.SetValues(mov.GetX() / 1000.0f, mov.GetY() / 1000.0f, mov.GetZ() / 1000.0f);
	m_At.SetValues(m_At.GetX() + mov.GetX(), m_At.GetY() + mov.GetY(), m_At.GetZ());
}