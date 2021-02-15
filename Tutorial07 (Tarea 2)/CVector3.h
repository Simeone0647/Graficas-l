#pragma once
#include <math.h>

class CVector3
{
public:

	CVector3();
	CVector3(float x, float y, float z);
	~CVector3();

	inline float getX() { return m_posx; }
	inline float getY() { return m_posy; }
	inline float getZ() { return m_posz; }
	inline float getA() { return m_a; }

	static CVector3 Normalize(CVector3& v);
	static CVector3 CrossProduct(CVector3& v1, CVector3& v2);
	static float DotProduct(CVector3& v1, CVector3& v2);


	CVector3 getValues();
	inline void setValues(float x, float y, float z) { m_posx = x; m_posy = y; m_posz = z; }

	CVector3 operator-(const CVector3& v)
	{
		CVector3 vec;
		vec.setValues(this->getX() - v.m_posx, this->getY() - v.m_posy, this->getZ() - v.m_posz);
		return vec;
	}

private:

	float m_posx, m_posy, m_posz, m_a;

};

