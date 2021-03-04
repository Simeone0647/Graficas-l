#include "Device.h"

Device::Device()
{
#if defined(DX11)
	m_Device = NULL;
#endif
}

Device::~Device()
{

}
#if defined(DX11)
HRESULT Device::CCreateRenderTargetView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
										__out_opt  ID3D11RenderTargetView** ppRTView)
{
	return m_Device->CreateRenderTargetView(pResource, NULL, ppRTView);
}

HRESULT Device::CCreateTexture2D(__in const D3D11_TEXTURE2D_DESC* pDesc,
								__in_xcount_opt(pDesc -> MipLevels* pDesc -> ArraySize) const D3D11_SUBRESOURCE_DATA* pInitialData,
								__out_opt ID3D11Texture2D** ppTexture2D)
{
	return m_Device->CreateTexture2D(pDesc, NULL, ppTexture2D);;
}

HRESULT Device::CCreateDepthStencilView(__in ID3D11Resource* pResource, __in_opt const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
										 __out_opt ID3D11DepthStencilView** ppDepthStencilView)
{
	return m_Device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

HRESULT Device::CCreateVertexShader(__in const void* pShaderBytecode, __in SIZE_T BytecodeLength, __in_opt ID3D11ClassLinkage* pClassLinkage,
									 __out_opt ID3D11VertexShader** ppVertexShader)
{
	return m_Device->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
}

HRESULT Device::CCreateInputLayout(__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
									__in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
									__in  const void* pShaderBytecodeWithInputSignature,
									__in  SIZE_T BytecodeLength,
									__out_opt  ID3D11InputLayout** ppInputLayout)
{
	return m_Device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

HRESULT Device::CCreatePixelShader(__in  const void* pShaderBytecode, __in  SIZE_T BytecodeLength, __in_opt  ID3D11ClassLinkage* pClassLinkage,
									__out_opt  ID3D11PixelShader** ppPixelShader)
{
	return m_Device->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
}

HRESULT Device::CCreateBuffer(__in  const D3D11_BUFFER_DESC* pDesc, __in_opt  const D3D11_SUBRESOURCE_DATA* pInitialData,
							   __out_opt  ID3D11Buffer** ppBuffer)
{
	return m_Device->CreateBuffer(pDesc, pInitialData, ppBuffer);
}

HRESULT Device::CCreateSamplerState(__in  const D3D11_SAMPLER_DESC* pSamplerDesc, __out_opt  ID3D11SamplerState** ppSamplerState)
{
	return m_Device->CreateSamplerState(pSamplerDesc, ppSamplerState);
}

HRESULT Device::CCreateShaderResourceView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, __out_opt  ID3D11ShaderResourceView** ppSRView)
{
	return m_Device->CreateShaderResourceView(pResource, pDesc, ppSRView);
}

HRESULT Device::CCreateRasterizerState(__in  const D3D11_RASTERIZER_DESC* pRasterizerDesc, __out_opt  ID3D11RasterizerState** ppRasterizerState)
{
	return m_Device->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
}
#endif