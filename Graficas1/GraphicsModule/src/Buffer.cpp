#include "Buffer.h"
#include "test.h"

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
void Buffer::BUpdateBD(GraphicsModule::UpdateBDStruct _BDStruct)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = (D3D11_USAGE)_BDStruct.Usage;
	m_BD.ByteWidth = _BDStruct.ByteWidth;
	m_BD.BindFlags = _BDStruct.BindFlags;
	m_BD.CPUAccessFlags = _BDStruct.CPUAccessFlags;
}
#endif