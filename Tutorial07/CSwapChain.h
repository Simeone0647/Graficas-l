#pragma once
#include "DXGI.h"
#include "CCamera.h"

class CSwapChain
{
public:

	CSwapChain();
	~CSwapChain();

	HRESULT CGetBuffer(UINT Buffer, __in  REFIID riid, __out  void** ppSurface);

	void CPresent(UINT SyncInterval, UINT Flags);

	inline IDXGISwapChain** GetSwapChainAdress() { return &m_SwapChain; }
	inline IDXGISwapChain* GetSwapChain() { return m_SwapChain; }

	void ResizeWindowPerspective(CCamera* camera, float angle, float ratio, float nearp, float farp);
	void ResizeWindowOrtographic(CCamera* camera, float width, float height, float nearp, float farp);

private:
	IDXGISwapChain* m_SwapChain;
};

