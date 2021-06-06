#include "Tech.h"
#include "RenderManager.h"
#include "test.h"

Tech::Tech(const TechDesc _Desc)
{
	m_Active = false;
	m_Desc = _Desc;

//for (unsigned int j = 0; j < _Desc.PassNum; ++j)
//{
	vector<tuple<string, string>> Defines;
	vector<tuple<string, string>> vSecondPassDefines;

	if (_Desc.ActualEffect == kDeferred)
	{
		m_IsDef = true;
		m_vPassesNames.push_back("GBuffer");
		m_vPassesNames.push_back("GBufferLight");

			if (_Desc.DeferredFlags == kNormalVertex || _Desc.DeferredFlags == 4 || _Desc.DeferredFlags == 8)
			{
				Defines.push_back({ "NORMAL_VERTEX", "TRUE" });
				m_vDefines.push_back(Defines);

				m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

				if (_Desc.DefLightFlags == 0)
				{
					vSecondPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
				else if (_Desc.DefLightFlags == 1)
				{
					vSecondPassDefines.push_back({ "PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
				else if (_Desc.DefLightFlags == 2)
				{
					vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
			}
			else if (_Desc.DeferredFlags == kNormalMap || _Desc.DeferredFlags == 5 || _Desc.DeferredFlags == 9)
			{
				Defines.push_back({ "NORMAL_MAP", "TRUE" });
				m_vDefines.push_back(Defines);

				m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

				if (_Desc.DefLightFlags == 0)
				{
					vSecondPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
				else if (_Desc.DefLightFlags == 1)
				{
					vSecondPassDefines.push_back({ "PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
				else if (_Desc.DefLightFlags == 2)
				{
					vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
			}
			else if (_Desc.DeferredFlags == kNVSM || _Desc.DeferredFlags == 6)
			{
				Defines.push_back({ "NORMAL_VERTEX", "TRUE"});
				Defines.push_back({ "SPECULAR_MAP", "TRUE" });
				m_vDefines.push_back(Defines);

				m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

				if (_Desc.DefLightFlags == 0)
				{
					vSecondPassDefines.push_back({ "PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
				else if (_Desc.DefLightFlags == 1)
				{
					vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
			}
			else if (_Desc.DeferredFlags == kNMSM || _Desc.DeferredFlags == 7)
			{
				Defines.push_back({ "NORMAL_MAP", "TRUE" });
				Defines.push_back({ "SPECULAR_MAP", "TRUE" });
				m_vDefines.push_back(Defines);

				m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

				if (_Desc.DefLightFlags == 0)
				{
					vSecondPassDefines.push_back({ "PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
				else if (_Desc.DefLightFlags == 1)
				{
					vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
					m_vDefines.push_back(vSecondPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));
				}
			}
		//}
		//GBufferLight
		//else if (j == 1)
		//{
			//if (_Desc.DeferredFlags == kDefPhong)
			//{
			//	Defines.push_back({ "PHONG", "TRUE" });
			//	m_vDefines.push_back(Defines);
			//}
			//if (_Desc.DeferredFlags == kDefBlinnPhong)
			//{
			//	Defines.push_back({ "BLINN_PHONG", "TRUE" });
			//	m_vDefines.push_back(Defines);
			//}
		//}
	}
	else if (_Desc.ActualEffect == kForward)
	{
		m_IsDef = false;
		m_vPassesNames.push_back("Light");

		//if (j == 0)
		//{
			//if (_Desc.DeferredFlags == kNormalVertex)
			//{
			//	Defines.push_back({ "NORMAL_VERTEX", "TRUE" });
			//	m_vDefines.push_back(Defines);
			//}
			//else if (_Desc.DeferredFlags == kNormalMap)
			//{
			//	Defines.push_back({ "NORMAL_MAP", "TRUE" });
			//	m_vDefines.push_back(Defines);
			//}
		//}
		//else if (j == 1)
		//{
			if (m_Desc.TechTypesFlag == kVertex)
			{
				Defines.push_back({ "VERTEX_LIGHT", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Vertex Lighting";
			}
			else if (m_Desc.TechTypesFlag == kVertexPhong)
			{
				Defines.push_back({ "VERTEX_LIGHT", "TRUE" });
				Defines.push_back({ "PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Vertex Lighting + Phong";
			}
			else if (m_Desc.TechTypesFlag == kVertexBlinnPhong)
			{
				Defines.push_back({ "VERTEX_LIGHT", "TRUE" });
				Defines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Vertex Lighting + BlinnPhong";
			}
			else if (m_Desc.TechTypesFlag == kPixel)
			{
				Defines.push_back({ "PIXEL_LIGHT", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting";
			}
			else if (m_Desc.TechTypesFlag == kPixelPhong)
			{
				Defines.push_back({ "PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + Phong";
			}
			else if (m_Desc.TechTypesFlag == kPixelBlinnPhong)
			{
				Defines.push_back({ "BLINN_PHONG", "TRUE" });
				Defines.push_back({ "PIXEL_LIGHT", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + BlinnPhong";
			}
			else if (m_Desc.TechTypesFlag == kPixelNM)
			{
				Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + NM";
			}
			else if (m_Desc.TechTypesFlag == kPixelPhongNM)
			{
				Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + Phong + NM";
			}
			else if (m_Desc.TechTypesFlag == kPixelPhongNMSM)
			{
				Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "SPECULAR_MAP_PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + Phong + NM + SM";
			}
			else if (m_Desc.TechTypesFlag == kPixelBlinnPhongNM)
			{
				Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + BlinnPhong + NM";
			}
			else if (m_Desc.TechTypesFlag == kPixelBlinnPhongNMSM)
			{
				Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "SPECULAR_MAP_PIXEL_LIGHT", "TRUE" });
				Defines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(Defines);
				m_Name = "Pixel Lighting + BlinnPhong + NM + SM";
			}
		//}
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));
	}
//}

	m_PassNum = m_vPasses.size();
}

Tech::~Tech()
{
}

void Tech::Render(HWND _hwnd, vector<Model>& _Models)
{
	for (unsigned int i = 0; i < m_PassNum; ++i)
	{
		if (m_IsDef)
		{
			//Ultimo pase, o sea el de luces
			if (i == m_PassNum - 1)
			{
				if (!RM::GetRenderManager().IsBackBufferCleaned())
				{ 
					RM::GetRenderManager().SetBackBuffer();
					RM::GetRenderManager().SetBackBufferCleaned(true);
				}
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().GBufferLightRasterState.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, true);
			}
			else
			{
				RM::GetRenderManager().SetGBufferRTV();
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().GBufferRasterState.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, false);
			}
		}
		else
		{ 
			if (!RM::GetRenderManager().IsBackBufferCleaned())
			{
				RM::GetRenderManager().SetBackBuffer();
				RM::GetRenderManager().SetBackBufferCleaned(true);
			}
			m_vPasses[i].Render(_hwnd, _Models, false);
		}
	}
}

//void Tech::CleanUpResources()
//{
//	for (unsigned int i = 0; i < m_Passes.size(); ++i)
//	{
//		m_Passes[i].CleanUpShaders();
//	}
//}

void Tech::Deactivate()
{
	m_Active = false;
	//for (unsigned int i = 0; i < m_Passes.size(); ++i)
	//{
	//	m_Passes[i].CleanUpShaders();
	//}
}