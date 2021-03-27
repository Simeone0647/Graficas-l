#include "DeviceContext.h"
#include "test.h"

DeviceContext::DeviceContext()
{
#if defined(DX11)
	m_DeviceContext = NULL;
#endif
}

DeviceContext::~DeviceContext()
{
}
#if defined(DX11)
void DeviceContext::CClearRenderTargetView(__in  ID3D11RenderTargetView* pRenderTargetView, __in  const FLOAT ColorRGBA[4])
{
	m_DeviceContext->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

void DeviceContext::CClearDepthStencilView(GraphicsModule::ClearDepthStencilViewStruct _ClearDSVStruct)
{
	m_DeviceContext->ClearDepthStencilView(_ClearDSVStruct.pDepthStencilView, _ClearDSVStruct.ClearFlags, _ClearDSVStruct.Depth, _ClearDSVStruct.Stencil);
}

void DeviceContext::CUpdateSubresource(GraphicsModule::UpdateSubResourceStruct _USRStruct)
{
	m_DeviceContext->UpdateSubresource(_USRStruct.pDstResource, _USRStruct.DstSubresource, _USRStruct.pDstBox, _USRStruct.pSrcData, _USRStruct.SrcRowPitch, _USRStruct.SrcDepthPitch);
}

void DeviceContext::COMSetRenderTargets(__in_range(0, D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)  UINT NumViews,
										 __in_ecount_opt(NumViews)  ID3D11RenderTargetView* const* ppRenderTargetViews,
										 __in_opt  ID3D11DepthStencilView* pDepthStencilView)
{
	m_DeviceContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void DeviceContext::CRSSetState(__in_opt  ID3D11RasterizerState* pRasterizerState)
{
	m_DeviceContext->RSSetState(pRasterizerState);
}

void DeviceContext::CRSSetViewports(__in_range(0, D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE)  UINT NumViewports,
									 __in_ecount_opt(NumViewports)  const D3D11_VIEWPORT* pViewports)
{
	m_DeviceContext->RSSetViewports(NumViewports, pViewports);
}

void DeviceContext::CIASetInputLayout(__in_opt  ID3D11InputLayout* pInputLayout)
{
	m_DeviceContext->IASetInputLayout(pInputLayout);
}

void DeviceContext::CIASetVertexBuffers(GraphicsModule::SetVertexBufferStruct _SetVBStruct)
{
	m_DeviceContext->IASetVertexBuffers(_SetVBStruct.StartSlot, _SetVBStruct.NumBuffers, _SetVBStruct.ppVertexBuffers, _SetVBStruct.pStrides, _SetVBStruct.pOffsets);
}

void DeviceContext::CIASetIndexBuffer(__in_opt  ID3D11Buffer* pIndexBuffer, __in  GraphicsModule::SIME_FORMAT Format, __in  UINT Offset)
{
	m_DeviceContext->IASetIndexBuffer(pIndexBuffer, (DXGI_FORMAT)Format, Offset);
}

void DeviceContext::CIASetPrimitiveTopology(__in  GraphicsModule::SIME_PRIMITIVE_TOPOLOGY Topology)
{
	m_DeviceContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)Topology);
}

void DeviceContext::CVSSetShader(__in_opt  ID3D11VertexShader* pVertexShader,
								   __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
								   UINT NumClassInstances)
{
	m_DeviceContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
}

void DeviceContext::CVSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
										   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
										   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers)
{
	m_DeviceContext->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void DeviceContext::CPSSetShader(__in_opt  ID3D11PixelShader* pPixelShader,
								  __in_ecount_opt(NumClassInstances)  ID3D11ClassInstance* const* ppClassInstances,
								  UINT NumClassInstances)
{
	m_DeviceContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
}

void DeviceContext::CPSSetConstantBuffers(__in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - 1)  UINT StartSlot,
										   __in_range(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT - StartSlot)  UINT NumBuffers,
										   __in_ecount(NumBuffers)  ID3D11Buffer* const* ppConstantBuffers)
{
	m_DeviceContext->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

void DeviceContext::CPSSetShaderResources(__in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1)  UINT StartSlot,
										   __in_range(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - StartSlot)  UINT NumViews,
										   __in_ecount(NumViews)  ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
	m_DeviceContext->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void DeviceContext::CPSSetSamplers(__in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - 1)  UINT StartSlot,
								   __in_range(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT - StartSlot)  UINT NumSamplers,
								   __in_ecount(NumSamplers)  ID3D11SamplerState* const* ppSamplers)
{
	m_DeviceContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

void DeviceContext::CDrawIndexed(__in  UINT IndexCount, __in  UINT StartIndexLocation, __in  INT BaseVertexLocation)
{
	m_DeviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}
#endif