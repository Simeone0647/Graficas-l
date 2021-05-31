#include "Pass.h"
#include "test.h"

Pass::Pass()
{

}

Pass::Pass(const vector<tuple<string, string>> _Macros, HWND _hwnd, string _Name, int _ID)
{	
	//if (_DefineName == "") m_ID = "None";
	//else if (_DefineName == "VERTEX_LIGHT") m_ID = "Vertex Light";
	//else if (_DefineName == "PIXEL_LIGHT") m_ID = "Pixel Light";

	m_Name = _Name;

	m_Shader.SetMacros(_Macros);
	m_Shader.CompileShaders(_hwnd, m_VertexShader, m_InputLayout, m_ShaderReflection, m_PixelShader);

	m_ID = _ID;
}

Pass::~Pass()
{
}

void Pass::Render(HWND _hwnd, vector<Model>& _Models)
{
	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetInputLayout(m_InputLayout.GetDXInputLayout());

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetShader(m_VertexShader.GetDXVertexShader(), NULL, 0);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetShader(m_PixelShader.GetDXPixelShader(), NULL, 0);
	
	for (unsigned int i = 0; i < _Models.size(); ++i)
	{
		//if (_Models[i].GetPassID(m_ID))
		//{ 
			_Models[i].Render(_hwnd);
		//}
	}
}

void Pass::CleanUpShaders()
{
	if (m_InputLayout.GetDXInputLayout()) m_InputLayout.GetDXInputLayout()->Release();
	if (m_VertexShader.GetDXVertexShader()) m_VertexShader.GetDXVertexShader()->Release();
	if (m_PixelShader.GetDXPixelShader()) m_PixelShader.GetDXPixelShader()->Release();
}
