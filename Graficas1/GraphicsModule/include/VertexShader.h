#pragma once
#if defined(DX11)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#endif
class VertexShader
{
public:
	VertexShader();
	~VertexShader();

#if defined(DX11)
	inline ID3D11VertexShader* GetDXVertexShader() { return m_pVertexShader; }
	inline ID3D11VertexShader** GetDXVertexShaderAddress() { return &m_pVertexShader; }
#endif

private:
#if defined(DX11)
	ID3D11VertexShader* m_pVertexShader;
#endif
};

