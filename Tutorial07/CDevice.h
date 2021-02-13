#pragma once
#include <d3d11.h>

enum class SIME1_DRIVER_TYPE
{
	SIME1_UNKNOWN = 0,
	SIME1_HARDWARE = (SIME1_UNKNOWN + 1),
	SIME1_REFERENCE = (SIME1_HARDWARE + 1),
	SIME1_NULL = (SIME1_REFERENCE + 1),
	SIME1_SOFTWARE = (SIME1_NULL + 1),
	SIME1_WRAP = (SIME1_SOFTWARE + 1),
};

enum class SIME1_FEATURE_LEVEL
{
	SIME1_FEATURE_LEVEL_9_1 = 0x9100,
	SIME1_FEATURE_LEVEL_9_2 = 0x9200,
	SIME1_FEATURE_LEVEL_9_3 = 0x9300,
	SIME1_FEATURE_LEVEL_10_0 = 0xa000,
	SIME1_FEATURE_LEVEL_10_1 = 0xa100,
	SIME1_FEATURE_LEVEL_11_0 = 0xb000
};

class CDevice
{
public:
	CDevice();
	~CDevice();

	HRESULT CCreateRenderTargetView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
								   __out_opt  ID3D11RenderTargetView** ppRTView);

	HRESULT CCreateTexture2D(__in  const D3D11_TEXTURE2D_DESC* pDesc,
							__in_xcount_opt(pDesc->MipLevels* pDesc->ArraySize)  const D3D11_SUBRESOURCE_DATA* pInitialData,
							__out_opt  ID3D11Texture2D** ppTexture2D);

	HRESULT CCreateDepthStencilView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
									 __out_opt  ID3D11DepthStencilView** ppDepthStencilView);

	HRESULT CCreateVertexShader(__in  const void* pShaderBytecode, __in  SIZE_T BytecodeLength, __in_opt  ID3D11ClassLinkage* pClassLinkage,
								__out_opt  ID3D11VertexShader** ppVertexShader);

	HRESULT CCreateInputLayout(__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
							   __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
							   __in  const void* pShaderBytecodeWithInputSignature,
							   __in  SIZE_T BytecodeLength,
							   __out_opt  ID3D11InputLayout** ppInputLayout);

	HRESULT CCreatePixelShader(__in  const void* pShaderBytecode, __in  SIZE_T BytecodeLength, __in_opt  ID3D11ClassLinkage* pClassLinkage,
							   __out_opt  ID3D11PixelShader** ppPixelShader);

	HRESULT CCreateBuffer(__in  const D3D11_BUFFER_DESC* pDesc, __in_opt  const D3D11_SUBRESOURCE_DATA* pInitialData,
						  __out_opt  ID3D11Buffer** ppBuffer);

	HRESULT CCreateSamplerState(__in  const D3D11_SAMPLER_DESC* pSamplerDesc, __out_opt  ID3D11SamplerState** ppSamplerState);

	inline ID3D11Device* GetDevice() { return m_Device; }

	inline ID3D11Device** GetDeviceAdress() { return &m_Device; }
private:

	ID3D11Device* m_Device;
};

