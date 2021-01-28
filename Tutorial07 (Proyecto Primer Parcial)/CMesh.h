#pragma once
#include <stdio.h>
#include <string.h>
#include "CVertex.h"

class CMesh
{
public:

	CMesh();
	~CMesh();

	void setVertex(CVertex* figura, int numofvertex);
	inline CVertex* getVertex() { return m_Vertex; }

	inline void setNumOfVertex(int numofvertex) { m_NumOfVertex = numofvertex; }
	inline int getNumOfVertex() { return m_NumOfVertex; }

	void setVertexIndex(unsigned short* vertexindexarr, int numofvertexindex);
	inline unsigned short* getVertexIndex() {return m_VertexIndex;}

	inline void setNumOfVertexIndex(int numvertexindex) { m_NumOfVertexIndex = numvertexindex; }
	inline int getNumOfVertexIndex() { return m_NumOfVertexIndex; }

	inline int getArraySize() { return sizeof(m_Vertex) * m_NumOfVertex; }
	inline int getVertexIndexSize() { return sizeof(unsigned short) * m_NumOfVertexIndex; }

	inline void setWorldMatrixValue(int index, float value) { m_WorldMatrix[index] = value; }
	inline float* getWorldMatrix() { return m_WorldMatrix; }
private:

	CVertex* m_Vertex;
	int m_NumOfVertex;

	unsigned short* m_VertexIndex;
	int m_NumOfVertexIndex;

	float* m_WorldMatrix;
};

