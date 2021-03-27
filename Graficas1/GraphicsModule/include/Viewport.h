#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

namespace GraphicsModule
{
	struct InitViewportStruct;
}

class Viewport
{
public:
	Viewport();
	~Viewport();

	/*
		* @Function Name: InitViewport
		* @brief...This function will init the viewport.
		* @param...#float: Screen width.
		* @param...#float: Screen height.
		* @param...#float: Screen minimum depth.
		* @param...#float: Screen maximum depth.
		* @param...#float: Screen top left X.
		* @param...#float: Screen top left y.
		* @bug.....No known bugs.
	*/
	void InitViewport(GraphicsModule::InitViewportStruct _InitVP);

#if defined(DX11)
	/*
		* @Function Name: GetViewportAddress
		* @brief...This function will return the Viewport's address.
		* @bug.....No known bugs.
		* @return..#D3D11_VIEWPORT*: Viewport's address
	*/
	inline D3D11_VIEWPORT* GetViewportAddress() { return &m_VP; }

	/*
		* @Function Name: GetViewport
		* @brief...This function will return the Viewport object.
		* @bug.....No known bugs.
		* @return..#D3D11_VIEWPORT: Viewport object
	*/
	inline D3D11_VIEWPORT GetViewport() { return m_VP; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_VP
		* @Type....#D3D11_VIEWPORT
		* @brief...The DX11 Viewport object.
	*/
	D3D11_VIEWPORT m_VP;
#endif
};

