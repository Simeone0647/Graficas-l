#include "CVertex.h"

CVertex::CVertex()
{
	for (int i = 0; i < 3; i++)
	{
		m_Position[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{
		m_Texture[i] = 0;
	}
}

CVertex::~CVertex()
{
	//if (m_PositionPointer != nullptr)
	//{
	//	delete[] m_PositionPointer;
	//	m_PositionPointer = nullptr;
	//}
	//
	//if (m_TexturePointer != nullptr)
	//{
	//	delete[] m_TexturePointer;
	//	m_TexturePointer = nullptr;
	//}
}

