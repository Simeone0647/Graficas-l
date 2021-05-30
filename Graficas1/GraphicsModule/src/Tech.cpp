#include "Tech.h"

Tech::Tech(const int _Flags, HWND _hwnd, int& _PassNum)
{
	m_DefineName = "";
	m_DefineValue = "";
	m_Active = false;

	m_Desc.TechTypesFlag = _Flags;
	if (m_Desc.TechTypesFlag == kNone)
	{ 
		m_DefineName += "";
		m_DefineValue += "";
		m_Name = "NONE";
	}
	else if (m_Desc.TechTypesFlag == kIlumPerVertex)
	{
		m_DefineName += "VERTEX_LIGHT";
		m_DefineValue += "TRUE";
		m_Name = "Vertex Lighting";
	}
	else if (m_Desc.TechTypesFlag == kIlumPerPixel)
	{
		m_DefineName += "PIXEL_LIGHT";
		m_DefineValue += "TRUE";
		m_Name = "Pixel Lighting";
	}
	else if (m_Desc.TechTypesFlag == kNormalMap)
	{
		m_DefineName += "NORMAL_MAP_PIXEL_LIGHT";
		m_DefineValue += "TRUE";
		m_Name = "Normal Map + Pixel Light";
	}
	m_Passes.push_back(Pass(m_DefineName, m_DefineValue, _hwnd, "Light", _PassNum));
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