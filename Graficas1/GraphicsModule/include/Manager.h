#pragma once
#include "SwapChain.h"
#include "Device.h"
#include "DeviceContext.h"
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
	
		inline Device GetDevice() { return m_Device; }
		inline DeviceContext GetDeviceContext() { return m_DeviceContext; }
		inline SwapChain GetSwapChain() { return m_SwapChain; }

	#if defined(DX11)
		HRESULT CreateDeviceAndSwapChain(CreateDevAndSCDesc deviceandswapchaindesc);
	#endif
		
	private:
		Device m_Device;
		DeviceContext m_DeviceContext;
		SwapChain m_SwapChain;
	};
}