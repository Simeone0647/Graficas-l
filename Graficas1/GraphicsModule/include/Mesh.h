#pragma once
#include <stdio.h>
#include <string.h>
#include "Vertex.h"

class Mesh
{
public:

	Mesh();
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
	inline Vertex* GetVertex() { return m_Vertex; }

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
	inline unsigned int* GetVertexIndex() {return m_VertexIndex;}

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
	inline void SetWorldMatrixValue(int index, float value) { m_WorldMatrix[index] = value; }

	/*
		* @Function Name: GetWorldMatrix
		* @brief...This function will return the mesh world matrix.
		* @bug.....No known bugs.
		* @return..#float_pointer: Pointer to an sixteen elements array.
	*/
	inline float* GetWorldMatrix() { return m_WorldMatrix; }

private:

	/*
		* @Variable Name: m_Vertex.
		* @Type: #CVertex_pointer.
		* @brief.Mesh vertex array.
	*/
	Vertex* m_Vertex;

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
	unsigned int* m_VertexIndex;

	/*
		* @Variable Name: m_NumOfVertexIndex.
		* @Type: #int.
		* @brief.Number of vertex index.
	*/
	int m_NumOfVertexIndex;

	/*
		* @Variable Name: m_WorldMatrix.
		* @Type: #float_poiner.
		* @brief.Pointer to an array of 16 floats, represent the mesh world matrix.
	*/
	float* m_WorldMatrix;
};

