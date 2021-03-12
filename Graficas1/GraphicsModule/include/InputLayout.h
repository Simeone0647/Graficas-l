#pragma once
#include <vector>
#if defined(DX11)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#endif
class InputLayout
{
public:
	InputLayout();
	~InputLayout();

#if defined(DX11)
	inline ID3D11InputLayout* GetDXInputLayout() { return m_pInputLayout; }
	inline ID3D11InputLayout** GetDXInputLayoutAddress() { return &m_pInputLayout; }

	void DefineInputLayout(D3D11_SHADER_DESC shaderdesc, ID3D11ShaderReflection* shaderreflection);

	inline D3D11_INPUT_ELEMENT_DESC* GetDXInputLayoutDescAddress() { return &m_vInputLayoutDesc[0]; }
	inline UINT GetDXInputLayoutDescSize() { return m_vInputLayoutDesc.size(); }
#endif

private:
#if defined(DX11)
	ID3D11InputLayout* m_pInputLayout;
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_vInputLayoutDesc;
#endif
};

