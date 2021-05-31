#include "Effect.h"

Effect::Effect(vector<Tech>* _Techs, string _Name)
{
	m_Name = _strdup(&_Name[0]);
	m_vTechs = _Techs;
	m_ImGuiNormalMap = false;
	m_ImGuiSpecularMap = false;
}

Effect::~Effect()
{
}

void Effect::Render(HWND _hwnd, vector<Model>& _Models)
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			m_vTechs[0][i].Render(_hwnd, _Models);
		}
	}
}

void Effect::ActivateIlumTechPlace(const int _LightPlace)
{
	
}

void Effect::ActivateTech(const int _LightPlace, const int _LightModel, const int _Features)
{
	if (_LightPlace == kVertex)
	{
		if (_LightModel == 0)
		{
			m_vTechs[0][0].Activate();
			return;
		}
		else if (_LightModel == 1)
		{
			m_vTechs[0][1].Activate();
			return;
		}
		else if (_LightModel == 2)
		{
			m_vTechs[0][2].Activate();
			return;
		}
	}
	else if (_LightPlace == 1)
	{
		if (_LightModel == 0)
		{
			if (_Features == 0)
			{ 
				m_vTechs[0][kPixel].Activate();
				return;
			}
			else if (_Features == kPixelNM)
			{
				m_vTechs[0][kPixelNM].Activate();
				return;
			}
		}
		else if (_LightModel == 1)
		{
			if (_Features == 0)
			{ 
				m_vTechs[0][kPixelPhong].Activate();
				return;
			}
			else if (_Features == kPixelPhongNM)
			{
				m_vTechs[0][kPixelPhongNM].Activate();
				return;
			}
			else if (_Features == kPixelPhongNMSM)
			{
				m_vTechs[0][kPixelPhongNMSM].Activate();
				return;
			}
		}
		else if (_LightModel == 2)
		{
			if (_Features == 0)
			{ 
				m_vTechs[0][kPixelBlinnPhong].Activate();
				return;
			}
			else if (_Features == kPixelBlinnPhongNM)
			{
				m_vTechs[0][kPixelBlinnPhongNM].Activate();
				return;
			}
			else if (_Features == kPixelBlinnPhongNMSM)
			{
				m_vTechs[0][kPixelBlinnPhongNMSM].Activate();
				return;
			}
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
