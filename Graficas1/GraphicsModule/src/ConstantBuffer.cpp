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
void ConstantBuffer::UpdateBD(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = usage;
	m_BD.ByteWidth = bytewidth;
	m_BD.BindFlags = bindflags;
	m_BD.CPUAccessFlags = cpuaccessflags;
}
#endif