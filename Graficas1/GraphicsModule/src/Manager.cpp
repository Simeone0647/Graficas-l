#include "Manager.h"
#include "test.h"

namespace GraphicsModule
{
	Manager::Manager()
	{
	#if defined(DX11)
		m_pd3dDevice = NULL;
		m_pImmediateContext = NULL;
		m_pSwapChain = NULL;
	#endif
	}
	
	Manager::~Manager()
	{
	}
	
#if defined(DX11)
	HRESULT Manager::CreateDeviceAndSwapChain(CreateDevAndSCDesc deviceandswapchaindesc)
	{
	
		return D3D11CreateDeviceAndSwapChain(NULL, deviceandswapchaindesc.driverType, NULL, deviceandswapchaindesc.CreateDeviceFlags, deviceandswapchaindesc.FeatureLevels,
									  deviceandswapchaindesc.NumFeatureLevels, D3D11_SDK_VERSION, &deviceandswapchaindesc.sd, &m_pSwapChain, &m_pd3dDevice,
									  &deviceandswapchaindesc.FeatureLevel, &m_pImmediateContext);

	}
#endif
}