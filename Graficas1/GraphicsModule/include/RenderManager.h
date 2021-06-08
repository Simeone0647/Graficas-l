#pragma once
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "DepthStencilView.h"
#include "RasterizerState.h"
#include "SamplerState.h"
//#include <iostream>
//#include <fstream>  

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

		void SetDefSSAORTV();

		void SetDefSkyboxRTV();

		inline void SetBackBufferCleaned(bool _value) { m_BackBufferCleaned = _value; }
		inline bool IsBackBufferCleaned() { return m_BackBufferCleaned; }
		Texture2D GBufferTextures[7];

		ShaderResourceView GBufferSRV[7];

		RenderTargetView BackBufferRTV;

		RenderTargetView GBufferRTV;
		RenderTargetView GBufferToneMapRTV;
		RenderTargetView GBufferCopyRTV;
		RenderTargetView DefSSAORTV;

		Texture2D DepthStencil;
		DepthStencilView DSView;
		ShaderResourceView DepthStencilSRV;

		RasterizerState GBufferRasterState;
		RasterizerState GBufferLightRasterState;

		SamplerState vGBufferSamplers;

		
		RasterizerState ToneRasterizer;

		
		RasterizerState CopyRasterizer;

		RenderTargetView ForwardLightRTV;
		Texture2D ForwardTextures[2];
		ShaderResourceView ForwardSRV[2];
		SamplerState ForwardSamplers;

		RenderTargetView ForwardToneMapRTV;
		
		SamplerState DefSSAOSampler;

		RenderTargetView DefSkyboxRTV;
		Texture2D DefSkyboxTex;
		Texture2D DefSkyboxTexRTV;
		ShaderResourceView DefSkyboxSRV;
		ShaderResourceView DefSkyboxSRVOutput;
		SamplerState DefSkyboxSam;
		RasterizerState DefSkyboxRaster;
	private:
		bool m_BackBufferCleaned;
	};

	extern RenderManager& GetRenderManager();
}

