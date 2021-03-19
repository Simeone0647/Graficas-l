#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif

class SamplerState
{
public:
	SamplerState();
	~SamplerState();

#if defined(DX11)
	inline ID3D11SamplerState* GetDXSamplerState() { return m_pSamplerLinear; }
	inline ID3D11SamplerState** GetDXSamplerStateAddress() { return &m_pSamplerLinear; }

	void SetDesc();

	inline D3D11_SAMPLER_DESC GetDXSamplerDesc() { return m_sampDesc; }
	inline D3D11_SAMPLER_DESC* GetDXSamplerDescAddress() { return &m_sampDesc; }
#endif
private:

#if defined (DX11)
	ID3D11SamplerState* m_pSamplerLinear;
	D3D11_SAMPLER_DESC m_sampDesc;
#endif
};

