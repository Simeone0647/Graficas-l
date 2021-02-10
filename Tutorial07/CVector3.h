#pragma once
#include <math.h>

class CVector3
{
public:

	CVector3();
	CVector3(float x, float y, float z);
	~CVector3();

	/*
		* @Function Name: GetX
		* @brief...This function will return the vector X value.
		* @bug.....No known bugs.
		* @return #float: Vector X value.
	*/
	inline float GetX() { return m_PosX; }

	/*
		* @Function Name: GetY
		* @brief...This function will return the vector Y value.
		* @bug.....No known bugs.
		* @return #float: Vector X value.
	*/
	inline float GetY() { return m_PosY; }

	/*
		* @Function Name: GetZ
		* @brief...This function will return the vector Z value.
		* @bug.....No known bugs.
		* @return #float: Vector X value.
	*/
	inline float GetZ() { return m_PosZ; }

	inline float GetA() { return m_A; }

	/*
		* @Function Name: Normalize
		* @brief...This function will normalize a vector.
		* @param...#CVector3&: The vector to normalize.
		* @bug.....No known bugs.
		* @return..#CVector3: The normalized vector.
	*/
	static CVector3 Normalize(CVector3& v);

	/*
		* @Function Name: CrossProduct
		* @brief...This function will realize the cross product of two vectors.
		* @param...#CVector3&: Left value of the cross product.
		* @param...#CVector3&: Right value of the cross product.
		* @bug.....No known bugs.
		* @return..#CVector3: The cross product of first param and second param.
	*/
	static CVector3 CrossProduct(CVector3& v1, CVector3& v2);

	/*
		* @Function Name: DotProduct
		* @brief...This function will realize the dot product of two vectors.
		* @param...#CVector3&: Left value of the dot product.
		* @param...#CVector3&: Right value of the dot product.
		* @bug.....No known bugs.
		* @return..#float: The dot product (scalar) of param one and param two.
	*/
	static float DotProduct(CVector3& v1, CVector3& v2);

	/*
		* @Function Name: GetValues
		* @brief...This function will return the vector.
		* @bug.....No known bugs.
		* @return #CVector3: The created vector.
	*/
	inline CVector3 GetValues() { return CVector3(GetX(), GetY(), GetZ()); }

	/*
		* @Function Name: SetValues
		* @brief...This function will set the vector values.
		* @param...#float: Vector X new value.
		* @param...#float: Vector Y new value.
		* @param...#float: Vector Z new value.
		* @bug.....No known bugs.
	*/
	inline void SetValues(float x, float y, float z) { m_PosX = x; m_PosY = y; m_PosZ = z; }

	CVector3 operator-(const CVector3& v)
	{
		CVector3 vec;
		vec.SetValues(this->GetX() - v.m_PosX, this->GetY() - v.m_PosY, this->GetZ() - v.m_PosZ);
		return vec;
	}

private:

	/*
		* @Variable Name: m_PosX.
		* @Type: #float.
		* @brief...Will represent the vector X component.
	*/
	float m_PosX;

	/*
		* @Variable Name: m_PosY.
		* @Type: #float.
		* @brief...Will represent the vector Y component.
	*/
	float m_PosY;

	/*
		* @Variable Name: m_PosZ.
		* @Type: #float.
		* @brief...Will represent the vector Z component.
	*/
	float m_PosZ;

	float m_A;

};

