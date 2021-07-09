#pragma once
#include "Bone.h"
#include "Vertex.h"

#include <vector>

using std::vector;

class SkeletalMesh
{
public:
	SkeletalMesh();
	~SkeletalMesh();

	void Init(vector<Bone> _Bones);

	inline unsigned int GetBonesNum() { return m_BonesNum; }

	inline string GetBoneName(const unsigned int _i) { return m_vBones[_i].GetName(); }

	void SetBoneFinalTransform(const unsigned int _ID, const Matrix _FinalTransform);

	inline Matrix GetBoneFinalTransform(const unsigned int _i) { return m_vBones[_i].GetFinalTransform(); }

	Matrix GetBoneOffsetMatrix(const unsigned int _ID);

	void AddBone(const Matrix _Offset, const string _Name, const unsigned int _ID);

	unsigned int GetBoneIndex(const string _Name);

	bool SearchBone(const string _Name);

	inline void AddVertex() { m_vVertex.push_back(Vertex()); }
private:
	vector<Bone> m_vBones;

	unsigned int m_BonesNum;

	map<string, unsigned int> m_BoneMapping;

	vector<Vertex> m_vVertex;

	//VertexBuffer m_VB;
	//IndexBuffer m_IB;
};

