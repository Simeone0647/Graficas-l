#include "Bone.h"

Bone::Bone(const Matrix _Offset, const string _Name, const unsigned int _ID)
{
	m_Offset = _Offset;
	m_ID = _ID;
	m_Name = _Name;
}

Bone::~Bone()
{
}
