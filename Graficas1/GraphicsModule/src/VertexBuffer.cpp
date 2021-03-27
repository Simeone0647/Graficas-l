#include "VertexBuffer.h"
#include "test.h"

VertexBuffer::VertexBuffer()
{
#if defined(DX11)
	m_pVertexBuffer = NULL;
#endif
}

VertexBuffer::~VertexBuffer()
{

}

#if defined(DX11)
void VertexBuffer::UpdateBD(GraphicsModule::UpdateBDStruct _UpdateBDStruct)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = (D3D11_USAGE)_UpdateBDStruct.Usage;
	m_BD.ByteWidth = _UpdateBDStruct.ByteWidth;
	m_BD.BindFlags = _UpdateBDStruct.BindFlags;
	m_BD.CPUAccessFlags = _UpdateBDStruct.CPUAccessFlags;
}

void VertexBuffer::UpdateInitData(void* vertex)
{
	ZeroMemory(&m_InitData, sizeof(m_InitData));
	m_InitData.pSysMem = vertex;
}
#endif