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