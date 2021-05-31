#include "Tech.h"

Tech::Tech(const int _Flags, HWND _hwnd, int& _PassNum)
{
	m_Active = false;

	m_Desc.TechTypesFlag = _Flags;

	if (m_Desc.TechTypesFlag == kVertex)
	{
		m_vDefines.push_back( { "VERTEX_LIGHT", "TRUE" } );
		m_Name = "Vertex Lighting";
	}
	else if (m_Desc.TechTypesFlag == kVertexPhong)
	{
		m_vDefines.push_back( { "VERTEX_LIGHT", "TRUE" } );
		m_vDefines.push_back( { "PHONG", "TRUE" } );
		m_Name = "Vertex Lighting + Phong";
	}
	else if (m_Desc.TechTypesFlag == kVertexBlinnPhong)
	{
		m_vDefines.push_back( { "VERTEX_LIGHT", "TRUE" } );
		m_vDefines.push_back( { "BLINN_PHONG", "TRUE" } );
		m_Name = "Vertex Lighting + BlinnPhong";
	}
	else if (m_Desc.TechTypesFlag == kPixel)
	{
		m_vDefines.push_back( { "PIXEL_LIGHT", "TRUE" } );
		m_Name = "Pixel Lighting";
	}
	else if (m_Desc.TechTypesFlag == kPixelPhong)
	{
		m_vDefines.push_back( { "PIXEL_LIGHT", "TRUE" } );
		m_vDefines.push_back( { "PHONG", "TRUE" } );
		m_Name = "Pixel Lighting + Phong";
	}
	else if (m_Desc.TechTypesFlag == kPixelBlinnPhong)
	{
		m_vDefines.push_back({ "PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "BLINN_PHONG", "TRUE" });
		m_Name = "Pixel Lighting + BlinnPhong";
	}
	else if (m_Desc.TechTypesFlag == kPixelNM)
	{
		m_vDefines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
		m_Name = "Pixel Lighting + NM";
	}
	else if (m_Desc.TechTypesFlag == kPixelPhongNM)
	{
		m_vDefines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "PHONG", "TRUE" });
		m_Name = "Pixel Lighting + Phong + NM";
	}
	else if (m_Desc.TechTypesFlag == kPixelPhongNMSM)
	{
		m_vDefines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "SPECULAR_MAP_PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "PHONG", "TRUE" });
		m_Name = "Pixel Lighting + Phong + NM + SM";
	}
	else if (m_Desc.TechTypesFlag == kPixelBlinnPhongNM)
	{
		m_vDefines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "BLINN_PHONG", "TRUE" });
		m_Name = "Pixel Lighting + BlinnPhong + NM";
	}
	else if (m_Desc.TechTypesFlag == kPixelBlinnPhongNMSM)
	{
		m_vDefines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "SPECULAR_MAP_PIXEL_LIGHT", "TRUE" });
		m_vDefines.push_back({ "BLINN_PHONG", "TRUE" });
		m_Name = "Pixel Lighting + BlinnPhong + NM + SM";
	}
	m_Passes.push_back(Pass(m_vDefines, _hwnd, "Light", _PassNum));
	_PassNum++;

	m_PassNum = m_Passes.size();
}

Tech::~Tech()
{
}

void Tech::Render(HWND _hwnd, vector<Model>& _Models)
{
	for (unsigned int i = 0; i < m_Passes.size(); ++i)
	{
		m_Passes[i].Render(_hwnd, _Models);
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