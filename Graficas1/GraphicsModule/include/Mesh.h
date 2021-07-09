#pragma once
#include <stdio.h>
#include <string.h>
#include "Vertex.h"
#include "Material.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "SamplerState.h"
#include "SkeletalMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>

using std::vector;

enum LoadTypes
{
	LoadTriangles = 0,
	LoadPoints,
	LoadRGBA,
	LoadBGRA
};

#if defined(DX11)
struct CBChangesEveryFrame
{
	Matrix mWorld;
	float vMeshColor[4];
};
#endif

class Mesh
{
public:

	Mesh(vector<Vertex> _Vertex, vector<unsigned int> _Indices, vector<std::string> _TexturesNames, const int _Flags[], string _Name,
		 vector<Bone> _Bones, const Matrix _GlobalInverseTransform);
	~Mesh();

	/*
		* @Function Name: GetVertex
		* @brief...This function will return a pointer to the vertex array.
		* @bug.....No known bugs.
		* @return #CVertex_Pointer: Pointer to the vertex array.
	*/
	inline std::vector<Vertex> GetVertex() { return m_vVertex; }

	/*
		* @Function Name: GetVertexIndex
		* @brief...This function will return a pointer to the vertex index array.
		* @bug.....No known bugs.
		* @return #unsigned_short_pointer: Pointer to the vertex index array.
	*/
	inline std::vector<unsigned int> GetVertexIndex() {return m_vVertexIndex;}

	void Update(const aiMesh* _CurrentMesh, const aiScene* _Scene, const float _Time);

	void Render(VertexBuffer& _VB, IndexBuffer& _IB, HWND _Hwnd);

	void SetUpMesh(VertexBuffer& _VB, IndexBuffer& _IB);

	std::string GetName() { return m_Name; }

	void SetModelMatrix(const Matrix _Matrix);

	#if defined(DX11) || defined(OGL)
	void LoadTexture();

	Material* GetMaterial() { return m_Material; }
	#endif

	#if defined(DX11)
	void CleanUpDXResources();
	#endif

	#if defined(OGL)
	inline unsigned int GetTexID(const unsigned int _i) { return m_vTexID[_i]; }

	inline float* GetMVPMatrix() { return m_MVP; }

	inline void SetMVPMatrix(const Matrix _Matrix) { Matrix::MatrixTo1D(_Matrix, m_MVP); }
	#endif
private:

	/*
		* @Variable Name: m_Vertex.
		* @Type: #CVertex_pointer.
		* @brief.Mesh vertex array.
	*/
	std::vector<Vertex> m_vVertex;

	/*
		* @Variable Name: m_NumOfVertex.
		* @Type: #int.
		* @brief.Indicate the number of vertex that the mesh have.
	*/
	int m_VertexNum;

	/*
		* @Variable Name: m_VertexIndex.
		* @Type: #unsigned_short_pointer.
		* @brief.Mesh vertex index array.
	*/
	std::vector<unsigned int> m_vVertexIndex;

	/*
		* @Variable Name: m_NumOfVertexIndex.
		* @Type: #int.
		* @brief.Number of vertex index.
	*/
	int m_VertexIndexNum;

	std::vector<std::string> m_vTexturesNames;

	std::string m_Name;

	Material* m_Material;

	bool m_LoadTypes[4] = {false, false, false, false};

	SkeletalMesh m_SkeletalMesh;

	//vector<Matrix> m_Transforms;

	Matrix m_GlobalInverseTransform;

	bool m_HasAnim;

	unsigned int m_BoneLocation[100];
	#if defined(DX11)
	/*
		* @Variable Name: m_ModelMatrix.
		* @Type: #Matrix.
		* @brief.4x4 2D Array. Represent the model matrix.
	*/
	Matrix m_ModelMatrix;

	CBChangesEveryFrame m_CBChangesEveryFrame;

	ConstantBuffer m_MeshCB;

	float m_MeshColor[3];

	SamplerState m_Samplers;
	#endif

	#if defined(OGL)
	/*
		* @Variable Name: m_ModelMatrix.
		* @Type: #Matrix.
		* @brief.4x4 2D Array. Represent the model matrix.
	*/
	float* m_ModelMatrix;

	float* m_MVP;

	unsigned int m_ShaderID;

	vector<unsigned int> m_vTexID;
	#endif
	//topologia 
};

