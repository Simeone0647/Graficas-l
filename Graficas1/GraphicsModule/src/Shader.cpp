#include "Shader.h"
#include "test.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

#if defined(DX11)
HRESULT Shader::CompileShaderFromFile(const char* szFileName, const D3D10_SHADER_MACRO* _Macros, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	//std::ofstream outfile("test.txt");
	//
	//outfile << "my text here!" << std::endl;
	//
	//outfile.close();

	HRESULT hr = S_OK;

	DWORD dwShaderFlags = SIMECOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= SIMECOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFileA(szFileName, _Macros, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void Shader::CreateInputLayout(HRESULT& _hr, ID3DBlob* _pVSBlob, HWND _hwnd, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection)
{
	_hr = _ShaderReflection.DoReflect(_pVSBlob);
	if (FAILED(_hr))
	{
		_pVSBlob->Release();
		std::cout << "Fallo hr en reflection" << std::endl;
	}

	//CREATE INPUT LAYOUT WITH REFLECTION
	_ShaderReflection.GetDesc();
	_InputLayout.DefineInputLayout(_ShaderReflection.GetDXShaderReflectionDesc(), _ShaderReflection.GetDXShaderReflection());

	GraphicsModule::CreateInputLayoutStruct ILStruct;
	ILStruct.pInputElementDescs = _InputLayout.GetDXInputLayoutDescAddress();
	ILStruct.NumElements = _InputLayout.GetDXInputLayoutDescSize();
	ILStruct.pShaderBytecodeWithInputSignature = _pVSBlob->GetBufferPointer();
	ILStruct.BytecodeLength = _pVSBlob->GetBufferSize();
	ILStruct.ppInputLayout = _InputLayout.GetDXInputLayoutAddress();

	_hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateInputLayout(ILStruct);
	_pVSBlob->Release();
	if (FAILED(_hr))
	{
		std::cerr << "Fallo hr en inputlayout" << std::endl;
	}

	//Free allocation shader reflection memory
	_ShaderReflection.GetDXShaderReflection()->Release();
}

void Shader::CompilePixelShader(HRESULT& _hr, HWND _hwnd, PixelShader& _PixelShader)
{
	ID3DBlob* pPSBlob = NULL;

	const D3D10_SHADER_MACRO Defines[] =
	{
		m_MacroName.c_str(), m_MacroValue.c_str(),
		NULL, NULL
	};

	_hr = CompileShaderFromFile("Tutorial07.fx", Defines, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(_hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
	}

	GraphicsModule::CreatePixelShaderStruct PSStruct;
	PSStruct.pShaderBytecode = pPSBlob->GetBufferPointer();
	PSStruct.BytecodeLength = pPSBlob->GetBufferSize();
	PSStruct.pClassLinkage = NULL;
	PSStruct.ppPixelShader = _PixelShader.GetDXPixelShaderAddress();

	// Create the pixel shader
	_hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreatePixelShader(PSStruct);
	pPSBlob->Release();
	if (FAILED(_hr))
	{
		std::cout << "Fallo hr en crear el piksel cheider" << std::endl;
	}
}
void Shader::SetMacros(std::string _DefineName, std::string _DefineValue)
{
	m_MacroName = _DefineName;
	m_MacroValue = _DefineValue;
}
#endif

void Shader::CompileShaders(HWND _hwnd, VertexShader& _VertexShader, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection, PixelShader& _PixelShader)
{
#if defined(DX11)
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;

	const D3D10_SHADER_MACRO Defines[] =
	{
		m_MacroName.c_str(), m_MacroValue.c_str(),
		NULL, NULL
	};

	hr = CompileShaderFromFile("Tutorial07.fx", Defines, "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL,
			"The FX file cannot be compiled. Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
	}

	GraphicsModule::CreateVertexShaderStruct VSStruct;
	VSStruct.pShaderBytecode = pVSBlob->GetBufferPointer();
	VSStruct.BytecodeLength = pVSBlob->GetBufferSize();
	VSStruct.pClassLinkage = NULL;
	VSStruct.ppVertexShader = _VertexShader.GetDXVertexShaderAddress();

	// Create the vertex shader
	hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateVertexShader(VSStruct);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		std::cout << "Fall� el blob en vertex shader" << std::endl;
	}

	CreateInputLayout(hr, pVSBlob, _hwnd, _InputLayout, _ShaderReflection);
	CompilePixelShader(hr, _hwnd, _PixelShader);
#endif
}