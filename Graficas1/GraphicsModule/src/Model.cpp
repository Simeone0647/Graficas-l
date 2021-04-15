#include "Model.h"

Model::Model() : m_MeshNum{ 0 }, m_Name{ "" }
{
	Matrix::SetIdentity(m_ModelMatrix);
	Matrix::SetIdentity(m_TraslationMatrix);
	Matrix::SetIdentity(m_RotationMatrix);
	Matrix::SetIdentity(m_ScaleMatrix);

}

Model::~Model()
{

}

void Model::SetUpModel(HWND _hwnd)
{
	for (int i = 0; i < m_MeshNum; i++)
	{
		m_vModelVB.push_back(VertexBuffer());
		m_vModelIB.push_back(IndexBuffer());
		m_Meshes[i].SetUpMesh(m_vModelVB[i], m_vModelIB[i], _hwnd);
	}
}

void Model::UpdateModelMatrix()
{
	for (int i = 0; i < m_MeshNum; i++)
	{
		Matrix::Copy(m_ModelMatrix, m_Meshes[i].GetModelMatrix());
	}
}

void Model::UpdateTranslationMatrix(const float _x, const float _y, const float _z)
{
	m_TraslationMatrix[12] = _x;
	m_TraslationMatrix[13] = _y;
	m_TraslationMatrix[14] = _z;
}

void Model::UpdateRotationMatrix(const float _x, const float _y, const float _z)
{
#if defined(OGL)

#endif
	float RotationX[16];
	float RotationY[16];
	float RotationZ[16];
	float Result[16];
	Matrix::SetIdentity(RotationX);
	Matrix::SetIdentity(RotationY);
	Matrix::SetIdentity(RotationZ);
	Matrix::SetIdentity(Result);

	const float X_Radian = _x * (M_PI / 180.0f);
	const float Y_Radian = _y * (M_PI / 180.0f);
	const float Z_Radian = _z * (M_PI / 180.0f);

	const int LeftHand = -1;

	if (_x != 0)
	{
		RotationX[5] = RotationX[10] = cos(X_Radian);
		RotationX[9] = sin(X_Radian) * LeftHand;
		RotationX[6] = -sin(X_Radian) * LeftHand;
		float AuxX[16];
		Matrix::Multiplication(Result, RotationX, AuxX);
		Matrix::Copy(AuxX, Result);
	}
	if (_y != 0)
	{
		RotationY[0] = RotationY[10] = cos(Y_Radian);
		RotationY[8] = -sin(Y_Radian) * LeftHand;
		RotationY[2] = sin(Y_Radian) * LeftHand;
		float AuxY[16];
		Matrix::Multiplication(Result, RotationY, AuxY);
		Matrix::Copy(AuxY, Result);
	}
	if (_z != 0)
	{
		RotationZ[0] = RotationZ[5] = cos(Z_Radian);
		RotationZ[4] = sin(Z_Radian) * LeftHand;
		RotationZ[1] = -sin(Z_Radian) * LeftHand;
		float AuxZ[16];
		Matrix::Multiplication(Result, RotationZ, AuxZ);
		Matrix::Copy(AuxZ, Result);
	}
	Matrix::Copy(Result, m_RotationMatrix);
}

void Model::UpdateScaleMatrix(const float _x, const float _y, const float _z)
{
	m_ScaleMatrix[0] = _x;
	m_ScaleMatrix[5] = _y;
	m_ScaleMatrix[10] = _z;
}

void Model::Update(GraphicsModule::test& _obj, HWND _hwnd)
{
	float RT[16];
	float MV[16];
	float MVP[16];

	Matrix::Multiplication(m_ScaleMatrix, m_RotationMatrix, RT);
	Matrix::Multiplication(RT, m_TraslationMatrix, m_ModelMatrix);
	#if defined(OGL)
	Matrix::Multiplication(m_ModelMatrix, _obj.m_Camera->GetViewMatrix(), MV);
	Matrix::Multiplication(MV, _obj.m_Camera->GetPerspectiveProjectionMatrix(), MVP);
	for (int i = 0; i < m_MeshNum; i++)
	{
		Matrix::Copy(MVP, m_Meshes[i].GetMVPMatrix());
	}
	#endif
	#if defined(DX11)
	for (int i = 0; i < m_MeshNum; i++)
	{
		Matrix::Copy(m_ModelMatrix, m_Meshes[i].GetModelMatrix());
	}
	#endif

	for (int i = 0; i < m_MeshNum; i++)
	{
		m_Meshes[i].Update();
	}
}

void Model::Render(HWND _hwnd)
{
	for (int i = 0; i < m_MeshNum; i++)
	{
		m_Meshes[i].Render(m_vModelVB[i], m_vModelIB[i], _hwnd);
	}
}

void Model::SetQuaternionValues(Vector3 _Axis, float _Angle)
{
	m_Quaternion.SetValues(_Axis, _Angle);
}

#if defined(DX11)

void Model::CleanUpDXResources()
{
	for (int i = 0; i < m_MeshNum; i++)
	{
		m_Meshes[i].CleanUpDXResources();
	}
}
#endif
