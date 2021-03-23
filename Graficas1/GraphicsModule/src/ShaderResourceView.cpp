#include "ShaderResourceView.h"

ShaderResourceView::ShaderResourceView()
{
#if defined(DX11)
	m_SRV = NULL;
#endif
}

ShaderResourceView::~ShaderResourceView()
{
}

void ShaderResourceView::SetDesc(DXGI_FORMAT _Format, D3D_SRV_DIMENSION _ViewDimension, UINT _Mips)
{
	ZeroMemory(&m_SRVDesc, sizeof(m_SRVDesc));
	m_SRVDesc.Format = _Format;
	m_SRVDesc.ViewDimension = _ViewDimension;
	m_SRVDesc.Texture2D.MipLevels = _Mips;
}
