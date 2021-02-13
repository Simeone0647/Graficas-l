#include "CDeviceContext.h"

CDeviceContext::CDeviceContext()
{
	m_DeviceContext = NULL;
}

CDeviceContext::~CDeviceContext()
{
}

void CDeviceContext::CClearRenderTargetView(__in  ID3D11RenderTargetView* pRenderTargetView, __in  const FLOAT ColorRGBA[4])
{
	m_DeviceContext->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

void CDeviceContext::CClearDepthStencilView(__in  ID3D11DepthStencilView* pDepthStencilView, __in  UINT ClearFlags, __in  FLOAT Depth,
											__in  UINT8 Stencil)
{
	m_DeviceContext->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

void CDeviceContext::CUpdateSubresource(__in  ID3D11Resource* pDstResource, __in  UINT DstSubresource, __in_opt  const D3D11_BOX* pDstBox,
										__in  const void* pSrcData, __in  UINT SrcRowPitch, __in  UINT SrcDepthPitch)
{
	m_DeviceContext->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

void CDeviceContext::COMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
										 __in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
										 __in_opt  ID3D11DepthStencilView* pDepthStencilView)
{
	m_DeviceContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void CDeviceContext::CRSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
									 __in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports)
{
	m_DeviceContext->RSSetViewports(NumViewports, pViewports);
}

void CDeviceContext::CIASetInputLayout(__in_opt  ID3D11InputLayout* pInputLayout)
{
	m_DeviceContext->IASetInputLayout(pInputLayout);
}

void CDeviceContext::CIASetVertexBuffers(__in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
										 __in_range(0, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumBuffers,
										 __in_ecount(NumBuffers)  ID3D11Buffer* const* ppVertexBuffers,
										 __in_ecount(NumBuffers)  const UINT* pStrides,
										 __in_ecount(NumBuffers)  const UINT* pOffsets)
{
	m_DeviceContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void CDeviceContext::CIASetIndexBuffer(__in_opt  ID3D11Buffer* pIndexBuffer, __in  DXGI_FORMAT Format, __in  UINT Offset)
{
	m_DeviceContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void CDeviceContext::CIASetPrimitiveTopology(__in  D3D11_PRIMITIVE_TOPOLOGY Topology)
{
	m_DeviceContext->IASetPrimitiveTopology(Topology);
}

void CDeviceContext::CVSSetShader(__in_opt  ID3D11VertexShader* pVertexShader,
								   __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
								   UINT NumClassInstances)
{
	m_DeviceContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void CDeviceContext::CVSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
										   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
										   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers)
{
	m_DeviceContext->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void CDeviceContext::CPSSetShader(__in_opt  ID3D11PixelShader* pPixelShader,
								  __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
								  UINT NumClassInstances)
{
	m_DeviceContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void CDeviceContext::CPSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
										   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
										   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers)
{
	m_DeviceContext->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void CDeviceContext::CPSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
										   __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
										   __in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	m_DeviceContext->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void CDeviceContext::CPSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
								   __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
								   __in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers)
{
	m_DeviceContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

void CDeviceContext::CDrawIndexed(__in  UINT IndexCount, __in  UINT StartIndexLocation, __in  INT BaseVertexLocation)
{
	m_DeviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}