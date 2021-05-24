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

class Shader
{
public:
	Shader();
	~Shader();
	
	#if defined(DX11)
	HRESULT CompileShaderFromFile(const char* szFileName, const D3D10_SHADER_MACRO* _Macros, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	void CreateInputLayout(HRESULT& _hr, ID3DBlob* pVSBlob, HWND _hwnd, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection);
	void CompilePixelShader(HRESULT& _hr, HWND _hwnd, PixelShader& _PixelShader);
	void SetMacros(std::string _DefineName, std::string _DefineValue);
	#endif

	void CompileShaders(HWND _hwnd, VertexShader& _VertexShader, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection, PixelShader& _PixelShader);
private:
	#if defined(DX11)
	std::string m_MacroName;
	std::string m_MacroValue;
	#endif
};

