#pragma once
#include "Vector3.h"
#include "Matrix.h"
#include <windows.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

namespace GraphicsModule
{
	struct UpdateProjectionMatrixStruct;
}

class Camera
{
public:

	Camera();
	~Camera();

	/*	
		* @Function Name: SetEye
		* @brief...This function will set the camera's Eye values.
		* @param...#float: Eye's X value.
		* @param...#float: Eye's Y value.
		* @param...#float: Eye's Z value.
		* @bug.....No known bugs.
	*/
	inline void SetEye(float _X, float _Y, float _Z) { m_Eye.m_X = _X; m_Eye.m_Y = _Y; m_Eye.m_Z = _Z;; }

	/*
		* @Function Name: GetEye
		* @brief...This function will return the camera's Eye.
		* @bug.....No known bugs.
		* @return..#CVector3: The camera's Eye vector.
	*/
	inline Vector3 GetEye() { return m_Eye; }

	inline Vector3* GetEyeAddress() { return &m_Eye; }

	/*
		* @Function Name: SetAt
		* @brief...This function will set the camera's At values.
		* @param...#float: Eye's X value.
		* @param...#float: Eye's Y value.
		* @param...#float: Eye's Z value.
		* @bug.....No known bugs.
	*/
	inline void SetAt(float _X, float _Y, float _Z) { m_At.m_X = _X; m_At.m_Y = _Y; m_At.m_Z = _Z; }

	/*
		* @Function Name: GetAt
		* @brief...This function will return the camera's At.
		* @bug.....No known bugs.
		* @return..#CVector3: The camera's At vector.
	*/
	inline Vector3 GetAt() { return m_At; }

	/*
		* @Function Name: SetUp
		* @brief...This function will set the camera's Up values.
		* @param...#float: Eye's X value.
		* @param...#float: Eye's Y value.
		* @param...#float: Eye's Z value.
		* @bug.....No known bugs.
	*/
	inline void SetUp(float _X, float _Y, float _Z) { m_Up.m_X = _X; m_Up.m_Y = _Y; m_Up.m_Z = _Z; }

	/*
		* @Function Name: GetUp
		* @brief...This function will return the camera's Up.
		* @bug.....No known bugs.
		* @return..#CVector3: The camera's Up vector.
	*/
	inline Vector3 GetUp() { return m_Up; }

	inline Vector3* GetFrontAddress() { return &m_Front; }

	/*
		* @Function Name: GetViewMatrix
		* @brief...This function will return the camera's view matrix.
		* @bug.....No known bugs.
		* @return..#float_pointer: A pointer to an array of sixteen floats.  
	*/
	//inline float* GetViewMatrix() { return m_ViewMatrix; }

	/*
		* @Function Name: UpdateViewMatrix
		* @brief...This function will update the view matrix with new values.
		* @bug.....No known bugs.
	*/
	void UpdateViewMatrix();

	/*
		* @Function Name: GetPerspectiveProjectionMatrix
		* @brief...This function will return the camera's perspective projection matrix.
		* @bug.....No known bugs.
		* @return..#float_pointer: A pointer to an array of sixteen floats.
	*/
	//inline float* GetPerspectiveProjectionMatrix() { return m_ProjectionMatrix; }

	/*
		* @Function Name: UpdatePerspectiveProjectionMatrix
		* @brief...This function will update the perspective projection matrix with new values.
		* @param...#float: Camera's view opening angle.
		* @param...#float: Screen ratio (width / height).
		* @param...#float: Camera's near plane.
		* @param...#float: Camera's far plane.
		* @bug.....No known bugs.
	*/
	void UpdatePerspectiveProjectionMatrix(GraphicsModule::UpdateProjectionMatrixStruct _PMStruct);


	/*
		* @Function Name: UpdateOrtographicProjectionMatrix
		* @brief...This function will update the ortographic projection matrix with new values.
		* @param...#float: Screen width.
		* @param...#float: Screen height.
		* @param...#float: Camera's near plane.
		* @param...#float: Camera's far plane.
		* @bug.....No known bugs.
	*/
	void UpdateOrtographicProjectionMatrix(GraphicsModule::UpdateProjectionMatrixStruct _PMStruct);

	/*
		* @Function Name: MoveCamera
		* @brief...This function will sum (or rest) the camera's eye and at with given values.
		* @param...#CVector3&: The vector that will be sumed or rested to the camera's eye and at.
		* @bug.....No known bugs.
	*/
	void MoveCamera(Vector3& mov);

	/*
		* @Function Name: RotateCamera
		* @brief...This function will sum (or rest) the camera's at with given values.
		* @param...#CVector3&: The vector that will be sumed or rested to the camera's at.
		* @bug.....No known bugs.
	*/
	void RotateCamera(const float _x, const float _y, const float _z);
//private:

	/*	
		* @Variable Name: m_Eye
		* @Type....#CVector3
		* @brief...This variable will represent the camera's Eye, that's it, the camera's position.
	*/
	Vector3 m_Eye;

	/*
		* @Variable Name: m_At
		* @Type....#CVector3
		* @brief...This variable will represent the camera's At, that's it, the point that the camera is viewing.
	*/
	Vector3 m_At;

	/*
		* @Variable Name: m_Up
		* @Type....#CVector3
		* @brief...This variable will represent the camera's Up, that's it, the up direction RELATIVE (or not) to the camera.
		*		   Cross product of At and Eye.
	*/
	Vector3 m_Up;
	
	Vector3 m_Front;

	/*
		* @Variable Name: m_ViewMatrix
		* @Type....#float_pointer
		* @brief...This variable will represent the camera's view matrix. It will point to an array of sixteen floats.
	*/
	float* m_ViewMatrix;

	/*
		* @Variable Name: m_TranslationMatrix
		* @Type....#float_pointer
		* @brief...This variable will represent the camera's translation matrix. It will point to an array of sixteen floats.
	*/
	float* m_TranslationMatrix;

	/*
		* @Variable Name: m_ProjectionMatrix
		* @Type....#float_pointer
		* @brief...This variable will represent the camera's projection matrix. It will point to an array of sixteen floats.
	*/
	float* m_ProjectionMatrix;
};

