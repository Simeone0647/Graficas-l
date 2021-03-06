#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
#if defined(DX11)
	m_pIndexBuffer = NULL;
#endif
}

IndexBuffer::~IndexBuffer()
{
}

#if defined(DX11)
void IndexBuffer::UpdateBD(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = usage;
	m_BD.ByteWidth = bytewidth;
	m_BD.BindFlags = bindflags;
	m_BD.CPUAccessFlags = cpuaccessflags;
}

void IndexBuffer::UpdateInitData(unsigned short* index)
{
	ZeroMemory(&m_InitData, sizeof(m_InitData));
	m_InitData.pSysMem = index;
}
#endif