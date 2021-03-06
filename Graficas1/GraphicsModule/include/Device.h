#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

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

class Device
{
public:
	Device();
	~Device();
#if defined(DX11)
	/*
		* @Function Name: CCreateRenderTargetView
		* @brief...This function will create a RenderTargetView.
		* @param...#ID3D11Resource*: 
		* @param...#D3D11_RENDER_TARGET_VIEW_DESC*: RTVDescriptor's address.
		* @param...#ID3D11RenderTargetView**: RTV Address.
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateRenderTargetView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
								   __out_opt  ID3D11RenderTargetView** ppRTView);

	/*
		* @Function Name: CCreateTexture2D
		* @brief...This function will create a Texture2D.
		* @param...#D3D11_TEXTURE2D_DESC*: Texture2DDescriptor's address
		* @param...#D3D11_SUBRESOURCE_DATA*: 
		* @param...#ID3D11Texture2D**: Texture2D's address.
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateTexture2D(__in  const D3D11_TEXTURE2D_DESC* pDesc,
							__in_xcount_opt(pDesc->MipLevels* pDesc->ArraySize)  const D3D11_SUBRESOURCE_DATA* pInitialData,
							__out_opt  ID3D11Texture2D** ppTexture2D);

	/*
		* @Function Name: CCreateDepthStencilView
		* @brief...This function will create a DSV.
		* @param...#ID3D11Resource*:
		* @param...#D3D11_DEPTH_STENCIL_VIEW_DESC*: DSVDescriptor's address.
		* @param...#ID3D11DepthStencilView**: DSV Address.
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateDepthStencilView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
									 __out_opt  ID3D11DepthStencilView** ppDepthStencilView);

	/*
		* @Function Name: CCreateVertexShader
		* @brief...This function will create a VertexShader.
		* @param...#void*:
		* @param...#SIZE_T: 
		* @param...#ID3D11ClassLinkage*: 
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateVertexShader(__in  const void* pShaderBytecode, __in  SIZE_T BytecodeLength, __in_opt  ID3D11ClassLinkage* pClassLinkage,
								__out_opt  ID3D11VertexShader** ppVertexShader);

	/*
		* @Function Name: CCreateInputLayout
		* @brief...This function will create a InputLayout.
		* @param...#D3D11_INPUT_ELEMENT_DESC*: InputElementDescriptor's Address
		* @param...#UINT: Number of elements.
		* @param...#void*: RTV Address.
		* @param...#SIZE_T:
		* @param...#ID3D11InputLayout**:
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateInputLayout(__in_ecount(NumElements)  const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
							   __in_range(0, D3D11_IA_VERTEX_INPUT_STRUCTURE_ELEMENT_COUNT)  UINT NumElements,
							   __in  const void* pShaderBytecodeWithInputSignature,
							   __in  SIZE_T BytecodeLength,
							   __out_opt  ID3D11InputLayout** ppInputLayout);

	/*
		* @Function Name: CCreatePixelShader
		* @brief...This function will create a PixelShader.
		* @param...#void*:
		* @param...#SIZE_T: 
		* @param...#ID3D11ClassLinkage*: 
		* @param...#ID3D11PixelShader**: PixelShader's Address
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreatePixelShader(__in  const void* pShaderBytecode, __in  SIZE_T BytecodeLength, __in_opt  ID3D11ClassLinkage* pClassLinkage,
							   __out_opt  ID3D11PixelShader** ppPixelShader);

	/*
		* @Function Name: CCreateBuffer
		* @brief...This function will create a Buffer.
		* @param...#D3D11_BUFFER_DESC*: BufferDescriptor's Address.
		* @param...#D3D11_SUBRESOURCE_DATA*: 
		* @param...#ID3D11Buffer**: Buffer address.
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateBuffer(__in  const D3D11_BUFFER_DESC* pDesc, __in_opt  const D3D11_SUBRESOURCE_DATA* pInitialData,
						  __out_opt  ID3D11Buffer** ppBuffer);

	/*
		* @Function Name: CCreateSamplerState
		* @brief...This function will create a SamplerState.
		* @param...#D3D11_SAMPLER_DESC*: SamplerDescriptor's Address
		* @param...#ID3D11SamplerState**: SamplerState's Address
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateSamplerState(__in  const D3D11_SAMPLER_DESC* pSamplerDesc, __out_opt  ID3D11SamplerState** ppSamplerState);

	/*
		* @Function Name: CCreateShaderResourceView
		* @brief...This function will create a ShaderResourceView.
		* @param...#ID3D11Resource*: 
		* @param...#D3D11_SHADER_RESOURCE_VIEW_DESC*: ShaderResourceViewDescriptor's Address.
		* @param...#ID3D11ShaderResourceView**: ShaderResourceView's Address.
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	HRESULT CCreateShaderResourceView(__in  ID3D11Resource* pResource, __in_opt  const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, 
													   __out_opt  ID3D11ShaderResourceView** ppSRView);
	/*
		* @Function Name: CCreateRasterizerState
		* @brief...This function will create a RasterizerState.
		* @param...#D3D11_RASTERIZER_DESC*: RasterizerDescriptor's Address.
		* @param...#1D3D11RasterizerState**: RasterizerState'Address.
		* @bug.....No known bugs.
		* @return..#HRESULT: Checks if the function was successfull.
	*/
	virtual HRESULT CCreateRasterizerState(__in  const D3D11_RASTERIZER_DESC* pRasterizerDesc, __out_opt  ID3D11RasterizerState** ppRasterizerState);

	/*
		* @Function Name: GetDXDevice
		* @brief...This function will return the DXDevice.
		* @bug.....No known bugs.
		* @return..#ID3D11Device*: DX11 Device.
	*/
	inline ID3D11Device* GetDXDevice() { return m_Device; }

	/*
		* @Function Name: GetDXDeviceAddress
		* @brief...This function will return the DXDevice's Address.
		* @bug.....No known bugs.
		* @return..#ID3D11Device**: DX11Device's Address.
	*/
	inline ID3D11Device** GetDXDeviceAddress() { return &m_Device; }
#endif
private:
#if defined(DX11)

	/*
		* @Variable Name: m_Device
		* @Type....#1D3D11Device*
		* @brief...DX11 Device
	*/
	ID3D11Device* m_Device;
#endif
};

