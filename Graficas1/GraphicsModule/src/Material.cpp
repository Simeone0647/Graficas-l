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
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(_obj.g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(_obj.g_DirLightBuffer.BGetBuffer(), 0, NULL, &_obj.g_DirLightBufferDesc, 0, 0);
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, _obj.g_SimeTextureRV.GetDXSRVAddress());
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetSamplers(0, 1, _obj.g_SimeSamplerState.GetDXSamplerStateAddress());
#endif
}
