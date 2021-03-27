#include "DepthStencilView.h"
#include "test.h"

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
void DepthStencilView::SetDescDSV(GraphicsModule::SIME_FORMAT _Format, GraphicsModule::SIME_DSV_DIMENSION _ViewDimension, UINT _Mipslice)
{
	ZeroMemory(&m_descDSV, sizeof(m_descDSV));
	m_descDSV.Format = (DXGI_FORMAT)_Format;
	m_descDSV.ViewDimension = (D3D11_DSV_DIMENSION)_ViewDimension;
	m_descDSV.Texture2D.MipSlice = _Mipslice;
}
#endif
