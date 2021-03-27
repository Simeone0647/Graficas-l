#include "ConstantBuffer.h"
#include "test.h"

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
void ConstantBuffer::UpdateBD(GraphicsModule::UpdateBDStruct _BDStruct)
{
	ZeroMemory(&m_BD, sizeof(m_BD));
	m_BD.Usage = (D3D11_USAGE)_BDStruct.Usage;
	m_BD.ByteWidth = _BDStruct.ByteWidth;
	m_BD.BindFlags = _BDStruct.BindFlags;
	m_BD.CPUAccessFlags = _BDStruct.CPUAccessFlags;
}
#endif