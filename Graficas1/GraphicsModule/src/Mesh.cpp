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
	for (int i = 0; i < 16; ++i)
	{
		cb.mWorld[i] = m_WorldMatrix[i];
	}

	cb.mWorld[0] = m_WorldMatrix[0];
	cb.mWorld[1] = m_WorldMatrix[4];
	cb.mWorld[2] = m_WorldMatrix[8];
	cb.mWorld[3] = m_WorldMatrix[12];

	cb.mWorld[4] = m_WorldMatrix[1];
	cb.mWorld[5] = m_WorldMatrix[5];
	cb.mWorld[6] = m_WorldMatrix[9];
	cb.mWorld[7] = m_WorldMatrix[13];

	cb.mWorld[8] = m_WorldMatrix[2];
	cb.mWorld[9] = m_WorldMatrix[6];
	cb.mWorld[10] = m_WorldMatrix[10];
	cb.mWorld[11] = m_WorldMatrix[14];

	cb.mWorld[12] = m_WorldMatrix[3];
	cb.mWorld[13] = m_WorldMatrix[7];
	cb.mWorld[14] = m_WorldMatrix[11];
	cb.mWorld[15] = m_WorldMatrix[15];


	cb.vMeshColor[0] = m_MeshColor[0];
	cb.vMeshColor[1] = m_MeshColor[1];
	cb.vMeshColor[2] = m_MeshColor[2];

}

void Mesh::Render(GraphicsModule::test& _obj, HWND _hwnd)
{
#if defined(DX11)
	m_Material->Render(_hwnd, _obj, cb);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetConstantBuffers(3, 1, _obj.g_DirLightBuffer.BGetBufferAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CDrawIndexed(m_NumOfVertexIndex, 0, 0);
#endif
}

void Mesh::UpdateVertexAndIndexBuffer(GraphicsModule::test& _obj, HWND _hwnd)
{
#if defined(DX11)
	HRESULT hr;
	_obj.g_SimeVertexBuffer.UpdateBD(D3D11_USAGE_DEFAULT, sizeof(Vertex) * m_NumOfVertex, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0);
	_obj.g_SimeVertexBuffer.UpdateInitData(m_Vertex);
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(_obj.g_SimeVertexBuffer.GetBDAddress(), _obj.g_SimeVertexBuffer.GetInitDataAddress(),
		_obj.g_SimeVertexBuffer.GetVertexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 1" << std::endl;
	}

	_obj.g_SimeIndexBuffer.UpdateBD(D3D11_USAGE_DEFAULT, sizeof(unsigned int) * m_NumOfVertexIndex, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
	_obj.g_SimeIndexBuffer.UpdateInitData(m_VertexIndex);
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateBuffer(_obj.g_SimeIndexBuffer.GetBDAddress(), _obj.g_SimeIndexBuffer.GetInitDataAdress(),
		_obj.g_SimeIndexBuffer.GetIndexBufferAddress());
	if (FAILED(hr))
	{
		std::cout << "error hr 2" << std::endl;
	}
#endif
}
