#include "Pass.h"
#include "test.h"
#include "RenderManager.h"

Pass::Pass()
{

}

Pass::Pass(const vector<tuple<string, string>> _Macros, HWND _hwnd, string _Name)
{	
	m_ID = 0;
	HRESULT hr;
	m_Name = _Name;
	if (m_Name == "GBuffer")
	{
		m_ShaderFilename = "GBuffer";
		D3D11_RASTERIZER_DESC dr;
		dr.CullMode = D3D11_CULL_BACK;
		dr.FillMode = D3D11_FILL_SOLID;
		dr.FrontCounterClockwise = false;
		dr.DepthBiasClamp = 0;
		dr.SlopeScaledDepthBias = 0;
		dr.DepthClipEnable = FALSE;
		dr.MultisampleEnable = TRUE;
		dr.ScissorEnable = FALSE;
		dr.AntialiasedLineEnable = FALSE;

		hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateRasterizerState(&dr, m_Rasterizer.GetRSAddress());
		if (FAILED(hr))
		{
			cout << "Fallo en crear el RasterizerLight" << endl;
		}
	}
	else if (m_Name == "GBufferLight")
	{
		D3D11_RASTERIZER_DESC dr;
		dr.CullMode = D3D11_CULL_NONE;
		dr.FillMode = D3D11_FILL_SOLID;
		dr.FrontCounterClockwise = false;
		dr.DepthBiasClamp = 0;
		dr.SlopeScaledDepthBias = 0;
		dr.DepthClipEnable = FALSE;
		dr.MultisampleEnable = TRUE;
		dr.ScissorEnable = FALSE;
		dr.AntialiasedLineEnable = FALSE;
		hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateRasterizerState(&dr, m_Rasterizer.GetRSAddress());
		if (FAILED(hr))
		{

		}

		m_ShaderFilename = "GBufferLight";
	}
	else if (m_Name == "Light")
	{
		m_ShaderFilename = "Tutorial07.fx";
	}
	m_Shader.SetMacros(_Macros);
	m_Shader.CompileShaders(_hwnd, m_VertexShader, m_InputLayout, m_ShaderReflection, m_PixelShader, m_ShaderFilename);
}

Pass::~Pass()
{
}

void Pass::Render(HWND _hwnd, vector<Model>& _Models, bool _ReadSAQ)
{
	//GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(m_Rasterizer.GetRS());

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CVSSetShader(m_VertexShader.GetDXVertexShader(), NULL, 0);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetShader(m_PixelShader.GetDXPixelShader(), NULL, 0);

	GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CIASetInputLayout(m_InputLayout.GetDXInputLayout());
	

	if (_ReadSAQ)
	{
		if (_Models.size() > 1)
		{ 
			for (unsigned int i = 0; i < 4; ++i)
			{
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetShaderResources(i, 1, RM::GetRenderManager().GBufferSRV[i].GetDXSRVAddress());
			}
			//for (unsigned int i = 0; i < RM::GetRenderManager().vGBufferSamplers.size(); ++i)
			//{
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CPSSetSamplers(0, RM::GetRenderManager().vGBufferSamplers.GetSamplerNum(),
																					   RM::GetRenderManager().vGBufferSamplers.GetDXSamplerStateAddress());
			//}
			_Models[0].Render(_hwnd);
		}
	}
	else
	{ 
		for (unsigned int i = 1; i < _Models.size(); ++i)
		{
			//if (_Models[i].GetPassID(m_ID))
			//{ 
				_Models[i].Render(_hwnd);
			//}
		}
	}
}

void Pass::CleanUpShaders()
{
	if (m_InputLayout.GetDXInputLayout()) m_InputLayout.GetDXInputLayout()->Release();
	if (m_VertexShader.GetDXVertexShader()) m_VertexShader.GetDXVertexShader()->Release();
	if (m_PixelShader.GetDXPixelShader()) m_PixelShader.GetDXPixelShader()->Release();
}
