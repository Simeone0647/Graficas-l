#include "Buffer.h"

Buffer::Buffer()
{
#if defined(DX11)
	m_pBuffer = NULL;
#endif
}

Buffer::~Buffer()
{
}

#if defined(DX11)
void Buffer::BUpdateBD(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT MiscFlags, UINT StructureByteStride)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = usage;
	m_BD.ByteWidth = bytewidth;
	m_BD.BindFlags = bindflags;
	m_BD.CPUAccessFlags = cpuaccessflags;
}
#endif