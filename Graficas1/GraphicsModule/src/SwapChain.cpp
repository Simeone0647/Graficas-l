#include "SwapChain.h"

SwapChain::SwapChain()
{
#if defined(DX11)
	m_SwapChain = NULL;
#endif
}

SwapChain::~SwapChain()
{
}

#if defined(DX11)
HRESULT SwapChain::CGetBuffer(UINT Buffer, __in  REFIID riid, __out  void** ppSurface)
{
	return m_SwapChain->GetBuffer(Buffer, riid, ppSurface);
}

void SwapChain::CPresent(UINT SyncInterval, UINT Flags)
{
	m_SwapChain->Present(SyncInterval, Flags);
}
#endif