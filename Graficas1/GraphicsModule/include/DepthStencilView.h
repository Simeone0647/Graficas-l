#pragma once
#include "Texture2D.h"
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

namespace GraphicsModule
{ 
	enum SIME_FORMAT;
	enum SIME_DSV_DIMENSION;
}

class DepthStencilView : public Texture2D
{
public:
	DepthStencilView();
	~DepthStencilView();

#if defined(DX11)
	/*
		* @Function Name: SetDescDSV
		* @brief...This function will set the DSV's descriptor.
		* @param...#DXGI_FORMAT: Descriptor format.
		* @param...#D3D11_DSV_DIMENSION: The texture dimension
		* @param...#UINT: 
		* @bug.....No known bugs.
	*/
	void SetDescDSV(GraphicsModule::SIME_FORMAT _Format, GraphicsModule::SIME_DSV_DIMENSION _ViewDimension, UINT _Mipslice);

	/*
		* @Function Name: GetDescDSVAddress
		* @brief...This function will return the DSVDescriptor Address.
		* @bug.....No known bugs.
		* @return..#D3D11_DEPTH_STENCIL_VIEW_DESC*: DSVDescriptor's adress.
	*/
	inline D3D11_DEPTH_STENCIL_VIEW_DESC* GetDescDSVAddress() { return &m_descDSV; }

	/*
		* @Function Name: GetDSV
		* @brief...This function will return the DSV object.
		* @bug.....No known bugs.
		* @return..#ID3D11DepthStencilView*: DSV object.
	*/
	inline ID3D11DepthStencilView* GetDSV() { return m_pDepthStencilView; }

	/*
		* @Function Name: GetDSVAddress
		* @brief...This function will return the DSV Address.
		* @bug.....No known bugs.
		* @return..#ID3D11DepthStencilView**: DSV's address.
	*/
	inline ID3D11DepthStencilView** GetDSVAddress() { return &m_pDepthStencilView; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_pDepthStencilView
		* @Type....#ID3D11DepthStencilView*
		* @brief...DX11 DepthStencilView.
	*/
	ID3D11DepthStencilView* m_pDepthStencilView;

	/*
		* @Variable Name: m_descDSV
		* @Type....#D3D11_DEPTH_STENCIL_VIEW_DESC
		* @brief...DSV Descriptor.
	*/
	D3D11_DEPTH_STENCIL_VIEW_DESC m_descDSV;
#endif
};

