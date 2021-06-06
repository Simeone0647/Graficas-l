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
	
		/*
			* @Function Name: GetDevice
			* @brief...This function will return MY Device
			* @bug.....No known bugs.
			* @return..#Device: My Device
		*/
		inline Device GetDevice() { return m_Device; }

		/*
			* @Function Name: GetDeviceContext
			* @brief...This function will return MY DeviceContext
			* @bug.....No known bugs.
			* @return..#DeviceContext: My DeviceContext
		*/
		inline DeviceContext GetDeviceContext() { return m_DeviceContext; }

		/*
			* @Function Name: GetSwapChain
			* @brief...This function will return MY GetSwapChain
			* @bug.....No known bugs.
			* @return..#SwapChain: My SwapChain
		*/
		inline SwapChain GetSwapChain() { return m_SwapChain; }
	#if defined(DX11)
		/*
			* @Function Name: CreateDeviceAndSwapChain
			* @brief...This function will create the Device, DeviceContext and SwapChain
			* @bug.....No known bugs.
			* @return..#CVector3: The camera's Eye vector.
		*/
		HRESULT CreateDeviceAndSwapChain(CreateDevAndSCDesc deviceandswapchaindesc);
	#endif
		
	private:
		/*
			* @Variable Name: m_Device
			* @Type....#Device
			* @brief...This variable is MY Device. It will have and call different Devices and functions depending on which API is in use.
		*/
		Device m_Device;

		/*
			* @Variable Name: m_DeviceContext
			* @Type....#DeviceContext
			* @brief...This variable is MY DeviceContext. It will have and call different DevicesContext and functions depending on which API is in use.
		*/
		DeviceContext m_DeviceContext;

		/*
			* @Variable Name: m_SwapChain
			* @Type....#SwapChain
			* @brief...This variable is MY SwapChain. It will have and call different SwapChains and functions depending on which API is in use.
		*/
		SwapChain m_SwapChain;
	};
}