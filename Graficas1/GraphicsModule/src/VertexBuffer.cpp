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
void VertexBuffer::UpdateBD(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT MiscFlags, UINT StructureByteStride)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = usage;
	m_BD.ByteWidth = bytewidth;
	m_BD.BindFlags = bindflags;
	m_BD.CPUAccessFlags = cpuaccessflags;
}

void VertexBuffer::UpdateInitData(void* vertex)
{
	ZeroMemory(&m_InitData, sizeof(m_InitData));
	m_InitData.pSysMem = vertex;
}
#endif