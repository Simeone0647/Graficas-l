#pragma once
#include "Texture2D.h"
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

class DepthStencilView : public Texture2D
{
public:
	DepthStencilView();
	~DepthStencilView();

#if defined(DX11)
	void SetDescDSV(DXGI_FORMAT format, D3D11_DSV_DIMENSION viewdimension, UINT mipslice);

	inline D3D11_DEPTH_STENCIL_VIEW_DESC* GetDescDSVAdress() { return &m_descDSV; }

	inline ID3D11DepthStencilView* GetDSV() { return m_pDepthStencilView; }
	inline ID3D11DepthStencilView** GetDSVAdress() { return &m_pDepthStencilView; }

#endif
private:
#if defined(DX11)
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D11_DEPTH_STENCIL_VIEW_DESC m_descDSV;
#endif
};

