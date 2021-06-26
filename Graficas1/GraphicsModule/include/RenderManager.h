#pragma once
#include "RenderTargetView.h"
#include "ShaderResourceView.h"
#include "DepthStencilView.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "Model.h"

using std::vector;

namespace RM
{ 
	struct UpdateProjectionMatrixStruct
	{
		float AngleY;
		float Ratio;
		float NearPlane;
		float FarPlane;
		float Width;
		float Height;
	};

	struct AmbientOcclusion
	{
		float Radius;
		float Scale;
		float Bias;
		float Intensity;
		int Iterations = 1;
		int Unused1 = 0;
		int Unused2 = 0;
		int Unused3 = 0;
	};

	struct Ambient
	{
		#if defined(DX11)
		XMFLOAT4 kAmbient;
		XMFLOAT4 AmbientColor;
		#endif
		#if defined(OGL)
		float kAmbient[4];
		float AmbientColor[4];
		#endif
	};

	struct Specular
	{
		#if defined(DX11)
		XMFLOAT4 kSpecular;
		XMFLOAT4 Shininess = { 0.01f, 0.0f, 0.0f, 0.0f };
		#endif
		#if defined(OGL)
		float kSpecular[4];
		float Shininess[4] = { 0.01f, 0.0f, 0.0f, 0.0f };
		#endif
	};

	struct Diffuse
	{
		#if defined(DX11)
		XMFLOAT4 kDiffuse;
		#endif
		#if defined(OGL)
		float kDiffuse[4];
		#endif
	};

	struct DirLight
	{
		#if defined(DX11)
		XMFLOAT4 Dir;
		XMFLOAT4 Color;
		#endif
		#if defined(OGL)
		float Dir[4];
		float Color[4];
		#endif
	};

	struct PointLight
	{
		#if defined(DX11)
		XMFLOAT4 Position;
		XMFLOAT4 Color;
		XMFLOAT4 Attenuation;
		#endif
		#if defined(OGL)
		float Position[4];
		float Color[4];
		float Attenuation[4];
		#endif
	};

	struct SpotLight
	{
		#if defined(DX11)
		XMFLOAT4 Dir;
		XMFLOAT4 Pos;
		XMFLOAT4 Color;
		#endif
		#if defined(OGL)
		float Dir[4];
		float Pos[4];
		float Color[4];
		#endif
		float Attenuation;
		float InnerRadius;
		float OuterRadius;
		float Unused;
	};

	struct CameraFront
	{
		#if defined(DX11)
		XMFLOAT4 Front;
		#endif
		#if defined(OGL)
		float Front[4];
		#endif
	};

	struct Expossure
	{
		#if defined(DX11)
		XMFLOAT4 Expo = { 0.01f, 0.0f, 0.0f, 0.0f };
		#endif
		#if defined(OGL)
		float Expo[4] = { 0.01f, 0.0f, 0.0f, 0.0f };
		#endif
	};

	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		void SetRenderTarget(const float _ClearColor[], RenderTargetView& _RenderTarget, DepthStencilView& _DepthStencil);

		inline void SetBackBufferCleaned(bool _Value) { m_BackBufferCleaned = _Value; }
		inline bool IsBackBufferCleaned() { return m_BackBufferCleaned; }
		void SetLights();
		void SetVP();
		void SetViewPos();
		void SetExpossure();
		void SetSSAOValues();
		//------------------ General --------------------------------
		RenderTargetView BackBufferRTV;
		
		Texture2D DepthStencil;
		DepthStencilView DSView;
		ShaderResourceView DepthStencilSRV;

		RenderTargetView SkyboxRTV;
		Texture2D SkyboxTex;
		ShaderResourceView SkyboxSRV;
		SamplerState SkyboxSam;

		Texture2D SkyboxTexResource;
		ShaderResourceView SkyboxSRVResource;
		RasterizerState SkyboxRaster;

		Texture2D DiffSkyBoxTexResource;
		ShaderResourceView DiffSkyBoxSRVResource;

		//---------- DEFERRED -----------
		RenderTargetView GBufferRTV;
		Texture2D GBufferTextures[4];
		ShaderResourceView GBufferSRV[4];
		SamplerState GBufferSamplers;
		RasterizerState GBufferRasterState;

		RenderTargetView DefToneMapRTV;
		Texture2D DefToneMapTex;
		ShaderResourceView DefToneMapSRV;
		SamplerState DefToneMapSamplers;
		RasterizerState ToneRasterizer;

		RenderTargetView DefCopyRTV;
		Texture2D DefCopyTex;
		ShaderResourceView DefCopySRV;
		SamplerState DefCopySamplers;
		RasterizerState CopyRasterizer;

		RenderTargetView DefSSAORTV;
		Texture2D DefSSAOTex;
		ShaderResourceView DefSSAOSRV;
		SamplerState DefSSAOSamplers;
	
		RasterizerState GBufferLightRasterState;

		//------------ FORWARD --------------
		RenderTargetView ForwardLightRTV;
		RenderTargetView ForwardToneMapRTV;
		Texture2D ForwardTextures[2];
		ShaderResourceView ForwardSRV[2];

		SamplerState ForwardSamplers;

		std::vector<Model> m_vModels;

		#if defined(OGL)
		unsigned int SkyboxTexResourceOGL;

		//FORWARD
		unsigned int FrameBuffer1;
		unsigned int FrameBuffer2;
		unsigned int FrameBuffer3;

		unsigned int DepthBuffer1;
		unsigned int DepthBuffer2;

		unsigned int SkyboxTexOGL;
		unsigned int LightTexOGL;

		//DEF

		unsigned int AlbedoFB;
		unsigned int AlbedoDepth;
		unsigned int AlbedoTex;

		unsigned int NormalFB;
		unsigned int NormalDepth;
		unsigned int NormalTex;

		unsigned int SpecularFB;
		unsigned int SpecularDepth;
		unsigned int SpecularTex;

		unsigned int PositionFB;
		unsigned int PositionDepth;
		unsigned int PositionTex;

		unsigned int DefSkyboxFB;
		unsigned int DefSkyboxDepth;
		unsigned int DefSkyboxTex;

		unsigned int SSAOFB;
		unsigned int SSAODepth;
		unsigned int SSAOTex;

		unsigned int ToneFB;
		unsigned int ToneDepth;
		unsigned int ToneTex;

		unsigned int CopyFB;
		unsigned int CopyDepth;
		unsigned int CopyTex;
		#endif

		AmbientOcclusion	g_AO;
		Buffer				g_AOBuffer;
		Buffer				g_ExpossureBuffer;
		Expossure			g_Expossure;
		CameraFront			g_Front;
		Buffer				g_CameraFrontBuffer;
		Buffer				g_AmbientBuffer;
		Buffer				g_SpecularBuffer;
		Buffer				g_DiffuseBuffer;
		Ambient				g_Ambient;
		Specular			g_Specular;
		Diffuse				g_Diffuse;
		DirLight			g_DirLightBufferDesc;
		Buffer				g_DirLightBuffer;
		PointLight			g_PointLightBufferDesc;
		Buffer				g_PointLightBuffer;
		SpotLight			g_SpotLightBufferDesc;
		Buffer				g_SpotLightBuffer;

		Camera              m_PerspectiveCamera;
		Camera              m_OrtographicCamera;
		Camera*				m_Camera;
	private:
		bool m_BackBufferCleaned;
	};

	extern RenderManager& GetRenderManager();
}

