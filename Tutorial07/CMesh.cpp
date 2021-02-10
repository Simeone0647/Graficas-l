#include "CMesh.h"

CMesh::CMesh() : m_Vertex{ nullptr }, m_NumOfVertex{ 0 }, m_VertexIndex{ nullptr }, m_NumOfVertexIndex{ 0 }
{
	m_WorldMatrix = new float[16];

	m_WorldMatrix[0] = 1.0f;
	m_WorldMatrix[1] = 0.0f;
	m_WorldMatrix[2] = 0.0f;
	m_WorldMatrix[3] = 0.0f;

	m_WorldMatrix[4] = 0.0f;
	m_WorldMatrix[5] = 1.0f;
	m_WorldMatrix[6] = 0.0f;
	m_WorldMatrix[7] = 0.0f;

	m_WorldMatrix[8] = 0.0f;
	m_WorldMatrix[9] = 0.0f;
	m_WorldMatrix[10] = 1.0f;
	m_WorldMatrix[11] = 0.0f;

	m_WorldMatrix[12] = 0.0f;
	m_WorldMatrix[13] = 0.0f;
	m_WorldMatrix[14] = 0.0f;
	m_WorldMatrix[15] = 1.0f;
}

CMesh::~CMesh()
{
	if (m_Vertex != nullptr)
	{
		delete[] m_Vertex;
		m_Vertex = nullptr;
	}

	if (m_VertexIndex != nullptr)
	{
		delete[] m_VertexIndex;
		m_VertexIndex = nullptr;
	}
}

void CMesh::SetVertex(CVertex* figure, int numofvertex)
{
	if (m_Vertex != nullptr)
	{
		delete[] m_Vertex;
	}

	m_Vertex = new CVertex[numofvertex];
	memcpy(m_Vertex, figure, sizeof(figure[0]) * numofvertex);
}

void CMesh::SetVertexIndex(unsigned short* vertexindexarr, int numofvertexindex)
{
	if (m_VertexIndex != nullptr)
	{
		delete[] m_VertexIndex;
	}
	
	m_VertexIndex = new unsigned short[numofvertexindex];
	memcpy(m_VertexIndex, vertexindexarr, sizeof(vertexindexarr[0]) * numofvertexindex);
}