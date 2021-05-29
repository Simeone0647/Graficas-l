#include "Effect.h"

Effect::Effect(vector<Tech>* _Techs, string _Name)
{
	m_Name = _strdup(&_Name[0]);
	m_vTechs = _Techs;
}

Effect::~Effect()
{
}

void Effect::Render(HWND _hwnd, vector<Model>& _Models)
{
	//if (m_vTechs[0][_i].IsActivated())
	//{
	//	m_vTechs[0][_i].Render(_hwnd, _Models);
	//}
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			m_vTechs[0][i].Render(_hwnd, _Models);
		}
	}
}

void Effect::DeactivateTech()
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			return m_vTechs[0][i].Deactivate();
		}
	}
}

void Effect::AddPass(Pass _Pass)
{
}

int Effect::GetPassNum()
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			return m_vTechs[0][i].GetPassNum();
		}
	}
}

int Effect::GetPassID(const int _j)
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			return m_vTechs[0][i].GetPassID(_j);
		}
	}
	return 0;
}

string Effect::GetPassName(const int _j)
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
				return m_vTechs[0][i].GetPassName(_j);
		}
	}
	return string();
}

int Effect::GetActiveTech()
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			return m_vTechs[0][i].GetLightingTech();
		}
	}
}
