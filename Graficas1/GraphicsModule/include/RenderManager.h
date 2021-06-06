#pragma once
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "DepthStencilView.h"
#include "RasterizerState.h"
#include "SamplerState.h"

namespace RM
{ 
	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();


		void SetBackBuffer();

		void SetGBufferRTV();

		inline void SetBackBufferCleaned(bool _value) { m_BackBufferCleaned = _value; }
		inline bool IsBackBufferCleaned() { return m_BackBufferCleaned; }
		Texture2D GBufferTextures[4];

		ShaderResourceView GBufferSRV[4];

		RenderTargetView BackBufferRTV;
		RenderTargetView GBufferRTV;

		Texture2D DepthStencil;
		DepthStencilView DSView;
		ShaderResourceView DepthStencilSRV;

		RasterizerState GBufferRasterState;
		RasterizerState GBufferLightRasterState;

		SamplerState vGBufferSamplers;

	private:
		bool m_BackBufferCleaned;
	};

	extern RenderManager& GetRenderManager();
}

