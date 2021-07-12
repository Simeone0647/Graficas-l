#include "Shader.h"
#include "test.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

#if defined(DX11)
HRESULT Shader::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
	hr = D3DX11CompileFromFileA(szFileName, m_vMacros.data(), NULL, szEntryPoint, szShaderModel,
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

void Shader::CreateInputLayout(HRESULT& _hr, ID3DBlob* _pVSBlob, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection)
{
	HWND hwnd = NULL;

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

	_hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateInputLayout(ILStruct);
	_pVSBlob->Release();
	if (FAILED(_hr))
	{
		std::cerr << "Fallo hr en inputlayout" << std::endl;
	}

	//Free allocation shader reflection memory
	_ShaderReflection.GetDXShaderReflection()->Release();
}

void Shader::CompilePixelShader(HRESULT& _hr, PixelShader& _PixelShader, string _Filename)
{
	HWND hwnd = NULL;

	ID3DBlob* pPSBlob = NULL;

	string Filename = _Filename + "PS.hlsl";
	_hr = CompileShaderFromFile(&Filename[0], "PS", "ps_4_0", &pPSBlob);

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
	_hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreatePixelShader(PSStruct);
	pPSBlob->Release();
	if (FAILED(_hr))
	{
		std::cout << "Fallo hr en crear el piksel cheider" << std::endl;
	}
}
#endif

#if defined(OGL)
unsigned int Shader::LoadShaders(const char* _VertexFilepath, const char* _FragmentFilepath)
{
	// Crear los shaders
	unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Leer el Vertex Shader desde archivo
	std::string VertexShaderCode = "#version 330 core\n";
	VertexShaderCode += "#extension GL_ARB_separate_shader_objects : enable\n";
	for (unsigned int i = 0; i < m_vMacros.size(); ++i)
	{
		VertexShaderCode += m_vMacros[i];
	}

	std::ifstream VertexShaderStream(_VertexFilepath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode += sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", _VertexFilepath);
		getchar();
		return 0;
	}

	// Leer el Fragment Shader desde archivo
	std::string FragmentShaderCode = "#version 330 core\n";
	FragmentShaderCode += "#extension GL_ARB_separate_shader_objects : enable\n";
	for (unsigned int i = 0; i < m_vMacros.size(); ++i)
	{
		FragmentShaderCode += m_vMacros[i];
	}

	std::ifstream FragmentShaderStream(_FragmentFilepath, std::ios::in);
	if (FragmentShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode += sstr.str();
		FragmentShaderStream.close();
	}

	int Result = GL_FALSE;
	int InfoLogLength;


	// Compilar Vertex Shader
	printf("Compiling shader : %s\n", _VertexFilepath);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Revisar Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compilar Fragment Shader
	printf("Compiling shader : %s\n", _FragmentFilepath);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Revisar Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Vincular el programa por medio del ID
	printf("Linking program\n");
	unsigned int ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Revisar el programa
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void Shader::CompileShaders(const string _ShaderFilepath)
{
	string VertexFilepath = _ShaderFilepath + "VS.glsl";
	string FragmentFilepath = _ShaderFilepath + "PS.glsl";

	m_ShaderID = LoadShaders(VertexFilepath.c_str(), FragmentFilepath.c_str());

}
#endif

void Shader::SetMacros(const vector<tuple<string, string>> _Macros)
{
	#if defined(DX11)
	for (unsigned int i = 0; i < _Macros.size(); ++i)
	{
		m_vMacros.push_back({ _strdup(std::get<0>(_Macros[i]).c_str()), _strdup(std::get<1>(_Macros[i]).c_str()) });
	}

	m_vMacros.push_back({ NULL, NULL });
	#endif
	#if defined(OGL)
	//m_vMacros.push_back("#pragma optimize (off)\n");
	for (unsigned int i = 0; i < _Macros.size(); ++i)
	{
		if(std::get<0>(_Macros[i]) != "" && std::get<1>(_Macros[i]) != "")
		{
			m_vMacros.push_back("#define " + std::get<0>(_Macros[i]) + "\n");
		}
	}
	#endif
}

#if defined(DX11)
void Shader::CompileShaders(VertexShader& _VertexShader, InputLayout& _InputLayout, ShaderReflection& _ShaderReflection, PixelShader& _PixelShader, string _Filename)
{

	HWND hwnd = NULL;

	HRESULT hr = S_OK;

	ID3DBlob* pVSBlob = NULL;

	string Filename = _Filename + "VS.hlsl";
	hr = CompileShaderFromFile(&Filename[0], "VS", "vs_4_0", &pVSBlob);

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
	hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateVertexShader(VSStruct);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		std::cout << "Falló el blob en vertex shader" << std::endl;
	}

	CreateInputLayout(hr, pVSBlob, _InputLayout, _ShaderReflection);
	CompilePixelShader(hr, _PixelShader, _Filename);
}
#endif
