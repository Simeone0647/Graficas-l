#include "Texture2D.h"
#include "test.h"

Texture2D::Texture2D()
{
#if defined(DX11)
	m_pTexture = NULL;
#endif
}

Texture2D::~Texture2D()
{
}

#if defined(DX11)
void Texture2D::SetDescDepth(GraphicsModule::CreateDepthDesc _DepthDesc)
{
	ZeroMemory(&m_descDepth, sizeof(m_descDepth));
	m_descDepth.Width = _DepthDesc.Width;
	m_descDepth.Height = _DepthDesc.Height;
	m_descDepth.MipLevels = _DepthDesc.Mips;
	m_descDepth.ArraySize = _DepthDesc.ArraySize;
	m_descDepth.Format = (DXGI_FORMAT)_DepthDesc.Format;
	m_descDepth.SampleDesc.Count = _DepthDesc.Count;
	m_descDepth.SampleDesc.Quality = _DepthDesc.Quality;
	m_descDepth.Usage = (D3D11_USAGE)_DepthDesc.Usage;
	m_descDepth.BindFlags = _DepthDesc.BindFlags;
	m_descDepth.CPUAccessFlags = _DepthDesc.CPUAccessFlags;
	m_descDepth.MiscFlags = _DepthDesc.MiscFlags;
}

void Texture2D::SetDescRT(GraphicsModule::SetRTDescStruct _RTDescStruct)
{
	ZeroMemory(&m_descDepth, sizeof(m_descDepth));
	m_descDepth.Width = _RTDescStruct.Width;
	m_descDepth.Height = _RTDescStruct.Height;
	m_descDepth.MipLevels = _RTDescStruct.MipLevels;
	m_descDepth.ArraySize = _RTDescStruct.Arraysize;
	m_descDepth.Format = (DXGI_FORMAT)_RTDescStruct.Format;
	m_descDepth.SampleDesc.Count = _RTDescStruct.Count;
	m_descDepth.SampleDesc.Quality = _RTDescStruct.Quality;
	m_descDepth.Usage = (D3D11_USAGE)_RTDescStruct.Usage;
	m_descDepth.BindFlags = _RTDescStruct.BindFlags;
	m_descDepth.CPUAccessFlags = _RTDescStruct.CPUAccessFlags;
	m_descDepth.MiscFlags = _RTDescStruct.MiscFlags;
}
#endif
