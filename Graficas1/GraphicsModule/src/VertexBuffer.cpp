#include "VertexBuffer.h"

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
void VertexBuffer::UpdateBd(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT MiscFlags, UINT StructureByteStride)
{
	ZeroMemory(&m_bd, sizeof(m_bd));
	m_bd.Usage = usage;
	m_bd.ByteWidth = bytewidth;
	m_bd.BindFlags = bindflags;
	m_bd.CPUAccessFlags = cpuaccessflags;
}

void VertexBuffer::UpdateInitData(void* vertex)
{
	ZeroMemory(&m_InitData, sizeof(m_InitData));
	m_InitData.pSysMem = vertex;
}
#endif