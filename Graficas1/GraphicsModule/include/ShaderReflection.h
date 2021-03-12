#pragma once
#if defined(DX11)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#endif
class ShaderReflection
{
public:
	ShaderReflection();
	~ShaderReflection();

#if defined(DX11)
	inline ID3D11ShaderReflection* GetDXShaderReflection() { return m_pShaderReflection; }
	inline ID3D11ShaderReflection** GetDXShaderReflectionAddress() { return &m_pShaderReflection; }
	inline D3D11_SHADER_DESC GetDXShaderReflectionDesc() { return ShaderReflectionDesc; }

	inline void GetDesc() { m_pShaderReflection->GetDesc(&ShaderReflectionDesc); }

	HRESULT DoReflect(ID3DBlob* pVSBlob);
#endif
private:
#if defined(DX11)
	ID3D11ShaderReflection* m_pShaderReflection;
	D3D11_SHADER_DESC ShaderReflectionDesc;
#endif
};

