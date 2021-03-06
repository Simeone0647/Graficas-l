#pragma once
#include "Texture2D.h"
class RenderTargetView : public Texture2D
{
public:
	RenderTargetView();
	~RenderTargetView();

#if defined(DX11)
	/*
		* @Function Name: GetRTV
		* @brief...This function will return the DX11 RTV.
		* @bug.....No known bugs.
		* @return..#ID3D11RenderTargetView*: DX11 RTV.
	*/
	inline ID3D11RenderTargetView* GetRTV() { return m_pRenderTargetView; }

	/*
		* @Function Name: GetRTVAdress
		* @brief...This function will return the DX RTV address.
		* @bug.....No known bugs.
		* @return..#ID3D11RenderTargetView**: DX11 RTV address.
	*/
	inline ID3D11RenderTargetView** GetRTVAdress() { return &m_pRenderTargetView; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_pRenderTargetView.
		* @Type....#ID3D11RenderTargetView*.
		* @brief...DX11 RTV.
	*/
	ID3D11RenderTargetView* m_pRenderTargetView;
#endif
};

