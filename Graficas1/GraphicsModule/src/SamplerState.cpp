#include "SamplerState.h"

SamplerState::SamplerState()
{
#if defined(DX11)
	m_pSamplerLinear = NULL;
	m_SampNum = 0;
#endif
}

SamplerState::~SamplerState()
{
}

#if defined(DX11)
void SamplerState::SetDesc(const bool _Value)
{
	ZeroMemory(&m_sampDesc, sizeof(m_sampDesc));
	if (_Value)
	{
		m_sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	else
	{ 
		m_sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	m_sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_sampDesc.MinLOD = 0;
	m_sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
}
#endif
