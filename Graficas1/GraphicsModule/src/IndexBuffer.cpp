#include "IndexBuffer.h"
#include "test.h"

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
void IndexBuffer::UpdateBD(GraphicsModule::UpdateBDStruct _UpdateBDStruct)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = (D3D11_USAGE)_UpdateBDStruct.Usage;
	m_BD.ByteWidth = _UpdateBDStruct.ByteWidth;
	m_BD.BindFlags = _UpdateBDStruct.BindFlags;
	m_BD.CPUAccessFlags = _UpdateBDStruct.CPUAccessFlags;
}

void IndexBuffer::UpdateInitData(unsigned int* index)
{
	ZeroMemory(&m_InitData, sizeof(m_InitData));
	m_InitData.pSysMem = index;
}
#endif