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
void IndexBuffer::UpdateBd(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride)
{
	ZeroMemory(&m_bd, sizeof(m_bd));
	m_bd.Usage = usage;
	m_bd.ByteWidth = bytewidth;
	m_bd.BindFlags = bindflags;
	m_bd.CPUAccessFlags = cpuaccessflags;
}

void IndexBuffer::UpdateInitData(unsigned short* index)
{
	ZeroMemory(&m_InitData, sizeof(m_InitData));
	m_InitData.pSysMem = index;
}
#endif