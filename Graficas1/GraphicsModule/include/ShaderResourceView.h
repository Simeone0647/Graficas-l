#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
#include <vector>

using std::vector;

namespace GraphicsModule
{
	enum SIME_FORMAT;
	enum SIME_SRV_DIMENSION;
}

class ShaderResourceView
{
public:
	ShaderResourceView();
	~ShaderResourceView();

#if defined(DX11)
	void SetDesc(GraphicsModule::SIME_FORMAT _Format, GraphicsModule::SIME_SRV_DIMENSION _ViewDimension, UINT _Mips);

	inline ID3D11ShaderResourceView* GetDXSRV() { return m_SRV; }
	inline ID3D11ShaderResourceView** GetDXSRVAddress() { return &m_SRV; }

	inline D3D11_SHADER_RESOURCE_VIEW_DESC* GetDXSRVDescAddress() { return& m_SRVDesc; }
	inline D3D11_SHADER_RESOURCE_VIEW_DESC GetDXSRVDesc() { return m_SRVDesc; }
#endif

private:
#if defined(DX11)
	ID3D11ShaderResourceView* m_SRV;
	D3D11_SHADER_RESOURCE_VIEW_DESC m_SRVDesc;
#endif
};

