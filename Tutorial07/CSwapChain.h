#pragma once
#include "DXGI.h"

class CSwapChain
{
public:

	CSwapChain();
	~CSwapChain();

	HRESULT CGetBuffer(UINT Buffer, __in  REFIID riid, __out  void** ppSurface);

	void CPresent(UINT SyncInterval, UINT Flags);

	inline IDXGISwapChain** GetSwapChainAdress() { return &m_SwapChain; }
	inline IDXGISwapChain* GetSwapChain() { return m_SwapChain; }

private:
	IDXGISwapChain* m_SwapChain;
};

