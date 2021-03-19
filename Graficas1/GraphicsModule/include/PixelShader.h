#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
class PixelShader
{
public:
	PixelShader();
	~PixelShader();

#if defined(DX11)
	inline ID3D11PixelShader* GetDXPixelShader() { return m_pPixelShader; }

	inline ID3D11PixelShader** GetDXPixelShaderAddress() { return &m_pPixelShader; }
#endif

private:
#if defined(DX11)
	ID3D11PixelShader* m_pPixelShader;
#endif
};

