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
	HRESULT CGetBuffer(UINT Buffer, __in  REFIID riid, __out  void** ppSurface);

	void CPresent(UINT SyncInterval, UINT Flags);

	inline IDXGISwapChain** GetDXSCAdress() { return &m_SwapChain; }
	inline IDXGISwapChain* GetDXSC() { return m_SwapChain; }
#endif
	void ResizeWindowPerspective(Camera* camera, float angle, float ratio, float nearp, float farp);
	void ResizeWindowOrtographic(Camera* camera, float width, float height, float nearp, float farp);

private:
#if defined(DX11)
	IDXGISwapChain* m_SwapChain;
#endif
};

