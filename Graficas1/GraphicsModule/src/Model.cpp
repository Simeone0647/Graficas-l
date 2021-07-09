#include "Model.h"
#include "RenderManager.h"
#include "test.h"

Model::Model()
{
	m_NumBones = 0;

	#if defined(OGL)
	m_HasAnim = false;
	#endif

	#if defined(DX11)
	for (auto i = 0; i < 4; ++i)
	{
		m_HasAnimDX[i] = 0;
	}
	#endif
}

Model::~Model()
{

}

void Model::SetUpModel()
{
	HWND hwnd = NULL;

	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vModelVBs.push_back(VertexBuffer());
		m_vModelIBs.push_back(IndexBuffer());

		m_vMeshes[i].SetUpMesh(m_vModelVBs[i], m_vModelIBs[i]);
	}
}

void Model::UpdateTranslationMatrix(const float _x, const float _y, const float _z)
{
	m_TraslationMatrix.m_Matrix[3][0] = _x;
	m_TraslationMatrix.m_Matrix[3][1] = _y;
	m_TraslationMatrix.m_Matrix[3][2] = _z;
}

void Model::UpdateRotationMatrix(const float _x, const float _y, const float _z)
{
#if defined(OGL)

#endif
	Matrix RotationX, RotationY, RotationZ, Result;

	const float X_Radian = _x * (M_PI / 180.0f);
	const float Y_Radian = _y * (M_PI / 180.0f);
	const float Z_Radian = _z * (M_PI / 180.0f);

	const int LeftHand = -1;

	if (_x != 0)
	{
		RotationX.m_Matrix[1][1] = RotationX.m_Matrix[2][2] = cos(X_Radian);
		RotationX.m_Matrix[2][1] = sin(X_Radian) * LeftHand;
		RotationX.m_Matrix[1][2] = -sin(X_Radian) * LeftHand;
		Result *= RotationX;
	}
	if (_y != 0)
	{
		RotationY.m_Matrix[0][0] = RotationY.m_Matrix[2][2] = cos(Y_Radian);
		RotationY.m_Matrix[2][0] = -sin(Y_Radian) * LeftHand;
		RotationY.m_Matrix[0][2] = sin(Y_Radian) * LeftHand;
		Result *= RotationY;
	}
	if (_z != 0)
	{
		RotationZ.m_Matrix[0][0] = RotationZ.m_Matrix[1][1] = cos(Z_Radian);
		RotationZ.m_Matrix[1][0] = sin(Z_Radian) * LeftHand;
		RotationZ.m_Matrix[0][1] = -sin(Z_Radian) * LeftHand;
		Result *= RotationZ;
	}

	m_RotationMatrix = Result;
}

void Model::UpdateScaleMatrix(const float _x, const float _y, const float _z)
{
	m_ScaleMatrix.m_Matrix[0][0] = _x;
	m_ScaleMatrix.m_Matrix[1][1] = _y;
	m_ScaleMatrix.m_Matrix[2][2] = _z;
}

void Model::Update(const float _Time)
{
	Matrix SR;

	SR = m_ScaleMatrix * m_RotationMatrix;
	m_ModelMatrix = SR * m_TraslationMatrix;

	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].SetModelMatrix(m_ModelMatrix);
	}

	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		aiMesh* CurrentMesh = m_pScene->mMeshes[i];
		m_vMeshes[i].Update(CurrentMesh, m_pScene, _Time);
	}

	if (m_pScene->HasAnimations())
	{
		Matrix Identity;

		#if defined(OGL)
		m_HasAnim = true;
		#endif

		#if defined(DX11)
		m_HasAnimDX[0] = 1;
		#endif

		auto TicksPerSecond = (float)m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f;
		auto TimeInTicks = _Time * TicksPerSecond;
		auto AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

		ReadNodeHeirarchy(AnimationTime, m_pScene, m_pScene->mRootNode, Identity);

		m_BonesTransforms.resize(m_SkeletalMesh.GetBonesNum());

		for (auto i = 0; i < m_SkeletalMesh.GetBonesNum(); ++i)
		{
			m_BonesTransforms[i] = m_SkeletalMesh.GetBoneFinalTransform(i);
		}
	}
}

void Model::Render(HWND _hwnd)
{
	#if defined(DX11)
	HWND hwnd = NULL;

	if (m_SkeletalMesh.GetBonesNum() != 0)
	{
		GraphicsModule::UpdateSubResourceStruct UpdateSRStruct;
		UpdateSRStruct.pDstResource = m_BBonesTransforms.BGetBuffer();
		UpdateSRStruct.DstSubresource = 0;
		UpdateSRStruct.pDstBox = NULL;
		UpdateSRStruct.pSrcData = m_BonesTransforms.data();
		UpdateSRStruct.SrcRowPitch = 0;
		UpdateSRStruct.SrcDepthPitch = 0;
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

		UpdateSRStruct.pDstResource = m_BHasAnim.BGetBuffer();
		UpdateSRStruct.DstSubresource = 0;
		UpdateSRStruct.pDstBox = NULL;
		UpdateSRStruct.pSrcData = &m_HasAnimDX;
		UpdateSRStruct.SrcRowPitch = 0;
		UpdateSRStruct.SrcDepthPitch = 0;
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CVSSetConstantBuffers(12, 1, m_BHasAnim.BGetBufferAddress());
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CVSSetConstantBuffers(13, 1, m_BBonesTransforms.BGetBufferAddress());
	}
	#endif

	#if defined(OGL)
	int ShaderID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

	const char* UniformNameHasAnim;
	int UniformHasAnim;
	UniformNameHasAnim = "HasAnim";
	UniformHasAnim = glGetUniformLocation(ShaderID, UniformNameHasAnim);
	if (UniformHasAnim == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameHasAnim);
	}
	glUniform1i(UniformHasAnim, m_HasAnim);

	if (m_SkeletalMesh.GetBonesNum() != 0)
	{
		for (unsigned int i = 0; i < 100; ++i)
		{
			char Name[128];
			memset(Name, 0, sizeof(Name));
			snprintf(Name, sizeof(Name), "Bones[%d]", i);
			m_BoneLocation[i] = glGetUniformLocation(ShaderID, Name);
			if (m_BoneLocation[i] == -1)
			{
				fprintf(stderr, "Could not bind uniform %s\n", Name);
			}
		}

		for (unsigned int i = 0; i < m_BonesTransforms.size(); ++i)
		{
			float* Arr = new float[16];

			Matrix::MatrixTo1D(m_BonesTransforms[i], Arr);
			glUniformMatrix4fv(m_BoneLocation[i], 1, 1, glm::value_ptr(glm::make_mat4(Arr)));

			delete[] Arr;
			Arr = nullptr;
		}
	}
	#endif

	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].Render(m_vModelVBs[i], m_vModelIBs[i], _hwnd);
	}
}

void Model::Load(const string _Filename)
{
	m_pScene = m_Importer.ReadFile(_Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices
													  | aiProcess_CalcTangentSpace);
	
	if (!m_pScene)
	{
		cout << "Error importing the model" << endl;
	}
	else
	{
		//Usar string
		char Drive[_MAX_DRIVE];
		char Directory[_MAX_DIR];
		char Fname[_MAX_FNAME];
		char Ext[_MAX_EXT];

		string Filename = "";

		_splitpath_s(_Filename.c_str(), Drive, _MAX_DRIVE, Directory, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);

		Filename = Fname;
		Filename += Ext;

		for (unsigned int i = 0; i < RM::GetRenderManager().m_vModels.size(); ++i)
		{
			if (RM::GetRenderManager().m_vModels[i]->GetName() == Filename)
			{
				cout << "Model already imported!" << endl;
				
				return;
			}
		}

		m_Name = Filename;
		m_MeshesNum = m_pScene->mNumMeshes;

		aiMatrix4x4 GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
		GlobalInverseTransform.Inverse();

		m_GlobalInverseTransform.m_Matrix[0][0] = GlobalInverseTransform.a1;
		m_GlobalInverseTransform.m_Matrix[0][1] = GlobalInverseTransform.a2;
		m_GlobalInverseTransform.m_Matrix[0][2] = GlobalInverseTransform.a3;
		m_GlobalInverseTransform.m_Matrix[0][3] = GlobalInverseTransform.a4;
		
		m_GlobalInverseTransform.m_Matrix[1][0] = GlobalInverseTransform.b1;
		m_GlobalInverseTransform.m_Matrix[1][1] = GlobalInverseTransform.b2;
		m_GlobalInverseTransform.m_Matrix[1][2] = GlobalInverseTransform.b3;
		m_GlobalInverseTransform.m_Matrix[1][3] = GlobalInverseTransform.b4;
												  
		m_GlobalInverseTransform.m_Matrix[2][0] = GlobalInverseTransform.c1;
		m_GlobalInverseTransform.m_Matrix[2][1] = GlobalInverseTransform.c2;
		m_GlobalInverseTransform.m_Matrix[2][2] = GlobalInverseTransform.c3;
		m_GlobalInverseTransform.m_Matrix[2][3] = GlobalInverseTransform.c4;
												  
		m_GlobalInverseTransform.m_Matrix[3][0] = GlobalInverseTransform.d1;
		m_GlobalInverseTransform.m_Matrix[3][1] = GlobalInverseTransform.d2;
		m_GlobalInverseTransform.m_Matrix[3][2] = GlobalInverseTransform.d3;
		m_GlobalInverseTransform.m_Matrix[3][3] = GlobalInverseTransform.d4;

		for (auto i = 0; i < m_MeshesNum; ++i)
		{
			aiMesh* CurrentMesh = m_pScene->mMeshes[i];

			m_vMeshes.push_back(LoadMesh(CurrentMesh, i));
		}

		#if defined(DX11)
		if (m_SkeletalMesh.GetBonesNum() != 0)
		{ 
			HRESULT hr;
			HWND hwnd = NULL;

			GraphicsModule::UpdateBDStruct BDStruct;
			BDStruct.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
			BDStruct.ByteWidth = sizeof(Matrix) * 100;
			BDStruct.BindFlags = GraphicsModule::SIME_BIND_CONSTANT_BUFFER;
			BDStruct.CPUAccessFlags = 0;
			BDStruct.StructureBytestride = 0;
			BDStruct.MiscFlags = 0;

			m_BBonesTransforms.BUpdateBD(BDStruct);

			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateBuffer(m_BBonesTransforms.BGetBDAddress(), NULL, m_BBonesTransforms.BGetBufferAddress());
			if (FAILED(hr))
			{
				cout << "Failed bones buffer creation." << endl;
			}

			GraphicsModule::UpdateBDStruct BDStruct2;
			BDStruct2.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
			BDStruct2.ByteWidth = sizeof(int) * 4;
			BDStruct2.BindFlags = GraphicsModule::SIME_BIND_CONSTANT_BUFFER;
			BDStruct2.CPUAccessFlags = 0;
			BDStruct2.StructureBytestride = 0;
			BDStruct2.MiscFlags = 0;

			m_BHasAnim.BUpdateBD(BDStruct2);

			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateBuffer(m_BHasAnim.BGetBDAddress(), NULL, m_BHasAnim.BGetBufferAddress());
			if (FAILED(hr))
			{
				cout << "Failed HASANIM buffer creation." << endl;
			}
		}
		#endif

		SetUpModel();
	}
}

Mesh Model::LoadMesh(aiMesh* _CurrentMesh, const unsigned int _i)
{
	vector<Vertex> vMeshVertex;
	vector<unsigned int> vMeshIndices;
	vector<Bone> vBones;
	vector<string> vFilename;

	aiString Path;

	unsigned int NumVertex = _CurrentMesh->mNumVertices;

	for (int i = 0; i < NumVertex; ++i)
	{
		Vertex vertex;

		vertex.SetPosition(_CurrentMesh->mVertices[i].x, _CurrentMesh->mVertices[i].y, _CurrentMesh->mVertices[i].z);

		if (_CurrentMesh->HasNormals())
		{
			vertex.SetNormal(_CurrentMesh->mNormals[i].x, _CurrentMesh->mNormals[i].y, _CurrentMesh->mNormals[i].z);
		}
		if (_CurrentMesh->HasTextureCoords(0))
		{
			#if defined(DX11)
			vertex.SetTexture(_CurrentMesh->mTextureCoords[0][i].x, 1 - _CurrentMesh->mTextureCoords[0][i].y);
			#endif
			#if defined(OGL)
			vertex.SetTexture(_CurrentMesh->mTextureCoords[0][i].x, _CurrentMesh->mTextureCoords[0][i].y);
			#endif
		}
		if (_CurrentMesh->HasTangentsAndBitangents())
		{
			vertex.SetTangent(_CurrentMesh->mTangents[i].x, _CurrentMesh->mTangents[i].y, _CurrentMesh->mTangents[i].z);
			vertex.SetBinormal(_CurrentMesh->mBitangents[i].x, _CurrentMesh->mBitangents[i].y, _CurrentMesh->mBitangents[i].z);
		}

		vMeshVertex.push_back(vertex);
	}

	for (unsigned int i = 0; i < _CurrentMesh->mNumFaces; i++)
	{
		aiFace& Face = _CurrentMesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
		{
			vMeshIndices.push_back(Face.mIndices[j]);
		}
	}

	aiString tmpMatName;
	const aiMaterial* pMaterial = m_pScene->mMaterials[_CurrentMesh->mMaterialIndex];
	pMaterial->Get(AI_MATKEY_NAME, tmpMatName);
	for (unsigned int j = 1; j < aiTextureType_UNKNOWN; j++)
	{
		if (pMaterial->GetTextureCount(aiTextureType(j)) > 0)
		{
			if (pMaterial->GetTexture(aiTextureType(j), 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				char Drive[_MAX_DRIVE];
				char Dir[_MAX_DIR];
				char Fname[_MAX_FNAME];
				char Ext[_MAX_EXT];

				std::string Filename = "";

				_splitpath_s(Path.data, Drive, _MAX_DRIVE, Dir, _MAX_DIR, Fname, _MAX_FNAME, Ext, _MAX_EXT);

				Filename = Fname;
				Filename += Ext;
				vFilename.push_back(Filename);
			}
		}
	}
	
	if (_CurrentMesh->HasBones())
	{
		for (unsigned int i = 0; i < _CurrentMesh->mNumBones; i++)
		{
			unsigned int BoneIndex = 0;

			string BoneName(_CurrentMesh->mBones[i]->mName.data);
			//Si no hay ningún hueso con BoneName de nombre, agregalo.
			if (!m_SkeletalMesh.SearchBone(BoneName)) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;

				Matrix Offset;
				
				Offset.m_Matrix[0][0] = _CurrentMesh->mBones[i]->mOffsetMatrix.a1;
				Offset.m_Matrix[0][1] = _CurrentMesh->mBones[i]->mOffsetMatrix.a2;
				Offset.m_Matrix[0][2] = _CurrentMesh->mBones[i]->mOffsetMatrix.a3;
				Offset.m_Matrix[0][3] = _CurrentMesh->mBones[i]->mOffsetMatrix.a4;
				
				Offset.m_Matrix[1][0] = _CurrentMesh->mBones[i]->mOffsetMatrix.b1;
				Offset.m_Matrix[1][1] = _CurrentMesh->mBones[i]->mOffsetMatrix.b2;
				Offset.m_Matrix[1][2] = _CurrentMesh->mBones[i]->mOffsetMatrix.b3;
				Offset.m_Matrix[1][3] = _CurrentMesh->mBones[i]->mOffsetMatrix.b4;
				
				Offset.m_Matrix[2][0] = _CurrentMesh->mBones[i]->mOffsetMatrix.c1;
				Offset.m_Matrix[2][1] = _CurrentMesh->mBones[i]->mOffsetMatrix.c2;
				Offset.m_Matrix[2][2] = _CurrentMesh->mBones[i]->mOffsetMatrix.c3;
				Offset.m_Matrix[2][3] = _CurrentMesh->mBones[i]->mOffsetMatrix.c4;
				
				Offset.m_Matrix[3][0] = _CurrentMesh->mBones[i]->mOffsetMatrix.d1;
				Offset.m_Matrix[3][1] = _CurrentMesh->mBones[i]->mOffsetMatrix.d2;
				Offset.m_Matrix[3][2] = _CurrentMesh->mBones[i]->mOffsetMatrix.d3;
				Offset.m_Matrix[3][3] = _CurrentMesh->mBones[i]->mOffsetMatrix.d4;

				m_SkeletalMesh.AddBone(Offset, BoneName, BoneIndex);

				
			}
			else 
			{
				BoneIndex = m_SkeletalMesh.GetBoneIndex(BoneName);
			}

			for (unsigned int j = 0; j < _CurrentMesh->mBones[i]->mNumWeights; j++)
			{
				auto ID = _CurrentMesh->mBones[i]->mWeights[j].mVertexId;
				auto Weight = _CurrentMesh->mBones[i]->mWeights[j].mWeight;
				
				vMeshVertex[ID].SetBoneWeight(Weight, BoneIndex);
			}
		}
	}

	int Flags[2] = { 0, 3 };

	return Mesh(vMeshVertex, vMeshIndices, vFilename, Flags, _CurrentMesh->mName.data, vBones, m_GlobalInverseTransform);
}

void Model::ReadNodeHeirarchy(const float _AnimationTime, const aiScene* _pScene, const aiNode* _pNode, Matrix& _ParentTransform)
{
	string NodeName(_pNode->mName.data);

	const aiAnimation* pAnimation = _pScene->mAnimations[0];

	Matrix NodeTransformation;

	NodeTransformation.m_Matrix[0][0] = _pNode->mTransformation.a1;
	NodeTransformation.m_Matrix[0][1] = _pNode->mTransformation.a2;
	NodeTransformation.m_Matrix[0][2] = _pNode->mTransformation.a3;
	NodeTransformation.m_Matrix[0][3] = _pNode->mTransformation.a4;

	NodeTransformation.m_Matrix[1][0] = _pNode->mTransformation.b1;
	NodeTransformation.m_Matrix[1][1] = _pNode->mTransformation.b2;
	NodeTransformation.m_Matrix[1][2] = _pNode->mTransformation.b3;
	NodeTransformation.m_Matrix[1][3] = _pNode->mTransformation.b4;

	NodeTransformation.m_Matrix[2][0] = _pNode->mTransformation.c1;
	NodeTransformation.m_Matrix[2][1] = _pNode->mTransformation.c2;
	NodeTransformation.m_Matrix[2][2] = _pNode->mTransformation.c3;
	NodeTransformation.m_Matrix[2][3] = _pNode->mTransformation.c4;

	NodeTransformation.m_Matrix[3][0] = _pNode->mTransformation.d1;
	NodeTransformation.m_Matrix[3][1] = _pNode->mTransformation.d2;
	NodeTransformation.m_Matrix[3][2] = _pNode->mTransformation.d3;
	NodeTransformation.m_Matrix[3][3] = _pNode->mTransformation.d4;

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, _AnimationTime, pNodeAnim);

		Matrix ScalingMatrix;

		ScalingMatrix.m_Matrix[0][0] = Scaling.x;
		ScalingMatrix.m_Matrix[1][1] = Scaling.y;
		ScalingMatrix.m_Matrix[2][2] = Scaling.z;

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, _AnimationTime, pNodeAnim);
		aiMatrix3x3 aiRotMat = RotationQ.GetMatrix();

		Matrix RotationMatrix;
		RotationMatrix.m_Matrix[0][0] = RotationQ.GetMatrix().a1;
		RotationMatrix.m_Matrix[0][1] = RotationQ.GetMatrix().a2;
		RotationMatrix.m_Matrix[0][2] = RotationQ.GetMatrix().a3;

		RotationMatrix.m_Matrix[1][0] = RotationQ.GetMatrix().b1;
		RotationMatrix.m_Matrix[1][1] = RotationQ.GetMatrix().b2;
		RotationMatrix.m_Matrix[1][2] = RotationQ.GetMatrix().b3;

		RotationMatrix.m_Matrix[2][0] = RotationQ.GetMatrix().c1;
		RotationMatrix.m_Matrix[2][1] = RotationQ.GetMatrix().c2;
		RotationMatrix.m_Matrix[2][2] = RotationQ.GetMatrix().c3;

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;

		CalcInterpolatedPosition(Translation, _AnimationTime, pNodeAnim);

		Matrix TranslationMatrix;

		TranslationMatrix.m_Matrix[0][3] = Translation.x;
		TranslationMatrix.m_Matrix[1][3] = Translation.y;
		TranslationMatrix.m_Matrix[2][3] = Translation.z;

		// Combine the above transformations
		NodeTransformation = TranslationMatrix * RotationMatrix * ScalingMatrix;
	}

	Matrix GlobalTransformation = _ParentTransform * NodeTransformation;

	if (m_SkeletalMesh.SearchBone(NodeName))
	{
		unsigned int BoneIndex = m_SkeletalMesh.GetBoneIndex(NodeName);

		Matrix FinalTransform = m_GlobalInverseTransform * GlobalTransformation * m_SkeletalMesh.GetBoneOffsetMatrix(BoneIndex);

		#if defined(DX11)
		FinalTransform = Matrix::Transpose(FinalTransform);
		#endif

		m_SkeletalMesh.SetBoneFinalTransform(BoneIndex, FinalTransform);
	}

	for (unsigned int i = 0; i < _pNode->mNumChildren; ++i)
	{
		ReadNodeHeirarchy(_AnimationTime, _pScene, _pNode->mChildren[i], GlobalTransformation);
	}
}

const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* _pAnimation, const string _NodeName)
{
	for (unsigned int i = 0; i < _pAnimation->mNumChannels; ++i)
	{
		const aiNodeAnim* pNodeAnim = _pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == _NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

void Model::CalcInterpolatedScaling(aiVector3D& _Out, const float _AnimationTime, const aiNodeAnim* _pNodeAnim)
{
	if (_pNodeAnim->mNumScalingKeys == 1)
	{
		_Out = _pNodeAnim->mScalingKeys[0].mValue;

		return;
	}

	unsigned int ScalingIndex = FindScaling(_AnimationTime, _pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);

	//assert(NextScalingIndex < _pNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(_pNodeAnim->mScalingKeys[NextScalingIndex].mTime - _pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (_AnimationTime - (float)_pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;

	//assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& Start = _pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = _pNodeAnim->mScalingKeys[NextScalingIndex].mValue;

	aiVector3D Delta = End - Start;

	_Out = Start + Factor * Delta;
}

void Model::CalcInterpolatedRotation(aiQuaternion& _Out, const float _AnimationTime, const aiNodeAnim* _pNodeAnim)
{
	// we need at least two values to interpolate...
	if (_pNodeAnim->mNumRotationKeys == 1) {
		_Out = _pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(_AnimationTime, _pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);

	//assert(NextRotationIndex < _pNodeAnim->mNumRotationKeys);

	float DeltaTime = (float)(_pNodeAnim->mRotationKeys[NextRotationIndex].mTime - _pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (_AnimationTime - (float)_pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;

	//assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiQuaternion& StartRotationQ = _pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = _pNodeAnim->mRotationKeys[NextRotationIndex].mValue;

	aiQuaternion::Interpolate(_Out, StartRotationQ, EndRotationQ, Factor);

	_Out = _Out.Normalize();
}

void Model::CalcInterpolatedPosition(aiVector3D& _Out, const float _AnimationTime, const aiNodeAnim* _pNodeAnim)
{
	if (_pNodeAnim->mNumPositionKeys == 1)
	{
		_Out = _pNodeAnim->mPositionKeys[0].mValue;

		return;
	}

	unsigned int PositionIndex = FindPosition(_AnimationTime, _pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);

	//assert(NextPositionIndex < _pNodeAnim->mNumPositionKeys);

	float DeltaTime = (float)(_pNodeAnim->mPositionKeys[NextPositionIndex].mTime - _pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (_AnimationTime - (float)_pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;

	//assert(Factor >= 0.0f && Factor <= 1.0f);

	const aiVector3D& Start = _pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = _pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;

	_Out = Start + Factor * Delta;
}

unsigned int Model::FindScaling(const float _AnimationTime, const aiNodeAnim* _pNodeAnim)
{
	//assert(_pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < _pNodeAnim->mNumScalingKeys - 1; ++i)
	{
		if (_AnimationTime < (float)_pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	//assert(0);

	return 0;
}

unsigned int Model::FindRotation(const float _AnimationTime, const aiNodeAnim* _pNodeAnim)
{
	//assert(_pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < _pNodeAnim->mNumRotationKeys - 1; ++i)
	{
		if (_AnimationTime < (float)_pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	//assert(0);

	return 0;
}

unsigned int Model::FindPosition(const float _AnimationTime, const aiNodeAnim* _pNodeAnim)
{
	for (unsigned int i = 0; i < _pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (_AnimationTime < (float)_pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	//assert(0);

	return 0;
}


bool Model::GetPassID(const int _PassID)
{
	#if defined(DX11)
	for (unsigned int i = 0; i < m_vPassID.size(); ++i)
	{
		if (m_vPassID[i] == _PassID)
		{
			return true;
		}
	}
	return false;
	#endif
	return false;
}
#if defined(DX11)
void Model::CleanUpDXResources()
{
	for (unsigned int i = 0; i < m_MeshesNum; ++i)
	{
		m_vMeshes[i].CleanUpDXResources();
	}
}
#endif
