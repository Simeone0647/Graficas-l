#pragma once
#if defined(DX11)
#include "DXGI.h"
#endif
#include "Camera.h"

class SwapChain
{
public:

	SwapChain();
	~SwapChain();

#if defined(DX11)

	/*
		* @Function Name: CGetBuffer
		* @brief...This function will set a buffer.
		* @param...#UINT: 
		* @param...#REFIID:
		* @param...#void*: Value adress where the buffer wil be set.
		* @bug.....No known bugs.
	*/
	HRESULT CGetBuffer(UINT Buffer, __in  REFIID riid, __out  void** ppSurface);

	/*
		* @Function Name: CPresent
		* @brief...This function will present the objects to render.
		* @param...#UINT:
		* @param...#UINT:
		* @bug.....No known bugs.
	*/
	void CPresent(UINT SyncInterval, UINT Flags);

	/*
		* @Function Name: GetDXSCAddress
		* @brief...This function will return the DX SC address.
		* @bug.....No known bugs.
		* @return..#IDXGISwapChain**: DXSwapChain address.
	*/
	inline IDXGISwapChain** GetDXSCAddress() { return &m_SwapChain; }

	/*
		* @Function Name: GetDXSC
		* @brief...This function will return the DX SC object.
		* @bug.....No known bugs.
		* @return..#IDXGISwapChain*: DXSwapChain object.
	*/
	inline IDXGISwapChain* GetDXSC() { return m_SwapChain; }

#endif

private:
#if defined(DX11)
	/*
		* @Variable Name: m_SwapChain
		* @Type....#IDXGISwapChain*
		* @brief...DXSC object
	*/
	IDXGISwapChain* m_SwapChain;
#endif
};

