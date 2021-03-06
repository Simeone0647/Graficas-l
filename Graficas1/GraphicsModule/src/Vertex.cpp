#include "Vertex.h"

Vertex::Vertex()
{
	//for (int i = 0; i < 4; i++)
	//{
	//	m_Position[i] = 0;
	//	m_Normal[i] = 0;
	//}
	m_Position[0] = 0.0f;
	m_Position[1] = 0.0f;
	m_Position[2] = 0.0f;
	m_Position[3] = 1.0f;

	for (int i = 0; i < 4; i++)
	{
		m_Normal[i] = 0.0f;
		m_Tangents[i] = 0.0f;
		m_Binormal[i] = 0.0f;
		m_BoneIndex[i] = 0;
		m_BoneWeight[i] = 0.0f;
	}
	for (int i = 0; i < 2; i++)
	{
		m_Texture[i] = 0;
	}
}

Vertex::Vertex(float posx, float posy, float posz, float normalx, float normaly, float normalz, float texturex, float texturey)
{
	m_Position[0] = posx;
	m_Position[1] = posy;
	m_Position[2] = posz;

	m_Normal[0] = normalx;
	m_Normal[1] = normaly;
	m_Normal[2] = normalz;

	m_Texture[0] = texturex;
	m_Texture[1] = texturey;
}

Vertex::~Vertex()
{

}

bool Vertex::SetBoneIndex(const int _ID)
{
	//for (unsigned int i = 0; i < 4; ++i)
	//{
	//	if (m_BoneIndex[i] < -1)
	//	{
	//		m_BoneIndex[i] = _ID;
	//
	//		return true;
	//	}
	//}

	return false;
}

bool Vertex::SetBoneWeight(const float _Weight, const int _ID)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (m_BoneWeight[i] == 0.0f)
		{
			m_BoneWeight[i] = _Weight;
			m_BoneIndex[i] = _ID;
			return true;
		}
	}

	return false;
}

void Vertex::CheckWeights()
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (m_BoneWeight[i] < 0.0f)
		{
			m_BoneWeight[i] = 0.0f;
		}
	}
}

void Vertex::CheckIDs()
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (m_BoneIndex[i] < -1)
		{
			m_BoneIndex[i] = -1;
		}
	}
}

