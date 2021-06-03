#pragma once
#if defined(DX11)
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#endif
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderReflection.h"
#include "InputLayout.h"
#include <iostream>
#include <fstream>  
#include <vector>
#include <tuple>

using namespace std;

class Shader
{
public:
	Shader();
	~Shader();
	
	#if defined(DX11)
	HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	void CreateInputLayout(HRESULT& _hr, ID3DBlob* pVSBlob, HWND _hwnd, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection);
	void CompilePixelShader(HRESULT& _hr, HWND _hwnd, PixelShader& _PixelShader, string _Filename);
	void SetMacros(const vector<tuple<string, string>> _Macros);
	#endif

	void CompileShaders(HWND _hwnd, VertexShader& _VertexShader, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection, PixelShader& _PixelShader, string _Filename);
private:
	#if defined(DX11)
	vector<D3D10_SHADER_MACRO> m_vMacros;
	#endif
};

