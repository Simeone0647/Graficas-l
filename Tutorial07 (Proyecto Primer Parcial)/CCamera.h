#pragma once
#include "CVector3.h"
#include <windows.h>
#include <iostream>

using namespace std;

class CCamera
{
public:

	CCamera();
	~CCamera();

	inline void setEye(float eye_x, float eye_y, float eye_z, float eye_a) { m_eye.setValues(eye_x, eye_y, eye_z); }
	inline CVector3 getEye() { return m_eye; }

	inline void setAt(float at_x, float at_y, float at_z, float at_a) { m_at.setValues(at_x, at_y, at_z); }
	inline CVector3 getAt() { return m_at; }

	inline void setUp(float up_x, float up_y, float up_z, float up_a) { m_up.setValues(up_x, up_y, up_z); }
	inline CVector3 getUp() { return m_up; }

	inline float* getViewMatrix() { return m_viewmatrix; }
	void UpdateViewMatrix();

	float* getPerspectiveProjectionMatrix();
	void UpdatePerspectiveProjectionMatrix(float angle_y, float ratio, float nearp, float farp);

	float* getOrtographicProjectionMatrix();
	void UpdateOrtographicProjectionMatrix(float left, float right, float bottom, float top, float nearp, float farp);

	void MoveCamera(CVector3 mov);
	void RotateCamera(CVector3 mov);

	inline void setMouseInitPos(int x, int y) {m_mousepos[0] = x; m_mousepos[1] = y;}
	inline int getMousePosX() { return m_mousepos[0]; }
	inline int getMousePosY() { return m_mousepos[1]; }
private:

	//This three vectors conform the View Matrix.
	//Camera position in the world defined by the scene's origin.
	CVector3 m_eye;

	//Focus point, the point the camera is viewing.
	CVector3 m_at;

	//Denote the up direction RELATIVE to the camera (can be the world's up), cross product between At and Eye.
	CVector3 m_up;
	
	float* m_viewmatrix;
	float* m_translationmatrix;
	float* m_projectionmatrix;

	int m_mousepos[2];
};

