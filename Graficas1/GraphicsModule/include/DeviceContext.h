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
	void CClearRenderTargetView(__in  ID3D11RenderTargetView* pRenderTargetView, __in  const FLOAT ColorRGBA[4]);

	void CClearDepthStencilView(__in  ID3D11DepthStencilView* pDepthStencilView, __in  UINT ClearFlags, __in  FLOAT Depth, __in  UINT8 Stencil);

	void CRSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
						 __in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports);

	void CIASetInputLayout(__in_opt  ID3D11InputLayout* pInputLayout);

	void CIASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
							 __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
							 __in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers,
							 __in_ecount(NumBuffers)  const UINT* pStrides,
							 __in_ecount(NumBuffers)  const UINT* pOffsets);

	void CIASetIndexBuffer(__in_opt  ID3D11Buffer* pIndexBuffer, __in  DXGI_FORMAT Format, __in  UINT Offset);

	void CIASetPrimitiveTopology(__in  D3D11_PRIMITIVE_TOPOLOGY Topology);

	void CVSSetShader(__in_opt  ID3D11VertexShader* pVertexShader,
					   __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
					   UINT NumClassInstances);

	void CVSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
							   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
							   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

	void CPSSetShader(__in_opt  ID3D11PixelShader* pPixelShader, __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
					  UINT NumClassInstances);

	void CPSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
							   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
							   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers);

	void CPSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
							   __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
							   __in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews);

	void CPSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
					   __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
					   __in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers);

	void CUpdateSubresource(__in  ID3D11Resource* pDstResource, __in  UINT DstSubresource, __in_opt  const D3D11_BOX* pDstBox,
							__in  const void* pSrcData, __in  UINT SrcRowPitch, __in  UINT SrcDepthPitch);

	void CDrawIndexed(__in  UINT IndexCount, __in  UINT StartIndexLocation, __in  INT BaseVertexLocation);

	void COMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
							 __in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
							 __in_opt  ID3D11DepthStencilView* pDepthStencilView);

	void CRSSetState(__in_opt  ID3D11RasterizerState* pRasterizerState);

	inline ID3D11DeviceContext** GetDXDCAdress() { return &m_DeviceContext; }
	inline ID3D11DeviceContext* GetDXDC() { return m_DeviceContext; }
#endif
private:
#if defined(DX11)
	ID3D11DeviceContext* m_DeviceContext;	
#endif
};

