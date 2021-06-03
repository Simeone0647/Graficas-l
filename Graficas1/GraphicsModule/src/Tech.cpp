#include "Tech.h"

Tech::Tech(const TechDesc _Desc)
{
	m_Active = false;

	m_Desc = _Desc;

	m_vPassesNames.push_back("GBuffer");
	m_vPassesNames.push_back("Light");

	for (unsigned int i = 0; i < _Desc.PassNum; ++i)
	{
		vector<tuple<string, string>> Defines;

		if (i == 0)
		{
			if (_Desc.DeferredFlags == kNormalVertex)
			{	
				Defines.push_back({"NORMAL_VERTEX", "TRUE" });
				m_vDefines.push_back(Defines);
			}
			else if (_Desc.DeferredFlags == kNormalMap)
			{
				Defines.push_back({ "NORMAL_MAP", "TRUE" });
				m_vDefines.push_back(Defines);
			}
		}
		else if (i == 1)
		{
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
		}
		m_vPasses.push_back(Pass(m_vDefines[i], _Desc.hwnd, m_vPassesNames[i]));
	}

	m_PassNum = m_vPasses.size();
}

Tech::~Tech()
{
}

void Tech::Render(HWND _hwnd, vector<Model>& _Models)
{
	for (unsigned int i = 0; i < m_PassNum - 1; ++i)
	{
		m_vPasses[i].Render(_hwnd, _Models);
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