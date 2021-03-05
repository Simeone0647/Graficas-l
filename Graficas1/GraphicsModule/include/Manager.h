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