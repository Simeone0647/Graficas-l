#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> _Vertex, std::vector<unsigned int> _Indices, std::vector<std::string> _TexturesNames, const int _Flags[], std::string _Name)
{
	m_vVertex = _Vertex;
	m_vVertexIndex = _Indices;
	m_VertexNum = m_vVertex.size();
	m_VertexIndexNum = m_vVertexIndex.size();
	m_vTexturesNames = _TexturesNames;
	m_Name = _Name;

	for (unsigned int i = 0; i < 2; ++i)
	{
		for (unsigned int j = 0; j < LoadTypes::LoadBGRA; ++j)
		{
			if (_Flags[i] == LoadTypes::LoadTriangles)
			{
				m_LoadTypes[0] = true;
			}
			else if (_Flags[i] == LoadTypes::LoadPoints)
			{
				m_LoadTypes[1] = true;
			}
			else if (_Flags[i] == LoadTypes::LoadBGRA)
			{
				m_LoadTypes[2] = true;
			}
			else if (_Flags[i] == LoadTypes::LoadRGBA)
			{
				m_LoadTypes[3] = true;
			}
		}
	}

	#if defined(DX11)
	m_MeshColor[0] = 0.0f;
	m_MeshColor[1] = 0.0f;
	m_MeshColor[2] = 0.0f;
	#endif
	
	#if defined(OGL)
	m_ModelMatrix = new float[16];
	m_MVP = new float[16];
	#endif

	m_Material = new Material;
}

Mesh::~Mesh()
{
}



void Mesh::Update()
{
#if defined(DX11)
	m_CBChangesEveryFrame.mWorld = Matrix::Transpose(m_ModelMatrix);
#endif
}

void Mesh::Render(VertexBuffer& _VB, IndexBuffer& _IB, HWND _Hwnd)
{
#if defined(DX11)
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GraphicsModule::SetVertexBufferStruct SetVBStruct;
	SetVBStruct.StartSlot = 0;
	SetVBStruct.NumBuffers = 1;
	SetVBStruct.ppVertexBuffers = _VB.GetVertexBufferAddress();
	SetVBStruct.pStrides = &stride;
	SetVBStruct.pOffsets = &offset;

	GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CIASetVertexBuffers(SetVBStruct);
	GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CIASetIndexBuffer(_IB.GetIndexBuffer(), GraphicsModule::SIME_FORMAT_R32_UINT, 0);
	if (m_LoadTypes[0])
	{ 
		GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CIASetPrimitiveTopology(GraphicsModule::SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else if (m_LoadTypes[1])
	{
		GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CIASetPrimitiveTopology(GraphicsModule::SIME_PRIMITIVE_TOPOLOGY_POINTLIST);
	}

	m_Material->Render(_Hwnd, m_CBChangesEveryFrame, m_MeshCB);
	
	GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, m_MeshCB.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, m_MeshCB.GetCBChangesEveryFrameAddress());

	if (m_Material->GetTexNum() > 0)
	{
		GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CPSSetSamplers(0, m_Samplers.GetSamplerNum(), m_Samplers.GetDXSamplerStateAddress());
	}
	GraphicsModule::GetManagerObj(_Hwnd).GetDeviceContext().CDrawIndexed(m_VertexIndexNum, 0, 0);

#endif
#if defined(OGL)
	int ShaderID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

	const char* UniformNameModel;
	int UniformModel;
	UniformNameModel = "World";
	UniformModel = glGetUniformLocation(ShaderID, UniformNameModel);
	if (UniformModel == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameModel);
	}
	glUniformMatrix4fv(UniformModel, 1, 0, glm::value_ptr(glm::make_mat4(m_ModelMatrix)));

	for (unsigned int i = 0; i < m_vTexID.size(); ++i)
	{
		if (i == 0)
		{
			glUniform1i(glGetUniformLocation(ShaderID, "AlbedoMap"), 0);
			glActiveTexture(GL_TEXTURE0);
		}
		else if (i == 1)
		{
			glUniform1i(glGetUniformLocation(ShaderID, "SpecularMap"), 1);
			glActiveTexture(GL_TEXTURE1);
		}
		else if (i == 2)
		{
			glUniform1i(glGetUniformLocation(ShaderID, "NormalMap"), 2);
			glActiveTexture(GL_TEXTURE2);
		}
		glBindTexture(GL_TEXTURE_2D, m_vTexID[i]);
	}
	glBindVertexArray(_VB.GetVAO());
	if (m_LoadTypes[0])
	{
		glDrawElements(SIME_TRIANGLES, m_VertexIndexNum, SIME_UNSIGNED_INT, 0);
	}
	else if (m_LoadTypes[1])
	{
		glDrawElements(GL_POINTS, m_VertexIndexNum, SIME_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	//glDisableVertexAttribArray(0);
#endif
}

void Mesh::SetUpMesh(VertexBuffer& _VB, IndexBuffer& _IB, HWND _Hwnd)
{
#if defined(DX11)
	HRESULT hr;

	GraphicsModule::UpdateBDStruct BDStruct;
	BDStruct.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
	BDStruct.ByteWidth = sizeof(GraphicsModule::CBNeverChanges);
	BDStruct.BindFlags = GraphicsModule::SIME_BIND_CONSTANT_BUFFER;
	BDStruct.CPUAccessFlags = 0;
	BDStruct.StructureBytestride = 0;
	BDStruct.MiscFlags = 0;
	BDStruct.ByteWidth = sizeof(CBChangesEveryFrame);

	m_MeshCB.UpdateBD(BDStruct);
	hr = GraphicsModule::GetManagerObj(_Hwnd).GetDevice().CCreateBuffer(m_MeshCB.GetBDAddress(), NULL, m_MeshCB.GetCBChangesEveryFrameAddress());
	if (FAILED(hr))
		std::cout << "Error hr de cb" << std::endl;

	GraphicsModule::UpdateBDStruct UpdateBDS;
	UpdateBDS.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
	UpdateBDS.ByteWidth = sizeof(Vertex) * m_VertexNum;
	UpdateBDS.BindFlags = GraphicsModule::SIME_BIND_VERTEX_BUFFER;
	UpdateBDS.CPUAccessFlags = 0;
	UpdateBDS.MiscFlags = 0;
	UpdateBDS.StructureBytestride = 0;

	_VB.UpdateBD(UpdateBDS);
	_VB.UpdateInitData(m_vVertex.data());
	hr = GraphicsModule::GetManagerObj(_Hwnd).GetDevice().CCreateBuffer(_VB.GetBDAddress(), _VB.GetInitDataAddress(), _VB.GetVertexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 1" << std::endl;
	}

	UpdateBDS.ByteWidth = sizeof(unsigned int) * m_VertexIndexNum;
	UpdateBDS.BindFlags = GraphicsModule::SIME_BIND_INDEX_BUFFER;

	_IB.UpdateBD(UpdateBDS);
	_IB.UpdateInitData(m_vVertexIndex.data());
	hr = GraphicsModule::GetManagerObj(_Hwnd).GetDevice().CCreateBuffer(_IB.GetBDAddress(), _IB.GetInitDataAdress(), _IB.GetIndexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 2" << std::endl;
	}

	LoadTexture(_Hwnd);
#endif
#if defined(OGL)
	glGenVertexArrays(1, _VB.GetVAOAddress());
	glGenBuffers(1, _VB.GetVBOAddress());
	glGenBuffers(1, _IB.GetEBOAddress());

	glBindVertexArray(_VB.GetVAO());
	glBindBuffer(SIME_ARRAY_BUFFER, _VB.GetVBO());

	glBufferData(SIME_ARRAY_BUFFER, m_VertexNum * sizeof(Vertex), m_vVertex.data(), SIME_STATIC_DRAW);

	glBindBuffer(SIME_ELEMENT_ARRAY_BUFFER, _IB.GetEBO());
	glBufferData(SIME_ELEMENT_ARRAY_BUFFER, m_VertexIndexNum * sizeof(unsigned int), m_vVertexIndex.data(), SIME_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)16);
	//binormals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)32);
	//tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)48);
	// vertex texture coords
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, SIME_FLOAT, 0, sizeof(Vertex), (void*)64);

	glBindVertexArray(0);

	LoadTexture(_Hwnd);
#endif
}

#if defined(DX11) || defined(OGL)
void Mesh::LoadTexture(HWND _hwnd)
{
	if (m_vTexturesNames.empty())
	{
		return;
	}

	FIBITMAP* FirstDib;

	float Width;
	float Height;

	const unsigned char* Bits(0);
	char* Char_Arr;

	for (unsigned int i = 0; i < m_vTexturesNames.size(); ++i)
	{

		Char_Arr = &m_vTexturesNames[i][0];
		FREE_IMAGE_FORMAT FIF = FIF_UNKNOWN;

		std::cout << m_vTexturesNames[i] << std::endl;

		FIF = FreeImage_GetFileType(Char_Arr, 0);
		FirstDib = FreeImage_Load(FIF, Char_Arr);

		FIBITMAP* SecondDib;
		SecondDib = FreeImage_ConvertTo32Bits(FirstDib);
		Bits = FreeImage_GetBits(SecondDib);
		Height = FreeImage_GetHeight(SecondDib);
		Width = FreeImage_GetWidth(SecondDib);

		if (!FirstDib)
		{
			std::cerr << "Error al cargar la imagen" << std::endl;
		}
		else
		{

		#if defined(DX11)
		HRESULT hr;

		//m_vSamplers.push_back(SamplerState());
		//m_vSamplers[i].SetDesc();

		m_Samplers.AddSampler();
		m_Samplers.AddDesc();
		m_Samplers.SetDesc(false, i);

		hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateSamplerState(m_Samplers.GetDXSamplerDescAddress(i), m_Samplers.GetLastElementAddress());

		if (FAILED(hr))
		{
			cout << "Fallo sampler" << endl;
		}

		Texture2D EntryTexture;

		int RowPitch = FreeImage_GetPitch(SecondDib);
		
		//ShaderResourceView EntryShader;
		GraphicsModule::SetRTDescStruct TextureDesc;
		
		TextureDesc.Width = Width;
		TextureDesc.Height = Height;
		TextureDesc.MipLevels = 0;
		TextureDesc.Arraysize = 1;
		if (m_LoadTypes[2])
		{ 
			TextureDesc.Format = GraphicsModule::SIME_FORMAT_B8G8R8A8_UNORM;
		}

		else if (m_LoadTypes[3])
		{
			TextureDesc.Format = GraphicsModule::SIME_FORMAT_R8G8B8A8_UNORM;
		}
		TextureDesc.Count = 1;
		TextureDesc.Quality = 0;
		TextureDesc.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
		TextureDesc.BindFlags = GraphicsModule::SIME_BIND_SHADER_RESOURCE | GraphicsModule::SIME_BIND_RENDER_TARGET;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;
		
		EntryTexture.SetDescRT(TextureDesc);

		hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateTexture2D(EntryTexture.GetDescDepthAddress(), NULL, EntryTexture.GetTextureAddress());
		if (FAILED(hr))
		{
			std::cout << "Error en la textura dx" << std::endl;
		}
		GraphicsModule::UpdateSubResourceStruct UpdateSRStruct;
		UpdateSRStruct.pDstResource = EntryTexture.GetTexture();
		UpdateSRStruct.DstSubresource = 0;
		UpdateSRStruct.pDstBox = NULL;
		UpdateSRStruct.pSrcData = Bits;
		UpdateSRStruct.SrcRowPitch = RowPitch;
		UpdateSRStruct.SrcDepthPitch = 0;
		GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

		if (m_LoadTypes[2])
		{
			m_Material->GetSRVTexture(i)->SetDesc(GraphicsModule::SIME_FORMAT_B8G8R8A8_UNORM, GraphicsModule::SIME_SRV_DIMENSION_TEXTURE2D, 1);
		}
		else if (m_LoadTypes[3])
		{
			m_Material->GetSRVTexture(i)->SetDesc(GraphicsModule::SIME_FORMAT_R8G8B8A8_UNORM, GraphicsModule::SIME_SRV_DIMENSION_TEXTURE2D, 1);
		}
		hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateShaderResourceView(EntryTexture.GetTexture(), m_Material->GetSRVTexture(i)->GetDXSRVDescAddress(),
		m_Material->GetSRVTexture(i)->GetDXSRVAddress());
		if (FAILED(hr))
		{
			std::cout << "Error en el srv dx" << std::endl;
		}

		m_Material->OneMoreTex();
	
	#endif
	#if defined(OGL)
		m_vTexID.push_back(i);
		glGenTextures(1, &m_vTexID[i]);
		glBindTexture(GL_TEXTURE_2D, m_vTexID[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		if (m_LoadTypes[3])
		{ 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Bits);
		}
		else if (m_LoadTypes[2])
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGRA, GL_UNSIGNED_BYTE, Bits);
		}
		glGenerateMipmap(GL_TEXTURE_2D);


		FreeImage_Unload(SecondDib);
		FreeImage_Unload(FirstDib);

		m_Material->OneMoreTex();
	#endif
		}
	}
}
#endif

void Mesh::SetModelMatrix(const Matrix _Matrix)
{
	#if defined(DX11)
	m_ModelMatrix = _Matrix;
	#endif
	#if defined(OGL)
	Matrix::MatrixTo1D(_Matrix, m_ModelMatrix);
	#endif
}

#if defined(DX11)
void Mesh::CleanUpDXResources()
{
	for (unsigned int i = 0; i < m_Material->GetTexNum(); ++i)
	{
		if (m_Material->GetSRVTexture(i)->GetDXSRV())
		{
			m_Material->GetSRVTexture(i)->GetDXSRV()->Release();
		}
	}
	for (unsigned int i = 0; i < m_Samplers.GetSamplerNum(); ++i)
	{
		if (m_Samplers.GetSampler(i))
		{
			m_Samplers.GetSampler(i)->Release();
		}
	}
	if (m_MeshCB.GetCBChangesEveryFrame())
	{
		m_MeshCB.GetCBChangesEveryFrame()->Release();
	}
}
#endif
