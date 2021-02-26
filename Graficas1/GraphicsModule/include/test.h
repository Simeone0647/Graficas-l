#pragma once

#include <windows.h>
#include "Buffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "Viewport.h"
#include "Texture2D.h"
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
	#if defined(DX11)
		HINSTANCE                           g_hInst = NULL;
		HWND                                g_hWnd = NULL;
		D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
		ID3D11Device*						g_pd3dDevice = NULL;
		ID3D11DeviceContext*				g_pImmediateContext = NULL;
		IDXGISwapChain*						g_pSwapChain = NULL;
		ID3D11RenderTargetView*				g_pRenderTargetView = NULL;
		//ID3D11Texture2D*					g_pDepthStencil = NULL;
		ID3D11DepthStencilView*				g_pDepthStencilView = NULL;
		ID3D11ShaderResourceView*			g_pDepthStencilSRV = NULL;
		ID3D11VertexShader*					g_pVertexShader = NULL;
		ID3D11PixelShader*					g_pPixelShader = NULL;
		ID3D11InputLayout*					g_pVertexLayout = NULL;
		ID3D11ShaderResourceView*			g_pTextureRV = NULL;
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
		HRESULT InitDevice(HWND hdwn);
	
		void Render();
	
		void CleanupDevice();

		HWND m_hwnd;
	};
}