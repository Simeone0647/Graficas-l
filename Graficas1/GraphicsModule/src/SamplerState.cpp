#include "SamplerState.h"

SamplerState::SamplerState()
{
#if defined(DX11)
	m_SampNum = 0;
#endif
}

SamplerState::~SamplerState()
{
}

#if defined(DX11)
void SamplerState::SetDesc(const bool _Value, const int _i)
{
	ZeroMemory(&m_vSampDesc[_i], sizeof(m_vSampDesc[_i]));
	if (_Value)
	{
		m_vSampDesc[_i].Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	else
	{ 
		m_vSampDesc[_i].Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	m_vSampDesc[_i].AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_vSampDesc[_i].AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_vSampDesc[_i].AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_vSampDesc[_i].ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_vSampDesc[_i].MinLOD = 0;
	m_vSampDesc[_i].MaxLOD = D3D11_FLOAT32_MAX;
}
#endif
