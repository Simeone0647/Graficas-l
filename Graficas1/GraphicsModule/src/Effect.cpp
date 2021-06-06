#include "Effect.h"
#include "RenderManager.h"

Effect::Effect(vector<Tech>* _Techs, string _Name, const SetTechDesc _TechDesc)
{
	m_Name = _strdup(&_Name[0]);
	m_vTechs = _Techs;

	m_ImGuiNormalMap = false;
	m_ImGuiSpecularMap = false;

	ImGuiDefSM = false;

	if (_TechDesc.DeferredRender)
	{
		m_ActiveTech = 1;
	}
	else if (_TechDesc.ForwardRender)
	{
		m_ActiveTech = 0;
	}
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
			return;
		}
	}
}

void Effect::ActivateIlumTechPlace(const int _LightPlace)
{
	
}

void Effect::ActivateTech(const SetTechDesc _TechDesc)
{
	if (m_ActiveTech == 0)
	{ 
		if (_TechDesc.LightingPlace == 0)
		{
			if (_TechDesc.LightingModel == 0)
			{
				m_vTechs[0][kVertex].Activate();//0
				return;
			}
			else if (_TechDesc.LightingModel == 1)
			{
				m_vTechs[0][kVertexPhong].Activate();//1
				return;
			}
			else if (_TechDesc.LightingModel == 2)
			{
				m_vTechs[0][kVertexBlinnPhong].Activate();//2
				return;
			}
		}
		else if (_TechDesc.LightingPlace == 1)
		{
			if (_TechDesc.LightingModel == 0)
			{
				if (_TechDesc.Features == 0)
				{ 
					m_vTechs[0][kPixel].Activate();//3
					return;
				}
				else if (_TechDesc.Features == kPixelNM)
				{
					m_vTechs[0][kPixelNM].Activate();//4
					return;
				}
			}
			else if (_TechDesc.LightingModel == 1)
			{
				if (_TechDesc.Features == 0)
				{ 
					m_vTechs[0][kPixelPhong].Activate();//5
					return;
				}
				else if (_TechDesc.Features == kPixelPhongNM)
				{
					m_vTechs[0][kPixelPhongNM].Activate();//7
					return;
				}
				else if (_TechDesc.Features == kPixelPhongNMSM)
				{
					m_vTechs[0][kPixelPhongNMSM].Activate();//8
					return;
				}
			}
			else if (_TechDesc.LightingModel == 2)
			{
				if (_TechDesc.Features == 0)
				{ 
					m_vTechs[0][kPixelBlinnPhong].Activate();//6
					return;
				}
				else if (_TechDesc.Features == kPixelBlinnPhongNM)
				{
					m_vTechs[0][kPixelBlinnPhongNM].Activate();//9
					return;
				}
				else if (_TechDesc.Features == kPixelBlinnPhongNMSM)
				{
					m_vTechs[0][kPixelBlinnPhongNMSM].Activate();//10
					return;
				}
			}
		}
	}
	else if (m_ActiveTech == 1)
	{
		if (_TechDesc.LightingModel == 0)
		{ 
			if (_TechDesc.NormalCalc == kNormalVertex)
			{
				m_vTechs[0][kNormalVertex].Activate();
				return;
			}
			else if (_TechDesc.NormalCalc == kNormalMap)
			{
				m_vTechs[0][kNormalMap].Activate();
				return;
			}
		}
		else if (_TechDesc.LightingModel == 1)
		{
			if (_TechDesc.NormalCalc == kNormalVertex)
			{
				if (_TechDesc.Specular)
				{
					m_vTechs[0][kNVSM].Activate();
					return;
				}
				else
				{ 
					m_vTechs[0][4].Activate();
					return;
				}
			}
			else if (_TechDesc.NormalCalc == kNormalMap)
			{
				if (_TechDesc.Specular)
				{
					m_vTechs[0][kNMSM].Activate();
					return;
				}
				else
				{ 
					m_vTechs[0][5].Activate();
					return;
				}
			}
		}
		else if (_TechDesc.LightingModel == 2)
		{
			if (_TechDesc.NormalCalc == kNormalVertex)
			{
				if (_TechDesc.Specular)
				{
					m_vTechs[0][6].Activate();
					return;
				}
				else
				{ 
					m_vTechs[0][8].Activate();
					return;
				}
			}
			else if (_TechDesc.NormalCalc == kNormalMap)
			{
				if (_TechDesc.Specular)
				{
					m_vTechs[0][7].Activate();
					return;
				}
				else
				{ 
					m_vTechs[0][9].Activate();
					return;
				}
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
