#pragma once
#include <stdio.h>
#include <string.h>
#include "Vertex.h"
#include "Material.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
struct CBChangesEveryFrame
{
	Matrix mWorld;
	float vMeshColor[4];
};

class Mesh
{
public:

	Mesh();
	Mesh(std::vector<Vertex> _MeshVertex, std::vector<unsigned int> _MeshIndices, std::vector<std::string> _Filename, unsigned int _VertexNum, bool _LoadRGBA, bool _LoadBGRA,
		bool _LoadTriangles, bool _LoadPoints, std::string _Name, unsigned int _ShaderID);
	~Mesh();

	/*
		* @Function Name: GetVertex
		* @brief...This function will return a pointer to the vertex array.
		* @bug.....No known bugs.
		* @return #CVertex_Pointer: Pointer to the vertex array.
	*/
	inline std::vector<Vertex> GetVertex() { return m_Vertex; }

	/*
		* @Function Name: GetVertexIndex
		* @brief...This function will return a pointer to the vertex index array.
		* @bug.....No known bugs.
		* @return #unsigned_short_pointer: Pointer to the vertex index array.
	*/
	inline std::vector<unsigned int> GetVertexIndex() {return m_VertexIndex;}

	/*
		* @Function Name: GetWorldMatrix
		* @brief...This function will return the mesh world matrix.
		* @bug.....No known bugs.
		* @return..#float_pointer: Pointer to an sixteen elements array.
	*/
	inline Matrix GetModelMatrix() { return m_ModelMatrix; }
	 
	inline Matrix GetTranslationMatrix() { return m_TraslationMatrix; }
	
	inline Matrix GetScaleMatrix() { return m_ScaleMatrix; }
	
	inline Matrix GetRotationMatrix() { return m_RotationMatrix; }
	
	inline Matrix GetMVPMatrix() { return m_MVP; }

	inline void SetModelMatrix(const Matrix _ModelMatrix) { m_ModelMatrix = _ModelMatrix; }

	void Update();

	void Render(VertexBuffer& _VB, IndexBuffer& _IB, HWND _hwnd);

	void SetUpMesh(VertexBuffer& _VB, IndexBuffer& _IB, const HWND _hwnd);

	#if defined(DX11) || defined(OGL)
	void LoadTexture(HWND _hwnd);
	#endif
	inline unsigned int GetTexId() { return m_TexID; }

	inline Texture2D GetDXTexture() { return EntryTexture; };

	#if defined(DX11)
	void CleanUpDXResources();
	#endif
	std::string GetName() { return m_Name; }

	Material* GetMaterial() { return m_Material; }
private:

	/*
		* @Variable Name: m_Vertex.
		* @Type: #CVertex_pointer.
		* @brief.Mesh vertex array.
	*/
	std::vector<Vertex> m_Vertex;

	/*
		* @Variable Name: m_NumOfVertex.
		* @Type: #int.
		* @brief.Indicate the number of vertex that the mesh have.
	*/
	int m_NumOfVertex;

	/*
		* @Variable Name: m_VertexIndex.
		* @Type: #unsigned_short_pointer.
		* @brief.Mesh vertex index array.
	*/
	std::vector<unsigned int> m_VertexIndex;

	/*
		* @Variable Name: m_NumOfVertexIndex.
		* @Type: #int.
		* @brief.Number of vertex index.
	*/
	int m_NumOfVertexIndex;

	/*
		* @Variable Name: m_ModelMatrix.
		* @Type: #float_poiner.
		* @brief.Pointer to an array of 16 floats, represent the model matrix.
	*/
	Matrix m_ModelMatrix;

	Matrix m_TraslationMatrix;

	Matrix m_RotationMatrix;

	Matrix m_ScaleMatrix;

	Matrix m_MVP;

	CBChangesEveryFrame m_cb;

	ConstantBuffer	m_MeshCB;

	Material* m_Material;

	float m_MeshColor[3];

	unsigned int m_VertexBuffer;

	unsigned int m_IndexBuffer;

	unsigned int m_VAO;

	unsigned int m_VBO;

	unsigned int m_EBO;
	#if defined(DX11) || defined(OGL)
	FIBITMAP* m_dib1;
	#endif
	bool m_ShowTexture;

	Texture2D EntryTexture;

	std::vector<std::string> m_Filename;

	std::string m_Name;

	unsigned int m_TexID = 0;

	bool m_LoadAsRGBA;

	bool m_LoadAsBGRA;

	bool m_LoadAsPoints;
	
	bool m_LoadAsTriangles;

	unsigned int m_ShaderID;
	//topologia 
};

