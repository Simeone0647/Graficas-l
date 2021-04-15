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
	float mWorld[16];
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
		* @Function Name: SetVertex
		* @brief...This function will set the vertex array.
		* @param...#CVertex_Pointer: A pointer to the CVertex passed array first element.
		* @param...#int: Passed array size.
		* @bug.....No known bugs.
	*/
	void SetVertex(Vertex* figure, int numofvertex);

	/*
		* @Function Name: GetVertex
		* @brief...This function will return a pointer to the vertex array.
		* @bug.....No known bugs.
		* @return #CVertex_Pointer: Pointer to the vertex array.
	*/
	inline std::vector<Vertex> GetVertex() { return m_Vertex; }

	/*
		* @Function Name: SetNumOfVertex
		* @brief...This function will set the number of vertex that the passed array contains, in other words, the size.
		* @param...#int: Size of the passed array.
		* @bug.....No known bugs.
	*/
	inline void SetNumOfVertex(int numofvertex) { m_NumOfVertex = numofvertex; }

	/*
		* @Function Name: GetNumOfVertex
		* @brief...This function will return the vertex array size.
		* @bug.....No known bugs.
		* @return #int: Vertex array size.
	*/
	inline int GetNumOfVertex() { return m_NumOfVertex; }

	/*
		* @Function Name: SetVertexIndex
		* @brief...This function will set the vertex index array.
		* @param...#unsigned_short_pointer: A pointer to the unsigned short passed array first element.
		* @param...#int: Passed array size.
		* @bug.....No known bugs.
	*/
	void SetVertexIndex(unsigned int* vertexindexarr, int numofvertexindex);

	/*
		* @Function Name: GetVertexIndex
		* @brief...This function will return a pointer to the vertex index array.
		* @bug.....No known bugs.
		* @return #unsigned_short_pointer: Pointer to the vertex index array.
	*/
	inline std::vector<unsigned int> GetVertexIndex() {return m_VertexIndex;}

	/*
		* @Function Name: SetNumOfVertexIndex
		* @brief...This function will set the number of vertex index.
		* @param...#int: Number of vertex index (array size).
		* @bug.....No known bugs.
	*/
	inline void SetNumOfVertexIndex(int numvertexindex) { m_NumOfVertexIndex = numvertexindex; }

	/*
		* @Function Name: GetNumOfVertexIndex
		* @brief...This function will return the vertex index array size.
		* @param...#unsigned_short_pointer: A pointer to the unsigned short passed array first element.
		* @param...#int: Passed array size.
		* @bug.....No known bugs.
		* @return..#int: Num of vertex index elements.
	*/
	inline int GetNumOfVertexIndex() { return m_NumOfVertexIndex; }

	/*
		* @Function Name: GetArraySize
		* @brief...This function will return the size in bytes of the vertex array.
		* @bug.....No known bugs.
		* @return..#int: Vertex array size in bytes.
	*/
	inline int GetArraySize() { return sizeof(m_Vertex) * m_NumOfVertex; }

	/*
		* @Function Name: GetVertexIndexSize
		* @brief...This function will return the size in bytes of the vertex index array.
		* @bug.....No known bugs.
		* @return..#int: Vertex index array size in bytes.
	*/
	inline int GetVertexIndexSize() { return sizeof(unsigned int) * m_NumOfVertexIndex; }

	/*
		* @Function Name: SetWorldMatrixValue
		* @brief...This function will set a value in a single slot of the world matrix array.
		* @param...#int: The index of the array where the value will be inserted.
		* @param...#float: Value that will be inserted.
		* @bug.....No known bugs.
	*/
	inline void SetWorldMatrixValue(int index, float value) { m_ModelMatrix[index] = value; }

	/*
		* @Function Name: GetWorldMatrix
		* @brief...This function will return the mesh world matrix.
		* @bug.....No known bugs.
		* @return..#float_pointer: Pointer to an sixteen elements array.
	*/
	inline float* GetModelMatrix() { return m_ModelMatrix; }

	inline float* GetTranslationMatrix() { return m_TraslationMatrix; }

	inline float* GetScaleMatrix() { return m_ScaleMatrix; }

	inline float* GetRotationMatrix() { return m_RotationMatrix; }

	inline float* GetMVPMatrix() { return m_MVP; }

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
	float m_ModelMatrix[16];

	float m_TraslationMatrix[16];

	float m_RotationMatrix[16];

	float m_ScaleMatrix[16];

	float m_MVP[16];

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

