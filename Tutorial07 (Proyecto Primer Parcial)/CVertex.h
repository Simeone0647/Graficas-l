#pragma once
class CVertex
{
public:
	CVertex();
	~CVertex();

	inline void setPosition(float x, float y, float z) { m_Position[0] = x; m_Position[1] = y; m_Position[2] = z; }

	inline void setTextura(float x, float y) { m_Texture[0] = x; m_Texture[1] = y; }

	inline int getSize() { return sizeof(m_Position) + sizeof(m_Texture);}
private:
	
	float m_Position[3];
	float m_Texture[2];
};

