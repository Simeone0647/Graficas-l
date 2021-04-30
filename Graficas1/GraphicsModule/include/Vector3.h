#pragma once
#include <math.h>

class Vector3
{
public:

	Vector3();
	Vector3(float _X, float _Y, float _Z);
	~Vector3();

	/*
		* @Function Name: Normalize
		* @brief...This function will normalize a vector.
		* @param...#CVector3&: The vector to normalize.
		* @bug.....No known bugs.
		* @return..#CVector3: The normalized vector.
	*/
	static Vector3 Normalize(Vector3& _Vector);

	/*
		* @Function Name: CrossProduct
		* @brief...This function will realize the cross product of two vectors.
		* @param...#CVector3&: Left value of the cross product.
		* @param...#CVector3&: Right value of the cross product.
		* @bug.....No known bugs.
		* @return..#CVector3: The cross product of first param and second param.
	*/
	static Vector3 CrossProduct(Vector3& _V1, Vector3& _V2);

	/*
		* @Function Name: DotProduct
		* @brief...This function will realize the dot product of two vectors.
		* @param...#CVector3&: Left value of the dot product.
		* @param...#CVector3&: Right value of the dot product.
		* @bug.....No known bugs.
		* @return..#float: The dot product (scalar) of param one and param two.
	*/
	static float DotProduct(Vector3& _V1, Vector3& _V2);

	Vector3 operator-(const Vector3& _Vector);

//private:

	/*
		* @Variable Name: m_X.
		* @Type: #float.
		* @brief...Will represent the vector X component.
	*/
	float m_X;

	/*
		* @Variable Name: m_Y.
		* @Type: #float.
		* @brief...Will represent the vector Y component.
	*/
	float m_Y;

	/*
		* @Variable Name: m_Z.
		* @Type: #float.
		* @brief...Will represent the vector Z component.
	*/
	float m_Z;

	float m_A;

};

