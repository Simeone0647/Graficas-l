#include "SkeletalMesh.h"

SkeletalMesh::SkeletalMesh()
{
	m_BonesNum = 0;
}

SkeletalMesh::~SkeletalMesh()
{
}

void SkeletalMesh::Init(vector<Bone> _Bones)
{
	m_vBones = _Bones;

	m_BonesNum = m_vBones.size();
}

void SkeletalMesh::SetBoneFinalTransform(const unsigned int _ID, const Matrix _FinalTransform)
{
	for (unsigned int i = 0; i < m_BonesNum; ++i)
	{
		if (m_vBones[i].GetID() == _ID)
		{
			m_vBones[i].SetFinalTransform(_FinalTransform);
		}
	}
}

void SkeletalMesh::SetLineFinalTransform(const unsigned int _ID, const Matrix _FinalTransform)
{
	for (unsigned int i = 0; i < m_BonesNum; ++i)
	{
		if (m_vBones[i].GetID() == _ID)
		{
			m_vBones[i].SetFinalLineTransform(_FinalTransform);
		}
	}
}

Matrix SkeletalMesh::GetBoneOffsetMatrix(const unsigned int _ID)
{
	for (unsigned int i = 0; i < m_BonesNum; ++i)
	{
		if (m_vBones[i].GetID() == _ID)
		{
			return m_vBones[i].GetOffsetMatrix();
		}
	}

	return Matrix();
}

void SkeletalMesh::AddBone(const Matrix _Offset, const string _Name, const unsigned int _ID)
{
	m_vBones.push_back(Bone(_Offset, _Name, _ID));

	m_BonesNum++;

	m_BoneMapping[_Name] = _ID;
}

unsigned int SkeletalMesh::GetBoneIndex(const string _Name)
{
	if (m_BoneMapping.find(_Name) != m_BoneMapping.end())
	{
		return m_BoneMapping[_Name];
	}
	else
	{
		return 0;
	}
}

bool SkeletalMesh::SearchBone(const string _Name)
{
	if (m_BoneMapping.find(_Name) == m_BoneMapping.end())
	{
		return false;
	}

	return true;
}
