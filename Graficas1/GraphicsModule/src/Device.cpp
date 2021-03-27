#include "Device.h"
#include "test.h"

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

HRESULT Device::CCreateVertexShader(GraphicsModule::CreateVertexShaderStruct _VSStruct)
{
	return m_Device->CreateVertexShader(_VSStruct.pShaderBytecode, _VSStruct.BytecodeLength, _VSStruct.pClassLinkage, _VSStruct.ppVertexShader);
}

HRESULT Device::CCreateInputLayout(GraphicsModule::CreateInputLayoutStruct _ILStruct)
{
	return m_Device->CreateInputLayout(_ILStruct.pInputElementDescs, _ILStruct.NumElements, _ILStruct.pShaderBytecodeWithInputSignature, _ILStruct.BytecodeLength, _ILStruct.ppInputLayout);
}

HRESULT Device::CCreatePixelShader(GraphicsModule::CreatePixelShaderStruct _PSStruct)
{
	return m_Device->CreatePixelShader(_PSStruct.pShaderBytecode, _PSStruct.BytecodeLength, _PSStruct.pClassLinkage, _PSStruct.ppPixelShader);
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