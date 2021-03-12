#include "ShaderReflection.h"

ShaderReflection::ShaderReflection()
{
#if defined(DX11)
	m_pShaderReflection = NULL;
#endif
}

ShaderReflection::~ShaderReflection()
{
}

#if defined(DX11)
HRESULT ShaderReflection::DoReflect(ID3DBlob* pVSBlob)
{
	return D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_pShaderReflection);
}
#endif