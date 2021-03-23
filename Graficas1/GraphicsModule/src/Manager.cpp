#include "Manager.h"
#include "test.h"

namespace GraphicsModule
{
	Manager::Manager()
	{
	}
	
	Manager::~Manager()
	{
	}
	
#if defined(DX11)
	HRESULT Manager::CreateDeviceAndSwapChain(CreateDevAndSCDesc deviceandswapchaindesc)
	{
		D3D_FEATURE_LEVEL DXFeatureLevels[3];
		DXFeatureLevels[0] = (D3D_FEATURE_LEVEL)deviceandswapchaindesc.FeatureLevels[0];
		DXFeatureLevels[1] = (D3D_FEATURE_LEVEL)deviceandswapchaindesc.FeatureLevels[1];
		DXFeatureLevels[2] = (D3D_FEATURE_LEVEL)deviceandswapchaindesc.FeatureLevels[2];

		D3D_FEATURE_LEVEL DXFeatureLevel = (D3D_FEATURE_LEVEL)deviceandswapchaindesc.FeatureLevel;

		return D3D11CreateDeviceAndSwapChain(NULL, (D3D_DRIVER_TYPE)deviceandswapchaindesc.driverType, NULL, deviceandswapchaindesc.CreateDeviceFlags, DXFeatureLevels,
									  deviceandswapchaindesc.NumFeatureLevels, D3D11_SDK_VERSION, &deviceandswapchaindesc.sd, m_SwapChain.GetDXSCAddress(), m_Device.GetDXDeviceAddress(),
									  &DXFeatureLevel, m_DeviceContext.GetDXDCAddress());
	}
#endif
}