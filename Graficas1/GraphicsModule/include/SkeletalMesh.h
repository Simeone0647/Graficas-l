#pragma once
#include "Bone.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <algorithm>
#include <vector>

using std::vector;
using std::reverse;

class SkeletalMesh
{
public:
	SkeletalMesh();
	~SkeletalMesh();

	void Init(vector<Bone> _Bones);

	inline unsigned int GetBonesNum() { return m_BonesNum; }

	inline string GetBoneName(const unsigned int _i) { return m_vBones[_i].GetName(); }

	void SetBoneFinalTransform(const unsigned int _ID, const Matrix _FinalTransform);

	void SetLineFinalTransform(const unsigned int _ID, const Matrix _FinalTransform);

	inline Matrix GetBoneFinalTransform(const unsigned int _i) { return m_vBones[_i].GetFinalTransform(); }

	inline Matrix GetLineFinalTransform(const unsigned int _i) { return m_vBones[_i].GetLineFinalTransform(); }

	Matrix GetBoneOffsetMatrix(const unsigned int _ID);

	void AddBone(const Matrix _Offset, const string _Name, const unsigned int _ID);

	unsigned int GetBoneIndex(const string _Name);

	bool SearchBone(const string _Name);

	inline void AddVertex() { m_vVertex.push_back(Vertex()); }

	inline void AddVertexIndex(const unsigned int _Index) { m_vVertexIndex.push_back(_Index); }

	inline void SetVertexWeight(const float _Weight, const int _ID) { m_vVertex.back().SetBoneWeight(_Weight, _ID); }

	inline unsigned int GetVertexNum() { return m_vVertex.size(); }



	inline Vertex* GetVertexData() { return m_vVertex.data(); }

	inline unsigned int* GetVertexIndexData() { return m_vVertexIndex.data(); }

	inline unsigned int GetVertexIndexNum() { return m_vVertexIndex.size(); }

	inline void ReverseVertexIndex() { reverse(m_vVertexIndex.begin(), m_vVertexIndex.end()); }

	inline VertexBuffer GetVB() { return m_VB; }

	inline IndexBuffer GetIB() { return m_IB; }

	vector<Vertex> m_vVertex;

	vector<unsigned int> m_vVertexIndex;

	#if defined(DX11)
	ConstantBuffer m_CB;
	#endif

	VertexBuffer m_VB;

	IndexBuffer m_IB;

private:
	vector<Bone> m_vBones;

	unsigned int m_BonesNum;

	map<string, unsigned int> m_BoneMapping;


};

