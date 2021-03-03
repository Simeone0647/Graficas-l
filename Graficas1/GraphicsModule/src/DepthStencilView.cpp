#include "DepthStencilView.h"

DepthStencilView::DepthStencilView()
{
#if defined(DX11)
	m_pDepthStencilView = NULL;
#endif
}

DepthStencilView::~DepthStencilView()
{
}

#if defined(DX11)
void DepthStencilView::SetDescDSV(DXGI_FORMAT format, D3D11_DSV_DIMENSION viewdimension, UINT mipslice)
{
	ZeroMemory(&m_descDSV, sizeof(m_descDSV));
	m_descDSV.Format = format;
	m_descDSV.ViewDimension = viewdimension;
	m_descDSV.Texture2D.MipSlice = mipslice;
}
#endif
