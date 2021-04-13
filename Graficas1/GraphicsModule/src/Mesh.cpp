#include "Mesh.h"

Mesh::Mesh() : m_Vertex{ nullptr }, m_NumOfVertex{ 0 }, m_VertexIndex{ nullptr }, m_NumOfVertexIndex{ 0 }, m_ShowTexture{ false }
{

	Matrix::SetIdentityMatrix(m_ModelMatrix);
	Matrix::SetIdentityMatrix(m_TraslationMatrix);
	Matrix::SetIdentityMatrix(m_RotationMatrix);
	Matrix::SetIdentityMatrix(m_ScaleMatrix);

	m_MeshColor[0] = 0.0f;
	m_MeshColor[1] = 0.0f;
	m_MeshColor[2] = 0.0f;
}

Mesh::~Mesh()
{
	if (m_Vertex != nullptr)
	{
		delete[] m_Vertex;
		m_Vertex = nullptr;
	}

	if (m_VertexIndex != nullptr)
	{
		delete[] m_VertexIndex;
		m_VertexIndex = nullptr;
	}
}

void Mesh::SetVertex(Vertex* figure, int numofvertex)
{
	if (m_Vertex != nullptr)
	{
		delete[] m_Vertex;
	}

	m_Vertex = new Vertex[numofvertex];
	memcpy(m_Vertex, figure, sizeof(figure[0]) * numofvertex);
}

void Mesh::SetVertexIndex(unsigned int* vertexindexarr, int numofvertexindex)
{
	if (m_VertexIndex != nullptr)
	{
		delete[] m_VertexIndex;
	}
	
	m_VertexIndex = new unsigned int[numofvertexindex];
	memcpy(m_VertexIndex, vertexindexarr, sizeof(vertexindexarr[0]) * numofvertexindex);
}

void Mesh::Update(GraphicsModule::test& _obj, HWND _hwnd)
{

	float RT[16];
	float MV[16];
	float MVP[16];

	Matrix::MatrixMultiplication(m_TraslationMatrix, m_RotationMatrix, RT);
	Matrix::MatrixMultiplication(RT, m_ScaleMatrix, m_ModelMatrix);
	Matrix::MatrixMultiplication(m_ModelMatrix, _obj.m_Camera->GetViewMatrix(), MV);
	Matrix::MatrixMultiplication(MV, _obj.m_Camera->GetPerspectiveProjectionMatrix(), MVP);

#if defined(DX11)
	for (int i = 0; i < 16; ++i)
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

	m_cb.vMeshColor[0] = m_MeshColor[0];
	m_cb.vMeshColor[1] = m_MeshColor[1];
	m_cb.vMeshColor[2] = m_MeshColor[2];
#endif
#if defined(OGL)
	//static float t = 3.14f;
	//t += 0.01f;
	//float angle = t / 1000.0f * 45.0f;

	//m_Rotation[0] = cos(t);
	//m_Rotation[1] = 0.0f;
	//m_Rotation[2] = sin(t);
	//m_Rotation[3] = 0.0f;
	//
	//m_Rotation[4] = 0.0f;
	//m_Rotation[5] = 1.0f;
	//m_Rotation[6] = 0.0f;
	//m_Rotation[7] = 0.0f;
	//
	//m_Rotation[8] = -sin(t);
	//m_Rotation[9] = 0.0f;
	//m_Rotation[10] = cos(t);
	//m_Rotation[11] = 0.0f;
	//
	//m_Rotation[12] = 0.0f;
	//m_Rotation[13] = 0.0f;
	//m_Rotation[14] = 0.0f;
	//m_Rotation[15] = 1.0f;



	const char* UniformName;
	int UniformMVP;
	UniformName = "mvp";
	UniformMVP = glGetUniformLocation(_obj.programID, UniformName);
	if (UniformMVP == -1) 
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformName);
	}
	glUniformMatrix4fv(UniformMVP, 1, 0, glm::value_ptr(glm::make_mat4(MVP)));
#endif
}

void Mesh::Render(GraphicsModule::test& _obj, HWND _hwnd)
{
#if defined(DX11)
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GraphicsModule::SetVertexBufferStruct SetVBStruct;
	SetVBStruct.StartSlot = 0;
	SetVBStruct.NumBuffers = 1;
	SetVBStruct.ppVertexBuffers = _obj.g_SimeVertexBuffer.GetVertexBufferAddress();
	SetVBStruct.pStrides = &stride;
	SetVBStruct.pOffsets = &offset;

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetVertexBuffers(SetVBStruct);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetIndexBuffer(_obj.g_SimeIndexBuffer.GetIndexBuffer(), GraphicsModule::SIME_FORMAT_R32_UINT, 0);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetPrimitiveTopology(GraphicsModule::SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Material->Render(_hwnd, _obj, m_cb);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(3, 1, _obj.g_DirLightBuffer.BGetBufferAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CDrawIndexed(m_NumOfVertexIndex, 0, 0);


#endif
#if defined(OGL)
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	//
	//glVertexAttribPointer(
	//	0,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
	//	3,                  // tamaño
	//	GL_FLOAT,           // tipo
	//	GL_FALSE,           // normalizado?
	//	sizeof(Vertex),                    // Paso
	//	(void*)0            // desfase del buffer
	//);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	//glActiveTexture(GL_TEXTURE0);
	//glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glBindVertexArray(m_VAO);
	glDrawElements(SIME_TRIANGLES, m_NumOfVertexIndex, SIME_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDisableVertexAttribArray(0);
#endif
}

void Mesh::SetUpMesh(GraphicsModule::test& _obj, HWND _hwnd, const char* _FileName)
{
#if defined(DX11)
	HRESULT hr;
	GraphicsModule::UpdateBDStruct UpdateBDS;
	UpdateBDS.Usage = GraphicsModule::SIME_USAGE_DEFAULT;
	UpdateBDS.ByteWidth = sizeof(Vertex) * m_NumOfVertex;
	UpdateBDS.BindFlags = GraphicsModule::SIME_BIND_VERTEX_BUFFER;
	UpdateBDS.CPUAccessFlags = 0;
	UpdateBDS.MiscFlags = 0;
	UpdateBDS.StructureBytestride = 0;

	_obj.g_SimeVertexBuffer.UpdateBD(UpdateBDS);
	_obj.g_SimeVertexBuffer.UpdateInitData(m_Vertex);
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(_obj.g_SimeVertexBuffer.GetBDAddress(), _obj.g_SimeVertexBuffer.GetInitDataAddress(),
		_obj.g_SimeVertexBuffer.GetVertexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 1" << std::endl;
	}

	UpdateBDS.ByteWidth = sizeof(unsigned int) * m_NumOfVertexIndex;
	UpdateBDS.BindFlags = GraphicsModule::SIME_BIND_INDEX_BUFFER;

	_obj.g_SimeIndexBuffer.UpdateBD(UpdateBDS);
	_obj.g_SimeIndexBuffer.UpdateInitData(m_VertexIndex);
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(_obj.g_SimeIndexBuffer.GetBDAddress(), _obj.g_SimeIndexBuffer.GetInitDataAdress(),
		_obj.g_SimeIndexBuffer.GetIndexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 2" << std::endl;
	}

	LoadTexture(_FileName, _obj, _hwnd);
#endif
#if defined(OGL)
	float MV[16];
	float MVP[16];
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(SIME_ARRAY_BUFFER, m_VBO);

	glBufferData(SIME_ARRAY_BUFFER, m_NumOfVertex * sizeof(Vertex), &m_Vertex[0], SIME_STATIC_DRAW);

	glBindBuffer(SIME_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(SIME_ELEMENT_ARRAY_BUFFER, m_NumOfVertexIndex * sizeof(unsigned int), &m_VertexIndex[0], SIME_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, SIME_FLOAT, 0, sizeof(Vertex), (void*)16);
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, SIME_FLOAT, 0, sizeof(Vertex), (void*)32);

	glBindVertexArray(0);
	LoadTexture(_FileName, _obj, _hwnd);
	Matrix::MatrixMultiplication(m_ModelMatrix, _obj.m_Camera->GetViewMatrix(), MV);
	Matrix::MatrixMultiplication(MV, _obj.m_Camera->GetPerspectiveProjectionMatrix(), MVP);
	//mvp = glm::make_mat4(m_Camera->GetPerspectiveProjectionMatrix()) * glm::make_mat4(m_Camera->GetViewMatrix()) * glm::make_mat4(modelmatrix);
	//mvp = glm::make_mat4(truematrix);
	const char* UniformName;
	int UniformMVP;
	UniformName = "mvp";
	UniformMVP = glGetUniformLocation(_obj.programID, UniformName);
	if (UniformMVP == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", UniformName);
	}
	glUniformMatrix4fv(UniformMVP, 1, 0, glm::value_ptr(glm::make_mat4(MVP)));
#endif
}

void Mesh::LoadTexture(const char* _FileName, GraphicsModule::test& _obj, HWND _hwnd)
{
	float width;
	float height;
	const unsigned char* bits(0);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(_FileName, 0);
	m_dib1 = FreeImage_Load(fif, _FileName);
	#if defined(DX11)
	FIBITMAP* dib;
	dib = FreeImage_ConvertTo32Bits(m_dib1);
	bits = FreeImage_GetBits(dib);
	#endif
	#if defined(OGL)
	//FIBITMAP* dib;
	//dib = FreeImage_ConvertTo32Bits(m_dib1);
	bits = FreeImage_GetBits(m_dib1);
	height = FreeImage_GetHeight(m_dib1);
	width = FreeImage_GetWidth(m_dib1);
	#endif
	if (!m_dib1)
	{
		std::cerr << "Error al cargar la imagen" << std::endl;
	}
	else
	{

	#if defined(DX11)
	HRESULT hr;
	height = FreeImage_GetHeight(dib);
	width = FreeImage_GetWidth(dib);
	int RowPitch = FreeImage_GetPitch(dib);
	
	//ShaderResourceView EntryShader;
	GraphicsModule::SetRTDescStruct TextureDesc;
	
	TextureDesc.Width = width;
	TextureDesc.Height = height;
	TextureDesc.MipLevels = 0;
	TextureDesc.Arraysize = 1;
	TextureDesc.Format = GraphicsModule::SIME_FORMAT_B8G8R8A8_UNORM;
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

	_obj.g_SimeTextureRV.SetDesc(GraphicsModule::SIME_FORMAT_B8G8R8A8_UNORM, GraphicsModule::SIME_SRV_DIMENSION_TEXTURE2D, 1);
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateShaderResourceView(EntryTexture.GetTexture(), _obj.g_SimeTextureRV.GetDXSRVDescAddress(),
	_obj.g_SimeTextureRV.GetDXSRVAddress());
	if (FAILED(hr))
	{
		std::cout << "Error en el srv dx" << std::endl;
	}
	FreeImage_Unload(dib);
	#endif
	#if defined(OGL)
		glGenTextures(1, &tex_id);
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	
		glGenerateMipmap(GL_TEXTURE_2D);
	#endif
		m_ShowTexture = true;

		FreeImage_Unload(m_dib1);
	}
}
