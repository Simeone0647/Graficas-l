#pragma once
#include "Camera.h"
#include "test.h"

Camera::Camera()
{
	m_ProjectionMatrix = new float[16];
	m_TranslationMatrix = new float[16];
	m_ViewMatrix = new float[16];
}

Camera::~Camera()
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

void Camera::UpdateViewMatrix()
{
	Vector3 A = m_At - m_Eye;
	Vector3 Z_Axis = Vector3::Normalize(A);//Front

	m_At.m_X = m_Eye.m_X + Z_Axis.m_X;
	m_At.m_Y = m_Eye.m_Y + Z_Axis.m_Y;
	m_At.m_Z = m_Eye.m_Z + Z_Axis.m_Z;

	Vector3 B = (Vector3::CrossProduct(m_Up, Z_Axis));

	Vector3 X_Axis = Vector3::Normalize(B);//right
	Vector3 Y_Axis = Vector3::CrossProduct(Z_Axis, X_Axis);//Up

	//m_ViewMatrix.m_Matrix[0][0] = X_Axis.m_X;
	//m_ViewMatrix.m_Matrix[0][1] = Y_Axis.m_X;
	//m_ViewMatrix.m_Matrix[0][2] = Z_Axis.m_X;
	//m_ViewMatrix.m_Matrix[0][3] = 0.0f;
	//
	//m_ViewMatrix.m_Matrix[1][0] = X_Axis.m_Y;
	//m_ViewMatrix.m_Matrix[1][1] = Y_Axis.m_Y;
	//m_ViewMatrix.m_Matrix[1][2] = Z_Axis.m_Y;
	//m_ViewMatrix.m_Matrix[1][3] = 0.0f;
	//
	//m_ViewMatrix.m_Matrix[2][0] = X_Axis.m_Z;
	//m_ViewMatrix.m_Matrix[2][1] = Y_Axis.m_Z;
	//m_ViewMatrix.m_Matrix[2][2] = Z_Axis.m_Z;
	//m_ViewMatrix.m_Matrix[2][3] = 0.0f;
	//
	//m_ViewMatrix.m_Matrix[3][0] = -Vector3::DotProduct(X_Axis, m_Eye);
	//m_ViewMatrix.m_Matrix[3][1] = -Vector3::DotProduct(Y_Axis, m_Eye);
	//m_ViewMatrix.m_Matrix[3][2] = -Vector3::DotProduct(Z_Axis, m_Eye);
	//m_ViewMatrix.m_Matrix[3][3] = 1.0f;

	m_ViewMatrix[0] = X_Axis.m_X;
	m_ViewMatrix[1] = Y_Axis.m_X;
	m_ViewMatrix[2] = Z_Axis.m_X;
	m_ViewMatrix[3] = 0.0f;
	
	m_ViewMatrix[4] = X_Axis.m_Y;
	m_ViewMatrix[5] = Y_Axis.m_Y;
	m_ViewMatrix[6] = Z_Axis.m_Y;
	m_ViewMatrix[7] = 0.0f;
	
	m_ViewMatrix[8] = X_Axis.m_Z;
	m_ViewMatrix[9] = Y_Axis.m_Z;
	m_ViewMatrix[10] = Z_Axis.m_Z;
	m_ViewMatrix[11] = 0.0f;
	
	m_ViewMatrix[12] = -Vector3::DotProduct(X_Axis, m_Eye);
	m_ViewMatrix[13] = -Vector3::DotProduct(Y_Axis, m_Eye);
	m_ViewMatrix[14] = -Vector3::DotProduct(Z_Axis, m_Eye);
	m_ViewMatrix[15] = 1.0f;

	//Matrix::Multiplication(m_RotationMatrix, m_TranslationMatrix, m_ViewMatrix);
	//m_viewmatrix = { xaxis.getX(), yaxis.getX(), zaxis.getX(), 0.0f,
	//				 xaxis.getY(), yaxis.getY(), zaxis.getY(), 0.0f,
	//				 xaxis.getZ(), yaxis.getZ(), zaxis.getZ(), 0.0f,
	//				 -CVector3::DotProduct(xaxis, m_eye), -CVector3::DotProduct(yaxis, m_eye), -CVector3::DotProduct(zaxis, m_eye),  1.0f };
}

void Camera::UpdatePerspectiveProjectionMatrix(GraphicsModule::UpdateProjectionMatrixStruct _PMStruct)
{
	float HalfAngle = _PMStruct.AngleY / 2;
	float SinHalfAngle = sin(HalfAngle);
	float CosHalfAngle = cos(HalfAngle);
	float Y = CosHalfAngle / SinHalfAngle;
	float X = Y / _PMStruct.Ratio;
	float Z = _PMStruct.FarPlane / (_PMStruct.FarPlane - _PMStruct.NearPlane);
	float zt = (-_PMStruct.FarPlane * _PMStruct.NearPlane) / (_PMStruct.FarPlane - _PMStruct.NearPlane);

	//m_ProjectionMatrix.m_Matrix[0][0] = X;
	//m_ProjectionMatrix.m_Matrix[0][1] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[0][2] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[0][3] = 0.0f;
	//
	//m_ProjectionMatrix.m_Matrix[1][0] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[1][1] = Y;
	//m_ProjectionMatrix.m_Matrix[1][2] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[1][3] = 0.0f;
	//
	//m_ProjectionMatrix.m_Matrix[2][0] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[2][1] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[2][2] = Z;
	//m_ProjectionMatrix.m_Matrix[2][3] = 0.0f;
	//
	//m_ProjectionMatrix.m_Matrix[3][0] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[3][1] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[3][2] = zt;
	//m_ProjectionMatrix.m_Matrix[3][3] = 0.0f;

	m_ProjectionMatrix[0] = X;
	m_ProjectionMatrix[1] = 0.0f;
	m_ProjectionMatrix[2] = 0.0f;
	m_ProjectionMatrix[3] = 0.0f;
	
	m_ProjectionMatrix[4] = 0.0f;
	m_ProjectionMatrix[5] = Y;
	m_ProjectionMatrix[6] = 0.0f;
	m_ProjectionMatrix[7] = 0.0f;
	
	m_ProjectionMatrix[8] = 0.0f;
	m_ProjectionMatrix[9] = 0.0f;
	m_ProjectionMatrix[10] = Z;
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

void Camera::UpdateOrtographicProjectionMatrix(GraphicsModule::UpdateProjectionMatrixStruct _PMStruct)
{
	float X = 2.0f / (_PMStruct.Width / 100);
	float Y = 2.0f / (_PMStruct.Height / 100);
	float Z = 1.0f / (_PMStruct.FarPlane - _PMStruct.NearPlane);

	float zz = -Z * _PMStruct.NearPlane;

	//m_ProjectionMatrix.m_Matrix[0][0] = X;
	//m_ProjectionMatrix.m_Matrix[0][1] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[0][2] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[0][3] = 0.0f;
	//
	//m_ProjectionMatrix.m_Matrix[1][0] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[1][1] = Y;
	//m_ProjectionMatrix.m_Matrix[1][2] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[1][3] = 0.0f;
	//
	//m_ProjectionMatrix.m_Matrix[2][0] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[2][1] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[2][2] = Z;
	//m_ProjectionMatrix.m_Matrix[2][3] = 0.0f;
	//
	//m_ProjectionMatrix.m_Matrix[3][0] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[3][1] = 0.0f;
	//m_ProjectionMatrix.m_Matrix[3][2] = zz;
	//m_ProjectionMatrix.m_Matrix[3][3] = 1.0f;

	m_ProjectionMatrix[0] = X;
	m_ProjectionMatrix[1] = 0.0f;
	m_ProjectionMatrix[2] = 0.0f;
	m_ProjectionMatrix[3] = 0.0f;
	
	m_ProjectionMatrix[4] = 0.0f;
	m_ProjectionMatrix[5] = Y;
	m_ProjectionMatrix[6] = 0.0f;
	m_ProjectionMatrix[7] = 0.0f;
	
	m_ProjectionMatrix[8] = 0.0f;
	m_ProjectionMatrix[9] = 0.0f;
	m_ProjectionMatrix[10] = Z;
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

void Camera::MoveCamera(Vector3& _Mov)
{
	m_Eye.m_X += _Mov.m_X;
	m_Eye.m_Y += _Mov.m_Y;
	m_Eye.m_Z += _Mov.m_Z;

	m_At.m_X += _Mov.m_X;
	m_At.m_Y += _Mov.m_Y;
	m_At.m_Z += _Mov.m_Z;
}

void Camera::RotateCamera(const float _X, const float _Y, const float _Z)
{
	if (_X != 0)
	{
		m_Eye.m_X = m_Eye.m_X + _X;
	}
	if (_Y != 0)
	{
		m_Eye.m_Y = m_Eye.m_Y + _Y;
	}
	if (_Z != 0)
	{
		m_Eye.m_Z = m_Eye.m_Z + _Z;
	}
}