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
}
