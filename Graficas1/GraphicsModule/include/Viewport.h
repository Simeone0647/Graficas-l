#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
class Viewport
{
public:
	Viewport();
	~Viewport();

	void InitViewport(float width, float height, float mindepth, float maxdepth, float topleftx, float toplefty);

#if defined(DX11)
	inline D3D11_VIEWPORT* GetViewportAdress() { return &m_vp; }
	inline D3D11_VIEWPORT GetViewport() { return m_vp; }
#endif
private:
#if defined(DX11)
	D3D11_VIEWPORT m_vp;
#endif
};

