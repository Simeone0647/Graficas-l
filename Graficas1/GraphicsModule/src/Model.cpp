#include "Model.h"

Model::Model() : m_MeshNum{ 0 }, m_Name{ "" }
{
	//Matrix::SetIdentity(m_ModelMatrix);
	//Matrix::SetIdentity(m_TraslationMatrix);
	//Matrix::SetIdentity(m_RotationMatrix);
	//Matrix::SetIdentity(m_ScaleMatrix);
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
		m_Meshes[i].SetModelMatrix(m_ModelMatrix);
		//Matrix::Copy(m_ModelMatrix, m_Meshes[i].GetModelMatrix());
	}
}

void Model::UpdateTranslationMatrix(const float _x, const float _y, const float _z)
{
	//m_TraslationMatrix[12] = _x;
	//m_TraslationMatrix[13] = _y;
	//m_TraslationMatrix[14] = _z;
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
	//m_ScaleMatrix[0] = _x;
	//m_ScaleMatrix[5] = _y;
	//m_ScaleMatrix[10] = _z;
	m_ScaleMatrix.m_Matrix[0][0] = _x;
	m_ScaleMatrix.m_Matrix[1][1] = _y;
	m_ScaleMatrix.m_Matrix[2][2] = _z;
}

void Model::Update(GraphicsModule::test& _obj, HWND _hwnd)
{
	Matrix RT;
	Matrix MV;
	Matrix MVP;

	//Matrix::Multiplication(m_ScaleMatrix, m_RotationMatrix, RT);
	//Matrix::Multiplication(RT, m_TraslationMatrix, m_ModelMatrix);
	RT = m_ScaleMatrix * m_RotationMatrix;
	m_ModelMatrix = RT * m_TraslationMatrix;
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
		m_Meshes[i].SetModelMatrix(m_ModelMatrix);
		//Matrix::Copy(m_ModelMatrix, m_Meshes[i].GetModelMatrix());
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

//void Model::SetQuaternionValues(Vector3 _Axis, float _Angle)
//{
//	m_Quaternion.SetValues(_Axis, _Angle);
//}

#if defined(DX11)

void Model::CleanUpDXResources()
{
	for (int i = 0; i < m_MeshNum; i++)
	{
		m_Meshes[i].CleanUpDXResources();
	}
}
#endif
