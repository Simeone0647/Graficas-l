#pragma once

#include <windows.h>
#include "Buffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Viewport.h"
#include "Texture2D.h"
#include "DepthStencilView.h"
#include "Manager.h"
#include "RenderTargetView.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Vertex.h"
#include "InputLayout.h"
#include "ShaderReflection.h"
#include "SamplerState.h"
#include "ShaderResourceView.h"

#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
namespace GraphicsModule
{

	enum SIME_DRIVER_TYPE
	{
		SIME_DRIVER_TYPE_UNKNOWN = 0,
		SIME_DRIVER_TYPE_HARDWARE = (SIME_DRIVER_TYPE_UNKNOWN + 1),
		SIME_DRIVER_TYPE_REFERENCE = (SIME_DRIVER_TYPE_HARDWARE + 1),
		SIME_DRIVER_TYPE_NULL = (SIME_DRIVER_TYPE_REFERENCE + 1),
		SIME_DRIVER_TYPE_SOFTWARE = (SIME_DRIVER_TYPE_NULL + 1),
		SIME_DRIVER_TYPE_WARP = (SIME_DRIVER_TYPE_SOFTWARE + 1)
	};

	enum SIME_FEATURE_LEVEL
	{
		SIME_FEATURE_LEVEL_9_1 = 0x9100,
		SIME_FEATURE_LEVEL_9_2 = 0x9200,
		SIME_FEATURE_LEVEL_9_3 = 0x9300,
		SIME_FEATURE_LEVEL_10_0 = 0xa000,
		SIME_FEATURE_LEVEL_10_1 = 0xa100,
		SIME_FEATURE_LEVEL_11_0 = 0xb000
	};

//---------------------------STRUCTS------------------------------------

	struct SimpleVertex
	{
	#if defined(DX11)
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
		XMFLOAT3 Normal;
	#endif
	};

	struct DirLight
	{
	#if defined(DX11)
		XMFLOAT4 Dir;
	#endif
	};

	struct CBNeverChanges
	{
	#if defined(DX11)
		XMMATRIX mView;
	#endif
	};

	struct CBChangeOnResize
	{
	#if defined(DX11)
		XMMATRIX mProjection;
	#endif
	};

	struct CBChangesEveryFrame
	{
	#if defined(DX11)
		XMMATRIX mWorld;
		XMFLOAT4 vMeshColor;
	#endif
	};

	struct CreateDevAndSCDesc
	{
	#if defined(DX11)
		SIME_DRIVER_TYPE driverType;
		UINT CreateDeviceFlags;
		SIME_FEATURE_LEVEL FeatureLevels[3];
		UINT NumFeatureLevels;
		UINT SDKVersion;
		DXGI_SWAP_CHAIN_DESC sd;
		SIME_FEATURE_LEVEL FeatureLevel;
	#endif
	};



	class test
	{
	public:
		HWND                                g_hWnd = NULL;
		ShaderResourceView					g_SimeTextureRV;
		ShaderResourceView					g_SimeDepthStencilSRV;
		SamplerState						g_SimeSamplerState;
		PixelShader							g_SimePixelShader;
		ShaderReflection					g_SimeVertexShaderReflection;
		InputLayout							g_SimeInputLayout;
		VertexShader						g_SimeVertexShader;
		DirLight							g_DirLightBufferDesc;
		Buffer								g_DirLightBuffer;
		VertexBuffer						g_SimeVertexBuffer;
		IndexBuffer							g_SimeIndexBuffer;
		ConstantBuffer						g_SimeCBNeverChanges;
		ConstantBuffer						g_SimeCBChangeOnResize;
		ConstantBuffer						g_SimeCBChangesEveryFrame;
		Viewport							g_SimeViewport;
		Texture2D							g_SimeDepthStencil;
		Texture2D							g_TextureRenderTarget;
		Texture2D							g_TextureRenderTarget2;
		Texture2D							g_TextureRenderTarget3;
		CreateDevAndSCDesc					g_DeviceAndSwapChainDesc;
		DepthStencilView					g_SimeDepthStencilView;
		RenderTargetView					g_SimeRenderTargetView;
		SIME_DRIVER_TYPE                    g_driverType = SIME_DRIVER_TYPE_NULL;
		Camera                              m_PerspectiveCamera;
		Camera                              m_OrtographicCamera;
		Camera*								m_Camera;
		LPPOINT                             MouseInitPos = new POINT;
		LPPOINT                             MouseFinalPos = new POINT;
		LPPOINT                             MouseDirection = new POINT;
		unsigned int* CubeVertexIndex =   new unsigned int[36];
		bool                                m_IsPerspectiveActive = true;
		bool                                m_IsFirstFrame = false;
		bool                                m_IsFirstPosStored = false;
		bool								m_ShowingTexture = false;

	#if defined(DX11)
		HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	#endif
	public:
		HRESULT InitDevice(HWND hdwn);
	
		void Render();
	
		void Update();
		
		void CleanupDevice();

		HWND m_hwnd;
	};

	extern Manager& GetManagerObj(HWND hwnd);
}