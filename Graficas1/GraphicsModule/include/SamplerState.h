#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
#include <vector>

using std::vector;

class SamplerState
{
public:
	SamplerState();
	~SamplerState();

	#if defined(DX11)
	inline ID3D11SamplerState** GetDXSamplerStateAddress() { return m_vSamplers.data(); }
	inline ID3D11SamplerState** GetLastElementAddress() { return &m_vSamplers.back(); }
	inline void AddSampler() { ID3D11SamplerState* NewSamp = NULL; m_vSamplers.push_back(NewSamp); m_SampNum++; }
	inline void AddDesc() { D3D11_SAMPLER_DESC Desc; m_vSampDesc.push_back(Desc); }

	inline int GetSamplerNum() { return m_SampNum; }
	inline ID3D11SamplerState* GetSampler(const int _i) { return m_vSamplers[_i]; }
	inline ID3D11SamplerState** GetSamplerAddress(const int _i) { return &m_vSamplers[_i]; }
	void SetDesc(const bool _Value, const int _i);

	inline D3D11_SAMPLER_DESC GetDXSamplerDesc(const int _i) { return m_vSampDesc[_i]; }
	inline D3D11_SAMPLER_DESC* GetDXSamplerDescAddress(const int _i) { return &m_vSampDesc[_i]; }
	#endif
private:

#if defined (DX11)
	vector<ID3D11SamplerState*> m_vSamplers;	
	vector<D3D11_SAMPLER_DESC> m_vSampDesc;

	int m_SampNum;
#endif
};

