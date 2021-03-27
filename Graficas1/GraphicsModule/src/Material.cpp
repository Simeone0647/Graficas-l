#include "Material.h"
#include "Mesh.h"

Material::Material()
{
	m_ID = 0;
}

Material::~Material()
{
}

void Material::Render(HWND _hwnd, GraphicsModule::test& _obj, CBChangesEveryFrame& cb)
{
#if defined(DX11)
	//m_ColorTexture = _obj.g_pTextureRV;
	GraphicsModule::UpdateSubResourceStruct UpdateSRStruct;
	UpdateSRStruct.pDstResource = _obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &cb;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = _obj.g_DirLightBuffer.BGetBuffer();
	UpdateSRStruct.pSrcData = &_obj.g_DirLightBufferDesc;

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, _obj.g_SimeTextureRV.GetDXSRVAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetSamplers(0, 1, _obj.g_SimeSamplerState.GetDXSamplerStateAddress());
#endif
}
