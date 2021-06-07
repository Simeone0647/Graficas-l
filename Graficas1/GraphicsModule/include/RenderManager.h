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

		void SetGBufferToneRTV();

		void SetGBufferCopyRTV();

		void SetForwardLightRTV();

		void SetForwardToneMapRTV();

		inline void SetBackBufferCleaned(bool _value) { m_BackBufferCleaned = _value; }
		inline bool IsBackBufferCleaned() { return m_BackBufferCleaned; }
		Texture2D GBufferTextures[6];

		ShaderResourceView GBufferSRV[6];

		RenderTargetView BackBufferRTV;
		RenderTargetView GBufferRTV;

		Texture2D DepthStencil;
		DepthStencilView DSView;
		ShaderResourceView DepthStencilSRV;

		RasterizerState GBufferRasterState;
		RasterizerState GBufferLightRasterState;

		SamplerState vGBufferSamplers;

		RenderTargetView GBufferToneMapRTV;
		RasterizerState ToneRasterizer;

		RenderTargetView GBufferCopyRTV;
		RasterizerState CopyRasterizer;

		RenderTargetView ForwardLightRTV;
		Texture2D ForwardTextures[2];
		ShaderResourceView ForwardSRV[2];
		SamplerState ForwardSamplers;

		RenderTargetView ForwardToneMapRTV;

	private:
		bool m_BackBufferCleaned;
	};

	extern RenderManager& GetRenderManager();
}

