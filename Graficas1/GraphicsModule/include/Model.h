#pragma once
#include <vector>
#include "Mesh.h"
#include "Quaternion.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Model
{
public:
	Model();
	~Model();

	inline void AddMesh(Mesh _Mesh) { m_vMeshes.push_back(_Mesh); }

	void SetUpModel(HWND _hwnd);

	inline void SetMeshNum(const unsigned int _Num) { m_MeshesNum = _Num; }

	inline unsigned int GetMeshNum() { return m_MeshesNum; }

	inline void SetName(std::string _Name) { m_Name = _Name; }

	inline std::string GetName() { return m_Name; }

	inline Matrix GetModelMatrix() { return m_ModelMatrix; }

	inline Matrix GetTranslationMatrix() { return m_TraslationMatrix; }

	inline Matrix GetScaleMatrix() { return m_ScaleMatrix; }

	inline Matrix GetRotationMatrix() { return m_RotationMatrix; }

	inline float* GetGuiPos() { return m_GuiPos; }

	inline float* GetGuiRot() { return m_GuiRot; }

	inline float* GetGuiScale() { return m_GuiScale; }

	void UpdateTranslationMatrix(const float _x, const float _y, const float _z);

	void UpdateRotationMatrix(const float _x, const float _y, const float _z);

	void UpdateScaleMatrix(const float _x, const float _y, const float _z);

	void Update();

	void Render(HWND _hwnd);

	std::vector<Mesh> GetMeshes() { return m_vMeshes; }

	#if defined(DX11)
	void CleanUpDXResources();
	#endif

	bool m_ImGuiSelected = false;

	float m_GuiPos[3]{};
	float m_GuiRot[3]{ 0.0f, 0.0f, 0.0f };
	float m_GuiScale[3]{ 1.0f, 1.0f, 1.0f };
private:

	std::vector<VertexBuffer> m_vModelVBs;

	std::vector<IndexBuffer> m_vModelIBs;

	std::vector<Mesh> m_vMeshes;

	int m_MeshesNum;

	/*
		* @Variable Name: m_ModelMatrix.
		* @Type: #float_poiner.
		* @brief.Pointer to an array of 16 floats, represent the model matrix.
	*/
	//float m_ModelMatrix[16];
	Matrix m_ModelMatrix;

	Matrix m_TraslationMatrix;

	Matrix m_RotationMatrix;

	Matrix m_ScaleMatrix;

	std::string m_Name;
};

