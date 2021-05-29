#include "Model.h"

Model::Model() : m_MeshesNum{ 0 }, m_Name{ "" }
{
}

Model::~Model()
{

}

void Model::SetUpModel(HWND _hwnd)
{
	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vModelVBs.push_back(VertexBuffer());
		m_vModelIBs.push_back(IndexBuffer());
		m_vMeshes[i].SetUpMesh(m_vModelVBs[i], m_vModelIBs[i], _hwnd);
	}
}

void Model::UpdateTranslationMatrix(const float _x, const float _y, const float _z)
{
	m_TraslationMatrix.m_Matrix[3][0] = _x;
	m_TraslationMatrix.m_Matrix[3][1] = _y;
	m_TraslationMatrix.m_Matrix[3][2] = _z;
}

void Model::UpdateRotationMatrix(const float _x, const float _y, const float _z)
{
#if defined(OGL)

#endif
	Matrix RotationX, RotationY, RotationZ, Result;

	const float X_Radian = _x * (M_PI / 180.0f);
	const float Y_Radian = _y * (M_PI / 180.0f);
	const float Z_Radian = _z * (M_PI / 180.0f);

	const int LeftHand = -1;

	if (_x != 0)
	{
		RotationX.m_Matrix[1][1] = RotationX.m_Matrix[2][2] = cos(X_Radian);
		RotationX.m_Matrix[2][1] = sin(X_Radian) * LeftHand;
		RotationX.m_Matrix[1][2] = -sin(X_Radian) * LeftHand;
		Result *= RotationX;
	}
	if (_y != 0)
	{
		RotationY.m_Matrix[0][0] = RotationY.m_Matrix[2][2] = cos(Y_Radian);
		RotationY.m_Matrix[2][0] = -sin(Y_Radian) * LeftHand;
		RotationY.m_Matrix[0][2] = sin(Y_Radian) * LeftHand;
		Result *= RotationY;
	}
	if (_z != 0)
	{
		RotationZ.m_Matrix[0][0] = RotationZ.m_Matrix[1][1] = cos(Z_Radian);
		RotationZ.m_Matrix[1][0] = sin(Z_Radian) * LeftHand;
		RotationZ.m_Matrix[0][1] = -sin(Z_Radian) * LeftHand;
		Result *= RotationZ;
	}

	m_RotationMatrix = Result;
}

void Model::UpdateScaleMatrix(const float _x, const float _y, const float _z)
{
	m_ScaleMatrix.m_Matrix[0][0] = _x;
	m_ScaleMatrix.m_Matrix[1][1] = _y;
	m_ScaleMatrix.m_Matrix[2][2] = _z;
}

void Model::Update()
{
	Matrix SR;

	SR = m_ScaleMatrix * m_RotationMatrix;
	m_ModelMatrix = SR * m_TraslationMatrix;
	#if defined(OGL)
	Matrix MV;
	Matrix MVP;
	MV = m_ModelMatrix * _obj.m_Camera->m_ViewMatrix;
	MVP = MV * _obj.m_Camera->m_ProjectionMatrix;
	for (int i = 0; i < m_MeshesNum; i++)
	{
		m_vMeshes[i].SetModelMatrix(m_ModelMatrix);
		m_vMeshes[i].SetMVPMatrix(MVP);
	}
	#endif
	#if defined(DX11)
	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].SetModelMatrix(m_ModelMatrix);
	}
	#endif

	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].Update();
	}
}

void Model::Render(HWND _hwnd)
{
	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].Render(m_vModelVBs[i], m_vModelIBs[i], _hwnd);
	}
}

#if defined(DX11)

bool Model::GetPassID(const int _PassID)
{
	for (unsigned int i = 0; i < m_vPassID.size(); ++i)
	{
		if (m_vPassID[i] == _PassID)
		{
			return true;
		}
	}
	return false;
}

void Model::CleanUpDXResources()
{
	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].CleanUpDXResources();
	}
}
#endif
