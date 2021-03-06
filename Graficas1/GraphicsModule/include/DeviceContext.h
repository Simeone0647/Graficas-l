#pragma once
#if defined(DX11)
#include <d3d11.h>
#endif

class DeviceContext
{
public:
	DeviceContext();
	~DeviceContext();
#if defined(DX11)
	/*
		* @Function Name: CClearRenderTargetView
		* @brief...Clear a RTV.
		* @param...#ID3D11RenderTargetView*: RTV to clear.
		* @param...#FLOAT: Color which will clear the RTV.
		* @bug.....No known bugs.
	*/
	void CClearRenderTargetView(__in  ID3D11RenderTargetView* pRenderTargetView, __in  const FLOAT ColorRGBA[4]);

	/*
		* @Function Name: CClearDepthStencilView
		* @brief...Clear a DSV.
		* @param...#ID3D11DepthStencilView*: DSV to clear.
		* @param...#UINT: 
		* @param...#FLOAT:
		* @param...#UINT8:
		* @bug.....No known bugs.
	*/
	void CClearDepthStencilView(__in  ID3D11DepthStencilView* pDepthStencilView, __in  UINT ClearFlags, __in  FLOAT Depth, __in  UINT8 Stencil);


	/*
		* @Function Name: CRSSetViewports
		* @brief...Set a viewport.
		* @param...#UINT: Number of viewports.
		* @param...#D3D11_VIEWPORT*: Viewport's address
		* @bug.....No known bugs.
	*/
	void CRSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
						 __in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports);

	/*
		* @Function Name: CIASetInputLayout
		* @brief...Set a InputLayout.
		* @param...#ID3D11InputLayout*: InputLayout's Address.
		* @bug.....No known bugs.
	*/
	void CIASetInputLayout(__in_opt  ID3D11InputLayout* pInputLayout);

	/*
		* @Function Name: CIASetVertexBuffers
		* @brief...Set a VertexBuffer
		* @param...#UINT: .
		* @param...#UINT:
		* @param...#ID3D11Buffer: Buffer's Address
		* @param...#UINT*:
		* @param...#UINT*:
		* @bug.....No known bugs.
	*/
	void CIASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
							 __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
							 __in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers,
							 __in_ecount(NumBuffers)  const UINT* pStrides,
							 __in_ecount(NumBuffers)  const UINT* pOffsets);

	/*
		* @Function Name: CIASetIndexBuffer
		* @brief...Set a IndexBuffer
		* @param...#ID3D11Buffer*: IndexBuffer's address 
		* @param...#DXGI_FORMAT: 
		* @param...#UINT:
		* @bug.....No known bugs.
	*/
	void CIASetIndexBuffer(__in_opt  ID3D11Buffer* pIndexBuffer, __in  DXGI_FORMAT Format, __in  UINT Offset);

	/*
		* @Function Name: CIASetPrimitiveTopology
		* @brief...Set the topology
		* @param...#D3D11_PRIMITIVE_TOPOLOGY: Topology to use
		* @bug.....No known bugs.
	*/
	void CIASetPrimitiveTopology(__in  D3D11_PRIMITIVE_TOPOLOGY Topology);

	/*
		* @Function Name: CVSSetShader
		* @brief...Set a Shader
		* @param...#ID3D11VertexShader*: VertexShader object
		* @param...#ID3D11ClassInstance**: ClassInstanceAddress
		* @param...#UINT: Number of class instances
		* @bug.....No known bugs.
	*/
	void CVSSetShader(__in_opt  ID3D11VertexShader* pVertexShader,
					   __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
					   UINT NumClassInstances);

	/*
		* @Function Name: CVSSetConstantBuffers
		* @brief...Set a ConstantBuffer
		* @param...#UINT: 
		* @param...#UINT: Number of buffers
		* @param...#ID3D11Buffer**: Buffer's address 
		* @bug.....No known bugs.
	*/
	void CVSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
							   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
							   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

	/*
		* @Function Name: CPSSetShader
		* @brief...Set a Pixel Shader
		* @param...#ID3D11PixelShader*: PixelShader object.
		* @param...#ID3D11ClassInstance**: Class Instance address.
		* @param...#UINT: Number of class instances.
		* @bug.....No known bugs.
	*/
	void CPSSetShader(__in_opt  ID3D11PixelShader* pPixelShader, __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
					  UINT NumClassInstances);

	/*
		* @Function Name: CPSSetConstantBuffers
		* @brief...Set a PixelShader Constant Buffer
		* @param...#UINT: 
		* @param...#UINT: Number of buffers.
		* @param...#ID3D11Buffer**: Buffer's address.
		* @bug.....No known bugs.
	*/
	void CPSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
							   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
							   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

	/*
		* @Function Name: CPSSetShaderResources
		* @brief...Set a Pixel Shader Resources
		* @param...#UINT: 
		* @param...#UINT:
		* @param...#ID3D11ShaderResourceView**: Shader Resource View address.
		* @bug.....No known bugs.
	*/
	void CPSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
							   __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
							   __in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews);

	/*
		* @Function Name: CPSSetSamplers
		* @brief...Set a Pixel Shader Sampler
		* @param...#UINT:
		* @param...#UINT: Number of samplers
		* @param...#ID3D11SamplerState**: Sampler's address.
		* @bug.....No known bugs.
	*/
	void CPSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
					   __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
					   __in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers);

	/*
		* @Function Name: CUpdateSubresource
		* @brief...Update a subresource
		* @param...#ID3D11Resource*:
		* @param...#UINT:
		* @param...#D3D11_BOX*:
		* @param...#void*:
		* @param...#UINT:
		* @param...#UINT:
		* @bug.....No known bugs.
	*/
	void CUpdateSubresource(__in  ID3D11Resource* pDstResource, __in  UINT DstSubresource, __in_opt  const D3D11_BOX* pDstBox,
							__in  const void* pSrcData, __in  UINT SrcRowPitch, __in  UINT SrcDepthPitch);

	/*
		* @Function Name: CDrawIndexed
		* @brief...Draws.
		* @param...#UINT:
		* @param...#UINT:
		* @param...#INT: 
		* @bug.....No known bugs.
	*/
	void CDrawIndexed(__in  UINT IndexCount, __in  UINT StartIndexLocation, __in  INT BaseVertexLocation);

	/*
		* @Function Name: COMSetRenderTargets
		* @brief...Seta a RenderTarget
		* @param...#UINT:
		* @param...#ID3D11RenderTargetView**: Render Target's address
		* @param...#ID3D11DepthStencilView*: DepthStencilView object.
		* @bug.....No known bugs.
	*/
	void COMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
							 __in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
							 __in_opt  ID3D11DepthStencilView* pDepthStencilView);

	/*
		* @Function Name: CRSSetState
		* @brief...Set a Rasterizer State
		* @param...#ID3D11RasterizerState*: RasterizerState object.
		* @bug.....No known bugs.
	*/
	void CRSSetState(__in_opt  ID3D11RasterizerState* pRasterizerState);

	/*
		* @Function Name: GetDXDCAddress
		* @brief...Returns the DX DeviceContext's address
		* @bug.....No known bugs.
		* @return..#ID3D11DeviceContext**: DX11 DeviceContext's address.  
	*/
	inline ID3D11DeviceContext** GetDXDCAddress() { return &m_DeviceContext; }

	/*
		* @Function Name: GetDXDC
		* @brief...Returns DX DeviceContext
		* @bug.....No known bugs.
		* @return..#ID3D11DeviceContext*: DX11 DeviceContext.  
	*/
	inline ID3D11DeviceContext* GetDXDC() { return m_DeviceContext; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_DeviceContext
		* @Type....#ID3D11DeviceContext*
		* @brief...DX11 DeviceContext
	*/
	ID3D11DeviceContext* m_DeviceContext;	
#endif
};

