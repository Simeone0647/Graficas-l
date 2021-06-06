#include "RenderManager.h"
#include "test.h"

namespace RM
{
	RenderManager::RenderManager()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			HWND hwnd = NULL;
			HRESULT hr;
			vGBufferSamplers.AddSampler();
			vGBufferSamplers.SetDesc();
			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(vGBufferSamplers.GetDXSamplerDescAddress(), vGBufferSamplers.GetLastElementAddress());
			if (FAILED(hr))
			{
				cout << "Fallo sampler" << endl;
			}
		}

		m_BackBufferCleaned = false;
	}
	
	RenderManager::~RenderManager()
	{
	}

	void RenderManager::SetBackBuffer()
	{
		HWND hwnd = NULL;
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		//float ClearColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
		GraphicsModule::ClearDepthStencilViewStruct ClearDSVStruct;
		ClearDSVStruct.pDepthStencilView = DSView.GetDSV();
		ClearDSVStruct.ClearFlags = GraphicsModule::SIME_CLEAR_DEPTH;
		ClearDSVStruct.Depth = 1.0f;
		ClearDSVStruct.Stencil = 0;

		//ID3D11RenderTargetView* rTargets[5] = { BackBufferRTV.GetRTV(0), NULL, NULL, NULL, NULL };

		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CClearRenderTargetView(BackBufferRTV.GetRTV(0), ClearColor);
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().COMSetRenderTargets(BackBufferRTV.GetRTVNum(), BackBufferRTV.GetRTVAdress(), DSView.GetDSV());
		//GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().COMSetRenderTargets(5, rTargets, DSView.GetDSV());
	}

	void RenderManager::SetGBufferRTV()
	{
		HWND hwnd = NULL;
		GraphicsModule::ClearDepthStencilViewStruct ClearDSVStruct;
		ClearDSVStruct.pDepthStencilView = DSView.GetDSV();
		ClearDSVStruct.ClearFlags = GraphicsModule::SIME_CLEAR_DEPTH;
		ClearDSVStruct.Depth = 1.0f;
		ClearDSVStruct.Stencil = 0;

		float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // red, green, blue, alpha

		for (unsigned int i = 0; i < RM::GetRenderManager().GBufferRTV.GetRTVNum(); ++i)
		{
			GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CClearRenderTargetView(GBufferRTV.GetRTV(i), ClearColor);
		}

		ID3D11ShaderResourceView* const pSRV[4] = { NULL, NULL, NULL, NULL };
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(0, 4, pSRV);

		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);
		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().COMSetRenderTargets(GBufferRTV.GetRTVNum(), GBufferRTV.GetRTVAdress(), DSView.GetDSV());
	}
	
	extern RenderManager& GetRenderManager()
	{
		static RenderManager* pRenderManager = nullptr;
		if (pRenderManager == nullptr)
		{
			pRenderManager = new RenderManager();
		}
		return *pRenderManager;
	}
}