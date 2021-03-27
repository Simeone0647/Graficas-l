#include "ShaderResourceView.h"
#include "test.h"

ShaderResourceView::ShaderResourceView()
{
#if defined(DX11)
	m_SRV = NULL;
#endif
}

ShaderResourceView::~ShaderResourceView()
{
}

void ShaderResourceView::SetDesc(GraphicsModule::SIME_FORMAT _Format, GraphicsModule::SIME_SRV_DIMENSION _ViewDimension, UINT _Mips)
{
	ZeroMemory(&m_SRVDesc, sizeof(m_SRVDesc));
	m_SRVDesc.Format = (DXGI_FORMAT)_Format;
	m_SRVDesc.ViewDimension = (D3D_SRV_DIMENSION)_ViewDimension;
	m_SRVDesc.Texture2D.MipLevels = _Mips;
}
