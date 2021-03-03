#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

//using namespace std;
//struct CreateDevAndSCDesc;
//{
//#if defined(DX11)
//	D3D_DRIVER_TYPE driverType;
//	UINT CreateDeviceFlags;
//	D3D_FEATURE_LEVEL FeatureLevels[3];
//	UINT NumFeatureLevels;
//	UINT SDKVersion;
//	DXGI_SWAP_CHAIN_DESC sd;
//	D3D_FEATURE_LEVEL FeatureLevel;
//#endif
//};
//struct GraphicsModule::CreateDevAndSCDesc;
//namespace GraphicsModule { struct CreateDevAndSCDesc; }
namespace GraphicsModule
{
	struct CreateDevAndSCDesc;
	struct CreateSDDesc;

	class Manager
	{
	public:
		Manager();
		~Manager();
	
	#if defined(DX11)
		ID3D11Device* GetDevice() { return m_pd3dDevice; }
		ID3D11Device** GetDeviceAdress() { return &m_pd3dDevice; }
	
		ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
		ID3D11DeviceContext** GetDeviceContextAdress() { return &m_pImmediateContext; }
	
		IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
		IDXGISwapChain** GetSwapChainAdress() { return &m_pSwapChain; }
		HRESULT CreateDeviceAndSwapChain(CreateDevAndSCDesc deviceandswapchaindesc);
	#endif
		
	private:
	#if defined(DX11)
		ID3D11Device* m_pd3dDevice;
		ID3D11DeviceContext* m_pImmediateContext;
		IDXGISwapChain* m_pSwapChain;
	#endif
	};
}