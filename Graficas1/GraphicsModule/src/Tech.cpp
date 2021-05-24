#include "Tech.h"

Tech::Tech(const int _Flags, HWND _hwnd)
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
	m_Passes.push_back(Pass(m_DefineName, m_DefineValue, _hwnd));
}

Tech::~Tech()
{
}

void Tech::Render(HWND _hwnd)
{
	for (unsigned int i = 0; i < m_Passes.size(); ++i)
	{
		m_Passes[i].Render(_hwnd);
	}
}

//void Tech::CleanUpResources()
//{
//	for (unsigned int i = 0; i < m_Passes.size(); ++i)
//	{
//		m_Passes[i].CleanUpShaders();
//	}
//}

void Tech::SetModels(std::vector<Model>* _Models, const unsigned int _PassNum)
{
	m_Passes[_PassNum].SetModels(_Models);
}

void Tech::Deactivate()
{
	m_Active = false;
	//for (unsigned int i = 0; i < m_Passes.size(); ++i)
	//{
	//	m_Passes[i].CleanUpShaders();
	//}
}