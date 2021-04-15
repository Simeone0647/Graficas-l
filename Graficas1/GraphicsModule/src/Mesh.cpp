#include "Mesh.h"

Mesh::Mesh() : m_NumOfVertex{ 0 }, m_NumOfVertexIndex{ 0 }, m_ShowTexture{ false }
{

	Matrix::SetIdentity(m_ModelMatrix);
	Matrix::SetIdentity(m_TraslationMatrix);
	Matrix::SetIdentity(m_RotationMatrix);
	Matrix::SetIdentity(m_ScaleMatrix);
	Matrix::SetIdentity(m_MVP);

	m_MeshColor[0] = 0.0f;
	m_MeshColor[1] = 0.0f;
	m_MeshColor[2] = 0.0f;
}

Mesh::Mesh(std::vector<Vertex> _MeshVertex, std::vector<unsigned int> _MeshIndices, std::vector<std::string> _Filename, unsigned int _VertexNum, bool _LoadRGBA, bool _LoadBGRA,
		   bool _LoadTriangles, bool _LoadPoints, std::string _Name, unsigned int _ShaderID)
{
	m_Vertex = _MeshVertex;
	m_VertexIndex = _MeshIndices;
	m_NumOfVertex = m_Vertex.size();
	m_NumOfVertexIndex = m_VertexIndex.size();
	m_Filename = _Filename;
	//if (!m_Filename.empty())
	//{ 
	//	std::cout << m_Filename[0] << std::endl;
	//}
	Matrix::SetIdentity(m_ModelMatrix);
	Matrix::SetIdentity(m_TraslationMatrix);
	Matrix::SetIdentity(m_RotationMatrix);
	Matrix::SetIdentity(m_ScaleMatrix);
	Matrix::SetIdentity(m_MVP);

	m_MeshColor[0] = 0.0f;
	m_MeshColor[1] = 0.0f;
	m_MeshColor[2] = 0.0f;

	m_LoadAsRGBA = _LoadRGBA;
	m_LoadAsBGRA = _LoadBGRA;
	m_LoadAsPoints = _LoadPoints;
	m_LoadAsTriangles = _LoadTriangles;

	m_Material = new Material;

	m_Name = _Name;

	m_ShaderID = _ShaderID;
}

Mesh::~Mesh()
{

}

void Mesh::SetVertex(Vertex* figure, int numofvertex)
{
	//if (m_Vertex != nullptr)
	//{
	//	delete[] m_Vertex;
	//}
	//
	//m_Vertex = new Vertex[numofvertex];
	//memcpy(m_Vertex, figure, sizeof(figure[0]) * numofvertex);
}

void Mesh::SetVertexIndex(unsigned int* vertexindexarr, int numofvertexindex)
{
	//if (m_VertexIndex != nullptr)
	//{
	//	delete[] m_VertexIndex;
	//}
	//
	//m_VertexIndex = new unsigned int[numofvertexindex];
	//memcpy(m_VertexIndex, vertexindexarr, sizeof(vertexindexarr[0]) * numofvertexindex);
}

void Mesh::Update()
{
#if defined(DX11)

	for (unsigned int i = 0; i < 16; ++i)
	{
		m_cb.mWorld[i] = m_ModelMatrix[i];
	}

	m_cb.mWorld[0] = m_ModelMatrix[0];
	m_cb.mWorld[1] = m_ModelMatrix[4];
	m_cb.mWorld[2] = m_ModelMatrix[8];
	m_cb.mWorld[3] = m_ModelMatrix[12];
						
	m_cb.mWorld[4] = m_ModelMatrix[1];
	m_cb.mWorld[5] = m_ModelMatrix[5];
	m_cb.mWorld[6] = m_ModelMatrix[9];
	m_cb.mWorld[7] = m_ModelMatrix[13];
						
	m_cb.mWorld[8] = m_ModelMatrix[2];
	m_cb.mWorld[9] = m_ModelMatrix[6];
	m_cb.mWorld[10] = m_ModelMatrix[10];
	m_cb.mWorld[11] = m_ModelMatrix[14];
	
	m_cb.mWorld[12] = m_ModelMatrix[3];
	m_cb.mWorld[13] = m_ModelMatrix[7];
	m_cb.mWorld[14] = m_ModelMatrix[11];
	m_cb.mWorld[15] = m_ModelMatrix[15];
	
	m_cb.vMeshColor[0] = m_ModelMatrix[0];
	m_cb.vMeshColor[1] = m_ModelMatrix[1];
	m_cb.vMeshColor[2] = m_ModelMatrix[2];
#endif
#if defined(OGL)

#endif
}

void Mesh::Render(VertexBuffer& _VB, IndexBuffer& _IB, HWND _hwnd)
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

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetVertexBuffers(SetVBStruct);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetIndexBuffer(_IB.GetIndexBuffer(), GraphicsModule::SIME_FORMAT_R32_UINT, 0);
	if (m_LoadAsTriangles)
	{ 
		GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetPrimitiveTopology(GraphicsModule::SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	else if (m_LoadAsPoints)
	{
		GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetPrimitiveTopology(GraphicsModule::SIME_PRIMITIVE_TOPOLOGY_POINTLIST);
	}

	m_Material->Render(_hwnd, m_cb, m_MeshCB);
	
	//GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	//
	//GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, m_MeshCB.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, m_MeshCB.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CDrawIndexed(m_VertexIndex.size(), 0, 0);

#endif
#if defined(OGL)
	const char* UniformName;
	int UniformMVP;
	UniformName = "mvp";
	UniformMVP = glGetUniformLocation(m_ShaderID, UniformName);
	if (UniformMVP == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformName);
	}
	glUniformMatrix4fv(UniformMVP, 1, 0, glm::value_ptr(glm::make_mat4(m_MVP)));

	const char* UniformNameModel;
	int UniformModel;
	UniformNameModel = "world";
	UniformModel = glGetUniformLocation(m_ShaderID, UniformNameModel);
	if (UniformModel == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameModel);
	}
	glUniformMatrix4fv(UniformModel, 1, 0, glm::value_ptr(glm::make_mat4(m_ModelMatrix)));

	//const char* UniformNameLight;
	//int UniformLight;
	//UniformNameLight = "dirlight";
	//UniformLight = glGetUniformLocation(m_ShaderID, UniformNameLight);
	//if (UniformLight == -1)
	//{
	//	fprintf(stderr, "Could not bind uniform %s\n", UniformNameLight);
	//}
	//glUniform4fv(UniformLight, 1, glm::value_ptr(glm::make_vec4(_obj.g_DirLightBufferDesc.Dir)));

	glBindTexture(GL_TEXTURE_2D, m_TexID);
	glBindVertexArray(m_VAO);
	if (m_LoadAsTriangles)
	{
		glDrawElements(SIME_TRIANGLES, m_NumOfVertexIndex, SIME_UNSIGNED_INT, 0);
	}
	else if (m_LoadAsPoints)
	{
		glDrawElements(GL_POINTS, m_NumOfVertexIndex, SIME_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
	//glDisableVertexAttribArray(0);
#endif
}

void Mesh::SetUpMesh(VertexBuffer& _VB, IndexBuffer& _IB, HWND _hwnd)
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
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(m_MeshCB.GetBDAddress(), NULL, m_MeshCB.GetCBChangesEveryFrameAddress());
	if (FAILED(hr))
		std::cout << "Error hr de cb" << std::endl;

	GraphicsModule::UpdateBDStruct UpdateBDS;
	UpdateBDS.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
	UpdateBDS.ByteWidth = sizeof(Vertex) * m_Vertex.size();
	UpdateBDS.BindFlags = GraphicsModule::SIME_BIND_VERTEX_BUFFER;
	UpdateBDS.CPUAccessFlags = 0;
	UpdateBDS.MiscFlags = 0;
	UpdateBDS.StructureBytestride = 0;

	_VB.UpdateBD(UpdateBDS);
	_VB.UpdateInitData(m_Vertex.data());
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(_VB.GetBDAddress(), _VB.GetInitDataAddress(), _VB.GetVertexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 1" << std::endl;
	}

	UpdateBDS.ByteWidth = sizeof(unsigned int) * m_VertexIndex.size();
	UpdateBDS.BindFlags = GraphicsModule::SIME_BIND_INDEX_BUFFER;

	_IB.UpdateBD(UpdateBDS);
	_IB.UpdateInitData(m_VertexIndex.data());
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(_IB.GetBDAddress(), _IB.GetInitDataAdress(), _IB.GetIndexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 2" << std::endl;
	}

	LoadTexture(_hwnd);
#endif
#if defined(OGL)
	float MV[16];
	float MVP[16];
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(SIME_ARRAY_BUFFER, m_VBO);

	glBufferData(SIME_ARRAY_BUFFER, m_NumOfVertex * sizeof(Vertex), m_Vertex.data(), SIME_STATIC_DRAW);

	glBindBuffer(SIME_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(SIME_ELEMENT_ARRAY_BUFFER, m_NumOfVertexIndex * sizeof(unsigned int), m_VertexIndex.data(), SIME_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)16);
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, SIME_FLOAT, 0, sizeof(Vertex), (void*)32);

	glBindVertexArray(0);

	//Matrix::MatrixMultiplication(m_ModelMatrix, _obj.m_Camera->GetViewMatrix(), MV);
	//Matrix::MatrixMultiplication(MV, _obj.m_Camera->GetPerspectiveProjectionMatrix(), MVP);

	LoadTexture(_hwnd);
#endif
}

#if defined(DX11) || defined(OGL)
void Mesh::LoadTexture(HWND _hwnd)
{
	if (m_Filename.empty())
	{
		return;
	}

	float width;
	float height;
	const unsigned char* bits(0);
	char* char_arr;
	
	char_arr = &m_Filename[0][0];
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	std::cout << m_Filename[0] << std::endl;

	fif = FreeImage_GetFileType(char_arr, 0);
	m_dib1 = FreeImage_Load(fif, char_arr);

	FIBITMAP* dib;
	dib = FreeImage_ConvertTo32Bits(m_dib1);
	bits = FreeImage_GetBits(dib);
	height = FreeImage_GetHeight(dib);
	width = FreeImage_GetWidth(dib);

	if (!m_dib1)
	{
		std::cerr << "Error al cargar la imagen" << std::endl;
	}
	else
	{

	#if defined(DX11)
	HRESULT hr;
	int RowPitch = FreeImage_GetPitch(dib);
	
	//ShaderResourceView EntryShader;
	GraphicsModule::SetRTDescStruct TextureDesc;
	
	TextureDesc.Width = width;
	TextureDesc.Height = height;
	TextureDesc.MipLevels = 0;
	TextureDesc.Arraysize = 1;
	if (m_LoadAsBGRA)
	{ 
		TextureDesc.Format = GraphicsModule::SIME_FORMAT_B8G8R8A8_UNORM;
	}

	else if (m_LoadAsRGBA)
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
	UpdateSRStruct.pSrcData = bits;
	UpdateSRStruct.SrcRowPitch = RowPitch;
	UpdateSRStruct.SrcDepthPitch = 0;
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	if (m_LoadAsBGRA)
	{
		m_Material->GetSRVTexture()->SetDesc(GraphicsModule::SIME_FORMAT_B8G8R8A8_UNORM, GraphicsModule::SIME_SRV_DIMENSION_TEXTURE2D, 1);
	}
	else if (m_LoadAsRGBA)
	{
		m_Material->GetSRVTexture()->SetDesc(GraphicsModule::SIME_FORMAT_R8G8B8A8_UNORM, GraphicsModule::SIME_SRV_DIMENSION_TEXTURE2D, 1);
	}
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateShaderResourceView(EntryTexture.GetTexture(), m_Material->GetSRVTexture()->GetDXSRVDescAddress(),
	m_Material->GetSRVTexture()->GetDXSRVAddress());
	if (FAILED(hr))
	{
		std::cout << "Error en el srv dx" << std::endl;
	}

	#endif
	#if defined(OGL)
		glGenTextures(1, &m_TexID);
		glBindTexture(GL_TEXTURE_2D, m_TexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		if (m_LoadAsRGBA)
		{ 
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bits);
		}
		else if (m_LoadAsBGRA)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	#endif
		m_ShowTexture = true;

		FreeImage_Unload(dib);
		FreeImage_Unload(m_dib1);
	}
}
#endif

#if defined(DX11)
void Mesh::CleanUpDXResources()
{
	if (m_Material->GetSRVTexture()->GetDXSRV())
	{
		m_Material->GetSRVTexture()->GetDXSRV()->Release();
	}
	if (m_MeshCB.GetCBChangesEveryFrame())
	{
		m_MeshCB.GetCBChangesEveryFrame()->Release();
	}
}
#endif
