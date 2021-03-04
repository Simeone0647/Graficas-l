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
	
		return D3D11CreateDeviceAndSwapChain(NULL, deviceandswapchaindesc.driverType, NULL, deviceandswapchaindesc.CreateDeviceFlags, deviceandswapchaindesc.FeatureLevels,
									  deviceandswapchaindesc.NumFeatureLevels, D3D11_SDK_VERSION, &deviceandswapchaindesc.sd, m_SwapChain.GetDXSCAdress(), m_Device.GetDXDeviceAdress(),
									  &deviceandswapchaindesc.FeatureLevel, m_DeviceContext.GetDXDCAdress());

	}
#endif
}