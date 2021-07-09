#pragma once
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Mesh.h"
#include <map>

using std::map;


class Model
{
public:
	Model();
	~Model();
	//Model(Model&&) {}
	//Model(Model&);

	inline void AddMesh(Mesh _Mesh) { m_vMeshes.push_back(_Mesh); }

	void SetUpModel();

	inline void SetMeshNum(const unsigned int _Num) { m_MeshesNum = _Num; }

	inline unsigned int GetMeshNum() { return m_MeshesNum; }

	inline void SetName(string _Name) { m_Name = _Name; }

	inline string GetName() { return m_Name; }

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

	void Update(const float _Time);

	void Render(HWND _hwnd);

	void Load(const string _Filename);

	Mesh LoadMesh(aiMesh* _CurrentMesh, const unsigned int _i);

	vector<Mesh> GetMeshes() { return m_vMeshes; }

	inline void SetPassID(int _ID) { m_vPassID.push_back(_ID); }
	bool GetPassID(const int _PassID);

	void ReadNodeHeirarchy(const float _AnimationTime, const aiScene* _pScene, const aiNode* _pNode, Matrix& _ParentTransform);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* _pAnimation, const string _NodeName);

	void CalcInterpolatedScaling(aiVector3D& _Out, const float _AnimationTime, const aiNodeAnim* _pNodeAnim);

	void CalcInterpolatedRotation(aiQuaternion& _Out, const float _AnimationTime, const aiNodeAnim* _pNodeAnim);

	void CalcInterpolatedPosition(aiVector3D& _Out, const float _AnimationTime, const aiNodeAnim* _pNodeAnim);

	unsigned int FindScaling(const float _AnimationTime, const aiNodeAnim* _pNodeAnim);

	unsigned int FindRotation(const float _AnimationTime, const aiNodeAnim* _pNodeAnim);

	unsigned int FindPosition(const float _AnimationTime, const aiNodeAnim* pNodeAnim);
	#if defined(DX11)
	void CleanUpDXResources();
	#endif

	bool m_ImGuiSelected = false;

	float m_GuiPos[3]{};
	float m_GuiRot[3]{ 0.0f, 0.0f, 0.0f };
	float m_GuiScale[3]{ 1.0f, 1.0f, 1.0f };
private:

	vector<VertexBuffer> m_vModelVBs;

	vector<IndexBuffer> m_vModelIBs;

	vector<Mesh> m_vMeshes;

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

	Matrix m_GlobalInverseTransform;

	string m_Name;

	vector<int> m_vPassID;

	Assimp::Importer m_Importer;

	const aiScene* m_pScene = new aiScene;

	unsigned int m_NumBones;

	SkeletalMesh m_SkeletalMesh;

	#if defined(OGL)
	bool m_HasAnim;
	#endif

	#if defined(DX11)
	int m_HasAnimDX[4];

	Buffer m_BBonesTransforms;

	Buffer m_BHasAnim;
	#endif

	vector<Matrix> m_BonesTransforms;

	unsigned int m_BoneLocation[100];
};

