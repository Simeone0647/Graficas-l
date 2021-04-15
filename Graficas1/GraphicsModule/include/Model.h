#pragma once
#include <vector>
#include "Mesh.h"
#include "Quaternion.h"
#define _USE_MATH_DEFINES
#include <math.h>
struct RotationMatrixStruct
{
	float x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
};

class Model
{
public:
	Model();
	~Model();

	inline void AddMesh(Mesh _Mesh) { m_Meshes.push_back(_Mesh); }

	void SetUpModel(HWND _hwnd);

	inline void SetMeshNum(const unsigned int _Num) { m_MeshNum = _Num; }

	inline void SetName(std::string _Name) { m_Name = _Name; }

	inline std::string GetName() { return m_Name; }

	inline float* GetModelMatrix() { return m_ModelMatrix; }

	inline float* GetTranslationMatrix() { return m_TraslationMatrix; }

	inline float* GetScaleMatrix() { return m_ScaleMatrix; }

	inline float* GetRotationMatrix() { return m_RotationMatrix; }

	inline float* GetGuiPos() { return m_GuiPos; }

	inline float* GetGuiRot() { return m_GuiRot; }

	inline float* GetGuiScale() { return m_GuiScale; }

	void UpdateModelMatrix();

	void UpdateTranslationMatrix(const float _x, const float _y, const float _z);

	void UpdateRotationMatrix(const float _x, const float _y, const float _z);

	void UpdateScaleMatrix(const float _x, const float _y, const float _z);

	void Update(GraphicsModule::test& _obj, HWND _hwnd);

	void Render(HWND _hwnd);

	std::vector<Mesh> GetMeshes() { return m_Meshes; }

	void SetQuaternionValues(Vector3 _Axis, float _Angle);
	#if defined(DX11)
	void CleanUpDXResources();
	#endif
private:

	std::vector<VertexBuffer> m_vModelVB;

	std::vector<IndexBuffer> m_vModelIB;

	std::vector<Mesh> m_Meshes;

	int m_MeshNum;

	/*
		* @Variable Name: m_ModelMatrix.
		* @Type: #float_poiner.
		* @brief.Pointer to an array of 16 floats, represent the model matrix.
	*/
	float m_ModelMatrix[16];

	float m_TraslationMatrix[16];

	float m_RotationMatrix[16];

	float m_ScaleMatrix[16];

	Quaternion m_Quaternion;

	float m_GuiPos[3]{};
	float m_GuiRot[3]{ 0.0f, 0.0f, 0.0f};
	float m_GuiScale[3]{ 1.0f, 1.0f, 1.0f };

	std::string m_Name;
};

