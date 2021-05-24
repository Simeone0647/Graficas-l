#include "Pass.h"
#include "test.h"

Pass::Pass()
{
	m_Models = nullptr;
}

Pass::Pass(std::vector<Model>* _Models, std::string _DefineName, std::string _DefineValue, HWND _hwnd)
{
	m_Models = _Models;
	
	m_Shader.SetMacros(_DefineName, _DefineValue);
	m_Shader.CompileShaders(_hwnd, m_VertexShader, m_InputLayout, m_ShaderReflection, m_PixelShader);
}

Pass::~Pass()
{
}

void Pass::Init(HWND _hwnd)
{
	m_Shader.CompileShaders(_hwnd, m_VertexShader, m_InputLayout, m_ShaderReflection, m_PixelShader);
}

void Pass::Render(HWND _hwnd)
{
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetInputLayout(m_InputLayout.GetDXInputLayout());

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetShader(m_VertexShader.GetDXVertexShader(), NULL, 0);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetShader(m_PixelShader.GetDXPixelShader(), NULL, 0);

	for (unsigned int i = 0; i < m_Models[0].size(); ++i)
	{
		m_Models[0][i].Render(_hwnd);
	}

}

void Pass::CleanUpDevice()
{
	if (m_InputLayout.GetDXInputLayout()) m_InputLayout.GetDXInputLayout()->Release();
	if (m_VertexShader.GetDXVertexShader()) m_VertexShader.GetDXVertexShader()->Release();
	if (m_PixelShader.GetDXPixelShader()) m_PixelShader.GetDXPixelShader()->Release();

	for (unsigned int i = 0; i < m_Models[0].size(); ++i)
	{
		m_Models[0][i].CleanUpDXResources();
	}
}
