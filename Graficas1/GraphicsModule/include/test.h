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
#include "Mesh.h"
#include "Vertex.h"


//#include "Manager.h"
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
namespace GraphicsModule
{
//---------------------------STRUCTS------------------------------------

	struct SimpleVertex
	{
	#if defined(DX11)
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
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
		D3D_DRIVER_TYPE driverType;
		UINT CreateDeviceFlags;
		D3D_FEATURE_LEVEL FeatureLevels[3];
		UINT NumFeatureLevels;
		UINT SDKVersion;
		DXGI_SWAP_CHAIN_DESC sd;
		D3D_FEATURE_LEVEL FeatureLevel;
	#endif
	};

	class test
	{
	public:
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
		RenderTargetView					g_SecondSimeRenderTargetView;
		RenderTargetView					g_ThirdSimeRenderTargetView;
		RenderTargetView					g_SimeSecondCubeRTV;
		Camera                              m_PerspectiveCamera;
		Camera                              m_OrtographicCamera;
		Camera*								m_Camera;
		Vertex                              CubeVertex[24];
		Mesh                                FirstCube;
		Mesh                                SecondCube;
		Mesh                                ThirdCube;
		Mesh                                FourthCube;
		LPPOINT                             MouseInitPos = new POINT;
		LPPOINT                             MouseFinalPos = new POINT;
		LPPOINT                             MouseDirection = new POINT;
		unsigned short* CubeVertexIndex =   new unsigned short[36];
		bool                                m_IsPerspectiveActive = true;
		bool                                m_IsFirstFrame = false;
		bool                                m_IsFirstPosStored = false;

	#if defined(DX11)
		HINSTANCE                           g_hInst = NULL;
		HWND                                g_hWnd = NULL;
		D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
		ID3D11ShaderResourceView*			g_pDepthStencilSRV = NULL;
		ID3D11VertexShader*					g_pVertexShader = NULL;
		ID3D11PixelShader*					g_pPixelShader = NULL;
		ID3D11InputLayout*					g_pVertexLayout = NULL;
		ID3D11ShaderResourceView*			g_pTextureRV = NULL;
		ID3D11ShaderResourceView*			g_pTextureRVViejoSabroso = NULL;
		ID3D11ShaderResourceView*			g_pViewRT2 = NULL;
		ID3D11ShaderResourceView*			g_pViewRT3 = NULL;
		ID3D11ShaderResourceView*			g_pViewRT4 = NULL;
		ID3D11SamplerState*					g_pSamplerLinear = NULL;
		XMMATRIX                            g_World;
		XMMATRIX                            g_View;
		XMMATRIX                            g_Projection;
		XMMATRIX                            g_Translation;
		XMFLOAT4                            g_vMeshColor;
		ID3D11RasterizerState*				g_Rasterizer = NULL;

	#endif

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