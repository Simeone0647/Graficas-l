#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

enum class TEXTURE_BIND_FLAG
{
	kBIND_VERTEX_BUFFER = 0x1L,
	kBIND_INDEX_BUFFER = 0x2L,
	kBIND_CONSTANT_BUFFER = 0x4L,
	kBIND_SHADER_RESOURCE = 0x8L,
	kBIND_STREAM_OUTPUT = 0x10L,
	kBIND_RENDER_TARGET = 0x20L,
	kBIND_DEPTH_STENCIL = 0x40L,
	kBIND_UNORDERED_ACCESS = 0x80L
};

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

#if defined(DX11)
	void SetDescDepth(UINT width, UINT height, UINT miplevels, UINT arraysize, DXGI_FORMAT format, UINT count, UINT quality, D3D11_USAGE usage, UINT cpuaccessflags, UINT miscflags);
	void SetDescRT(UINT width, UINT height, UINT miplevels, UINT arraysize, DXGI_FORMAT format, UINT count, UINT quality, D3D11_USAGE usage, UINT cpuaccessflags, UINT miscflags);

	inline D3D11_TEXTURE2D_DESC* GetDescDepthAdress() { return &m_descDepth; }

	inline ID3D11Texture2D* GetTexture() { return m_pTexture; }
	inline ID3D11Texture2D** GetTextureAdress() { return &m_pTexture; }
#endif
private:
#if defined(DX11)
	ID3D11Texture2D* m_pTexture;
	D3D11_TEXTURE2D_DESC m_descDepth;
#endif
};

