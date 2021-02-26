#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{
#if defined(DX11)
	ID3D11Buffer* m_pCBNeverChanges = NULL;
	ID3D11Buffer* m_pCBChangeOnResize = NULL;
	ID3D11Buffer* m_pCBChangesEveryFrame = NULL;
#endif
}

ConstantBuffer::~ConstantBuffer()
{
}

#if defined(DX11)
void ConstantBuffer::UpdateBd(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride)
{
	ZeroMemory(&m_bd, sizeof(m_bd));
	m_bd.Usage = usage;
	m_bd.ByteWidth = bytewidth;
	m_bd.BindFlags = bindflags;
	m_bd.CPUAccessFlags = cpuaccessflags;
}
#endif