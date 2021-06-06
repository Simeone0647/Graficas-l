#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#endif

namespace GraphicsModule
{
	struct SetRasterizerStruct;
}

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	#if defined(DX11)
	void SetRasterizerDesc(GraphicsModule::SetRasterizerStruct _RasterizerStruct);
	D3D11_RASTERIZER_DESC* GetRSDescAddress() { return &m_RasterStateDesc; }
	ID3D11RasterizerState** GetRSAddress() { return &m_RasterState; }
	ID3D11RasterizerState* GetRS() { return m_RasterState; }
	#endif
private:

	#if defined(DX11)
	D3D11_RASTERIZER_DESC m_RasterStateDesc;
	ID3D11RasterizerState* m_RasterState;
	#endif
};

