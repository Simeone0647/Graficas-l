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
	void CompileShaders(VertexShader& _VertexShader, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection, PixelShader& _PixelShader, string _Filename);
	#endif
	#if defined(OGL)
	void CompileShaders(const string _ShaderFilepath);
	inline int GetShaderID() { return m_ShaderID; }
	#endif
	void SetMacros(const vector<tuple<string, string>> _Macros);

private:
	#if defined(DX11)
	HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void CreateInputLayout(HRESULT& _hr, ID3DBlob* pVSBlob, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection);

	void CompilePixelShader(HRESULT& _hr, PixelShader& _PixelShader, string _Filename);

	vector<D3D10_SHADER_MACRO> m_vMacros;
	#endif
	#if defined(OGL)
	unsigned int LoadShaders(const char* _VertexFilepath, const char* _FragmentFilepath);
	unsigned int m_ShaderID;
	
	vector<string> m_vMacros;
	#endif
};

