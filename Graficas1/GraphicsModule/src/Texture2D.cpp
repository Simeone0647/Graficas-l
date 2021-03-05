#include "Texture2D.h"

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
void Texture2D::SetDescDepth(UINT width, UINT height, UINT miplevels, UINT arraysize, DXGI_FORMAT format, UINT count, UINT quality, D3D11_USAGE usage, UINT cpuaccessflags, UINT miscflags)
{
	ZeroMemory(&m_descDepth, sizeof(m_descDepth));
	m_descDepth.Width = width;
	m_descDepth.Height = height;
	m_descDepth.MipLevels = miplevels;
	m_descDepth.ArraySize = arraysize;
	m_descDepth.Format = format;
	m_descDepth.SampleDesc.Count = count;
	m_descDepth.SampleDesc.Quality = quality;
	m_descDepth.Usage = usage;
	//m_descDepth.BindFlags = TEXTURE_BIND_FLAG::kBIND_DEPTH_STENCIL | TEXTURE_BIND_FLAG::kBIND_SHADER_RESOURCE;
	m_descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_descDepth.CPUAccessFlags = cpuaccessflags;
	m_descDepth.MiscFlags = miscflags;
}

void Texture2D::SetDescRT(UINT width, UINT height, UINT miplevels, UINT arraysize, DXGI_FORMAT format, UINT count, UINT quality, D3D11_USAGE usage, UINT cpuaccessflags, UINT miscflags)
{
	ZeroMemory(&m_descDepth, sizeof(m_descDepth));
	m_descDepth.Width = width;
	m_descDepth.Height = height;
	m_descDepth.MipLevels = miplevels;
	m_descDepth.ArraySize = arraysize;
	m_descDepth.Format = format;
	m_descDepth.SampleDesc.Count = count;
	m_descDepth.SampleDesc.Quality = quality;
	m_descDepth.Usage = usage;
	//m_descDepth.BindFlags = TEXTURE_BIND_FLAG::kBIND_DEPTH_STENCIL | TEXTURE_BIND_FLAG::kBIND_SHADER_RESOURCE;
	m_descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	m_descDepth.CPUAccessFlags = cpuaccessflags;
	m_descDepth.MiscFlags = miscflags;
}
#endif
