#include "Vertex.h"

Vertex::Vertex()
{
	for (int i = 0; i < 3; i++)
	{
		m_Position[i] = 0;
		m_Normal[i] = 0;
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

