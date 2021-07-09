#pragma once
#include "Matrix.h"
#include <string>
#include <map>

using std::string;
using std::map;

class Bone
{
public:
	 Bone(const Matrix _Offset, const string _Name, const unsigned int _ID);
	~Bone();

	inline string GetName() { return m_Name; }

	inline unsigned int GetID() { return m_ID; }

	inline void SetFinalTransform(Matrix _FinalTransform) { m_FinalTransform = _FinalTransform; }

	inline Matrix GetFinalTransform() { return m_FinalTransform; }

	inline Matrix GetOffsetMatrix() { return m_Offset; }
private:
	string m_Name;
	unsigned int m_ID;
	Matrix m_Offset;
	Matrix m_FinalTransform;
};

