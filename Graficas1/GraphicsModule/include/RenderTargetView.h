#pragma once
#include "Texture2D.h"
#include <vector>

using std::vector;

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
	//inline ID3D11RenderTargetView* GetRTV() { return m_pRenderTargetView; }

	/*
		* @Function Name: GetRTVAdress
		* @brief...This function will return the DX RTV address.
		* @bug.....No known bugs.
		* @return..#ID3D11RenderTargetView**: DX11 RTV address.
	*/
	//inline ID3D11RenderTargetView** GetRTVAdress() { return &m_pRenderTargetView; }
	inline ID3D11RenderTargetView** GetRTVAdress() { return m_vRTVs.data(); }

	inline ID3D11RenderTargetView** GetLastElementAddress() { return &m_vRTVs.back(); }

	inline void AddRTV() { ID3D11RenderTargetView* NewRTV = NULL; m_vRTVs.push_back( NewRTV );  m_RTVNum++; }

	inline ID3D11RenderTargetView* GetRTV(const int _i) { return m_vRTVs[_i]; }

	inline int GetRTVNum() { return m_RTVNum; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_pRenderTargetView.
		* @Type....#ID3D11RenderTargetView*.
		* @brief...DX11 RTV.
	*/
	ID3D11RenderTargetView* m_pRenderTargetView;
	
	vector<ID3D11RenderTargetView*> m_vRTVs;

	int m_RTVNum;
#endif
};

