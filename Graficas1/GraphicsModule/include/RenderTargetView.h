#pragma once
#include "Texture2D.h"
class RenderTargetView : public Texture2D
{
public:
	RenderTargetView();
	~RenderTargetView();

#if defined(DX11)
	inline ID3D11RenderTargetView* GetRTV() { return m_pRenderTargetView; }
	inline ID3D11RenderTargetView** GetRTVAdress() { return &m_pRenderTargetView; }
#endif
private:
#if defined(DX11)
	ID3D11RenderTargetView* m_pRenderTargetView;
#endif
};

