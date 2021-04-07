#include "Mesh.h"

Mesh::Mesh() : m_Vertex{ nullptr }, m_NumOfVertex{ 0 }, m_VertexIndex{ nullptr }, m_NumOfVertexIndex{ 0 }
{
	m_WorldMatrix = new float[16];

	m_WorldMatrix[0] = 1.0f;
	m_WorldMatrix[1] = 0.0f;
	m_WorldMatrix[2] = 0.0f;
	m_WorldMatrix[3] = 0.0f;

	m_WorldMatrix[4] = 0.0f;
	m_WorldMatrix[5] = 1.0f;
	m_WorldMatrix[6] = 0.0f;
	m_WorldMatrix[7] = 0.0f;

	m_WorldMatrix[8] = 0.0f;
	m_WorldMatrix[9] = 0.0f;
	m_WorldMatrix[10] = 1.0f;
	m_WorldMatrix[11] = 0.0f;

	m_WorldMatrix[12] = 0.0f;
	m_WorldMatrix[13] = 0.0f;
	m_WorldMatrix[14] = 0.0f;
	m_WorldMatrix[15] = 1.0f;

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
#if defined(DX11)
	for (int i = 0; i < 16; ++i)
	{
		m_cb.mWorld[i] = m_WorldMatrix[i];
	}

	m_cb.mWorld[0] = m_WorldMatrix[0];
	m_cb.mWorld[1] = m_WorldMatrix[4];
	m_cb.mWorld[2] = m_WorldMatrix[8];
	m_cb.mWorld[3] = m_WorldMatrix[12];

	m_cb.mWorld[4] = m_WorldMatrix[1];
	m_cb.mWorld[5] = m_WorldMatrix[5];
	m_cb.mWorld[6] = m_WorldMatrix[9];
	m_cb.mWorld[7] = m_WorldMatrix[13];

	m_cb.mWorld[8] = m_WorldMatrix[2];
	m_cb.mWorld[9] = m_WorldMatrix[6];
	m_cb.mWorld[10] = m_WorldMatrix[10];
	m_cb.mWorld[11] = m_WorldMatrix[14];

	m_cb.mWorld[12] = m_WorldMatrix[3];
	m_cb.mWorld[13] = m_WorldMatrix[7];
	m_cb.mWorld[14] = m_WorldMatrix[11];
	m_cb.mWorld[15] = m_WorldMatrix[15];

	m_cb.vMeshColor[0] = m_MeshColor[0];
	m_cb.vMeshColor[1] = m_MeshColor[1];
	m_cb.vMeshColor[2] = m_MeshColor[2];
#endif
#if defined(OGL)
	static float t = 3.14f;
	t += 0.01f;
	float angle = t / 1000.0f * 45.0f;
	float Rotation[16];

	Rotation[0] = 1.0f;
	Rotation[1] = 0.0f;
	Rotation[2] = 0.0f;
	Rotation[3] = 0.0f;

	Rotation[4] = 0.0f;
	Rotation[5] = 1.0f;
	Rotation[6] = 0.0f;
	Rotation[7] = 0.0f;

	Rotation[8] = 0.0f;
	Rotation[9] = 0.0f;
	Rotation[10] = 1.0f;
	Rotation[11] = 0.0f;

	Rotation[12] = 0.0f;
	Rotation[13] = 0.0f;
	Rotation[14] = 0.0f;
	Rotation[15] = 1.0f;

	m_Rotation[0] = cos(t);
	m_Rotation[1] = 0.0f;
	m_Rotation[2] = sin(t);
	m_Rotation[3] = 0.0f;

	m_Rotation[4] = 0.0f;
	m_Rotation[5] = 1.0f;
	m_Rotation[6] = 0.0f;
	m_Rotation[7] = 0.0f;

	m_Rotation[8] = -sin(t);
	m_Rotation[9] = 0.0f;
	m_Rotation[10] = cos(t);
	m_Rotation[11] = 0.0f;

	m_Rotation[12] = 0.0f;
	m_Rotation[13] = 0.0f;
	m_Rotation[14] = 0.0f;
	m_Rotation[15] = 1.0f;

	Matrix::MatrixMultiplication(m_Rotation, &Rotation[0], m_WorldMatrix);
	Matrix::MatrixMultiplication(m_WorldMatrix, _obj.m_Camera->GetViewMatrix(), m_MV);
	Matrix::MatrixMultiplication(m_MV, _obj.m_Camera->GetPerspectiveProjectionMatrix(), m_MVP);

	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(_obj.programID, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	}
	glUniformMatrix4fv(uniform_mvp, 1, 0, glm::value_ptr(glm::make_mat4(m_MVP)));

#endif
}

void Mesh::Render(GraphicsModule::test& _obj, HWND _hwnd)
{
#if defined(DX11)
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	m_Material->Render(_hwnd, _obj, m_cb);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(3, 1, _obj.g_DirLightBuffer.BGetBufferAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CDrawIndexed(m_NumOfVertexIndex, 0, 0);

	GraphicsModule::SetVertexBufferStruct SetVBStruct;
	SetVBStruct.StartSlot = 0;
	SetVBStruct.NumBuffers = 1;
	SetVBStruct.ppVertexBuffers = _obj.g_SimeVertexBuffer.GetVertexBufferAddress();
	SetVBStruct.pStrides = &stride;
	SetVBStruct.pOffsets = &offset;

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetVertexBuffers(SetVBStruct);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetIndexBuffer(_obj.g_SimeIndexBuffer.GetIndexBuffer(), GraphicsModule::SIME_FORMAT_R32_UINT, 0);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetPrimitiveTopology(GraphicsModule::SIME_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	//glBindTexture(GL_TEXTURE_2D, tex_id);
	glBindVertexArray(VAO);
	glDrawElements(SIME_TRIANGLES, m_NumOfVertexIndex, SIME_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glDisableVertexAttribArray(0);
#endif
}

void Mesh::UpdateVertexAndIndexBuffer(GraphicsModule::test& _obj, HWND _hwnd)
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
#endif
#if defined(OGL)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(SIME_ARRAY_BUFFER, VBO);

	glBufferData(SIME_ARRAY_BUFFER, m_NumOfVertex * sizeof(Vertex), &m_Vertex[0], SIME_STATIC_DRAW);

	glBindBuffer(SIME_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(SIME_ELEMENT_ARRAY_BUFFER, m_NumOfVertexIndex * sizeof(unsigned int), &m_VertexIndex[0], SIME_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, SIME_FLOAT, 0, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, SIME_FLOAT, 0, sizeof(Vertex), (void*)12);
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, SIME_FLOAT, 0, sizeof(Vertex), (void*)24);

	glBindVertexArray(0);

	//std::ofstream outfile("test.txt");
	//
	//outfile << "my text here!" << std::endl;
	//
	//outfile.close();
	
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float width;
	float height;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename("base_albedo.jpg");
	m_dib1 = FreeImage_Load(fif, "base_albedo.jpg", JPEG_DEFAULT);
	if (!m_dib1)
	{
		std::cerr << "Erreur ouverture d\'image" << std::endl;
	}
	else
	{
		
		height = FreeImage_GetHeight(m_dib1);
		width = FreeImage_GetWidth(m_dib1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(m_dib1));
		glGenerateMipmap(GL_TEXTURE_2D);
		FreeImage_Unload(m_dib1);
	}
	


	//RGBQUAD rgbquad;
	//
	//
	//FREE_IMAGE_TYPE type;
	//BITMAPINFOHEADER* header;
	//
	//type = FreeImage_GetImageType(m_dib1);
	//
	//height = FreeImage_GetHeight(m_dib1);
	//width = FreeImage_GetWidth(m_dib1);
	//
	//header = FreeImage_GetInfoHeader(m_dib1);
	//int scanLineWidh = ((3 * width) % 4 == 0) ? 3 * width : ((3 * width) / 4) * 4 + 4;
	//unsigned char* texels = (GLubyte*)calloc(height * scanLineWidh, sizeof(GLubyte));
	//for (x = 0; x < width; x++)
	//	for (y = 0; y < height; y++)
	//	{
	//		FreeImage_GetPixelColor(m_dib1, x, y, &rgbquad);
	//
	//		texels[(y * scanLineWidh + 3 * x)] = ((GLubyte*)&rgbquad)[2];
	//		texels[(y * scanLineWidh + 3 * x) + 1] = ((GLubyte*)&rgbquad)[1];
	//		texels[(y * scanLineWidh + 3 * x) + 2] = ((GLubyte*)&rgbquad)[0];
	//	}
	//
	//
	//
	//
	//
	//free(texels);

	m_WorldMatrix[14] = -4.0f;

	Matrix::MatrixMultiplication(m_WorldMatrix, _obj.m_Camera->GetViewMatrix(), m_MV);
	Matrix::MatrixMultiplication(m_MV, _obj.m_Camera->GetPerspectiveProjectionMatrix(), m_MVP);
	//mvp = glm::make_mat4(m_Camera->GetPerspectiveProjectionMatrix()) * glm::make_mat4(m_Camera->GetViewMatrix()) * glm::make_mat4(modelmatrix);
	//mvp = glm::make_mat4(truematrix);
	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(_obj.programID, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	}
	glUniformMatrix4fv(uniform_mvp, 1, 0, glm::value_ptr(glm::make_mat4(m_MVP)));
#endif
}
