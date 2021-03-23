#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

#if defined(DX11)
	/*
		* @Function Name: SetDescDepth
		* @brief...This function will set the Texture2D descriptor.
		* @param...#UINT: Texture width.
		* @param...#UINT: Texture height.
		* @param...#UINT: Mip levels.
		* @param...#UINT: Array size in bytes.
		* @param...#DXGI_FORMAT: The format that the texture will have.
		* @param...#UINT: .
		* @param...#UINT: .
		* @param...#D3D11_USAGE: How the texture will be used.
		* @param...#UINT: CPU access flags.
		* @param...#UINT: Miscellaneous flags.
		* @bug.....No known bugs.
	*/
	void SetDescDepth(UINT width, UINT height, UINT miplevels, UINT arraysize, DXGI_FORMAT format, UINT count, UINT quality, D3D11_USAGE usage, UINT cpuaccessflags, UINT miscflags);
	
	/*
		* @Function Name: SetDescRT
		* @brief...This function will set the RenderTarget descriptor.
		* @param...#UINT: RenderTarget width.
		* @param...#UINT: RenderTarget height.
		* @param...#UINT: Mip levels.
		* @param...#UINT: Array size in bytes.
		* @param...#DXGI_FORMAT: The format that the RT will have.
		* @param...#UINT: .
		* @param...#UINT: .
		* @param...#D3D11_USAGE: How the texture will be used.
		* @param...#UINT: CPU access flags.
		* @param...#UINT: Miscellaneous flags.
		* @bug.....No known bugs.
	*/
	void SetDescRT(UINT width, UINT height, UINT miplevels, UINT arraysize, DXGI_FORMAT format, UINT count, UINT quality, D3D11_USAGE usage, UINT cpuaccessflags, UINT miscflags);

	/*
		* @Function Name: SetDescDepth
		* @brief...This function will return the Texture2D descriptor address.
		* @bug.....No known bugs.
		* @return..#D3D11_TEXTURE2D_DESC*: Texture2DDesc address.
	*/
	inline D3D11_TEXTURE2D_DESC* GetDescDepthAddress() { return &m_descDepth; }

	/*
		* @Function Name: GetTexture
		* @brief...This function will return the Texture2D object.
		* @bug.....No known bugs.
		* @return..#ID3D11Texture2D*: Texture2D object.
	*/
	inline ID3D11Texture2D* GetTexture() { return m_pTexture; }

	/*
		* @Function Name: GetTextureAddress
		* @brief...This function will return the Texture2D address.
		* @bug.....No known bugs.
		* @return..#ID3D11Texture2D**: Texture2D address.
	*/
	inline ID3D11Texture2D** GetTextureAddress() { return &m_pTexture; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_pTexture
		* @Type....#ID3D11Texture2D*
		* @brief...DX11 2DTexture.
	*/
	ID3D11Texture2D* m_pTexture;

	/*
		* @Variable Name: m_descDepth
		* @Type....#D3D11_TEXTURE2D_DESC*
		* @brief...DX11 2DTexture Descriptor.
	*/
	D3D11_TEXTURE2D_DESC m_descDepth;
#endif
};

