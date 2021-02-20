#include "CSwapChain.h"

CSwapChain::CSwapChain()
{
	m_SwapChain = NULL;
}

CSwapChain::~CSwapChain()
{
}

HRESULT CSwapChain::CGetBuffer(UINT Buffer, __in  REFIID riid, __out  void** ppSurface)
{
	return m_SwapChain->GetBuffer(Buffer, riid, ppSurface);
}

void CSwapChain::CPresent(UINT SyncInterval, UINT Flags)
{
	m_SwapChain->Present(SyncInterval, Flags);
}

void CSwapChain::ResizeWindowPerspective(CCamera* camera, float angle, float ratio, float nearp, float farp)
{
	camera->UpdatePerspectiveProjectionMatrix(angle, ratio, nearp, farp);
}

void CSwapChain::ResizeWindowOrtographic(CCamera* camera, float width, float height, float nearp, float farp)
{
	camera->UpdateOrtographicProjectionMatrix(width, height, nearp, farp);
}
