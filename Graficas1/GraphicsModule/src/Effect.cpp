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

void Effect::Render()
{
	for (unsigned int i = 0; i < m_vTechs[0].size(); ++i)
	{
		if (m_vTechs[0][i].IsActivated())
		{
			m_vTechs[0][i].Render();
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
				if (_TechDesc.ColorCorr == 0)
				{
					m_vTechs[0][kVertex].Activate();//0
					return;
				}
				else if (_TechDesc.ColorCorr == 1)
				{
					m_vTechs[0][41].Activate();//0
					return;
				}
				else if (_TechDesc.ColorCorr == 2)
				{
					m_vTechs[0][52].Activate();//0
					return;
				}
			}
			else if (_TechDesc.LightingModel == 1)
			{
				if (_TechDesc.ColorCorr == 0)
				{
					m_vTechs[0][kVertexPhong].Activate();//0
					return;
				}
				else if (_TechDesc.ColorCorr == 1)
				{
					m_vTechs[0][42].Activate();//0
					return;
				}
				else if (_TechDesc.ColorCorr == 2)
				{
					m_vTechs[0][53].Activate();//0
					return;
				}
			}
			else if (_TechDesc.LightingModel == 2)
			{
				if (_TechDesc.ColorCorr == 0)
				{
					m_vTechs[0][kVertexBlinnPhong].Activate();//0
					return;
				}
				else if (_TechDesc.ColorCorr == 1)
				{
					m_vTechs[0][43].Activate();//0
					return;
				}
				else if (_TechDesc.ColorCorr == 2)
				{
					m_vTechs[0][54].Activate();//0
					return;
				}
			}
		}
		else if (_TechDesc.LightingPlace == 1)
		{
			if (_TechDesc.LightingModel == 0)
			{
				if (_TechDesc.Features == 0)
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixel].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][44].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][55].Activate();//0
						return;
					}
				}
				else if (_TechDesc.Features == kPixelNM)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelNM].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][47].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][58].Activate();//0
						return;
					}
				}
			}
			else if (_TechDesc.LightingModel == 1)
			{
				if (_TechDesc.Features == 0)
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelPhong].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][45].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][56].Activate();//0
						return;
					}
				}
				else if (_TechDesc.Features == kPixelPhongNM)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelPhongNM].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][48].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][59].Activate();//0
						return;
					}
				}
				else if (_TechDesc.Features == kPixelPhongNMSM)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelPhongNMSM].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][49].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][60].Activate();//0
						return;
					}
				}
			}
			else if (_TechDesc.LightingModel == 2)
			{
				if (_TechDesc.Features == 0)
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelBlinnPhong].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][46].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][57].Activate();//0
						return;
					}
					return;
				}
				else if (_TechDesc.Features == kPixelBlinnPhongNM)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelBlinnPhongNM].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][50].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][61].Activate();//0
						return;
					}
				}
				else if (_TechDesc.Features == kPixelBlinnPhongNMSM)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][kPixelBlinnPhongNMSM].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][51].Activate();//0
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][62].Activate();//0
						return;
					}
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
				if (_TechDesc.ColorCorr == 0)
				{
					m_vTechs[0][kNormalVertex].Activate();
					return;
				}
				else if (_TechDesc.ColorCorr == 1)
				{
					m_vTechs[0][4].Activate();
					return;
				}
				else if (_TechDesc.ColorCorr == 2)
				{
					m_vTechs[0][8].Activate();
					return;
				}
			}
			else if (_TechDesc.NormalCalc == kNormalMap)
			{
				if (_TechDesc.ColorCorr == 0)
				{
					m_vTechs[0][kNormalMap].Activate();
					return;
				}
				else if (_TechDesc.ColorCorr == 1)
				{
					m_vTechs[0][5].Activate();
					return;
				}
				else if (_TechDesc.ColorCorr == 2)
				{
					m_vTechs[0][9].Activate();
					return;
				}

			}
		}
		else if (_TechDesc.LightingModel == 1)
		{
			if (_TechDesc.NormalCalc == kNormalVertex)
			{
				if (_TechDesc.Specular)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][2].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][6].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][10].Activate();
						return;
					}
				}
				else
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][12].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][16].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][20].Activate();
						return;
					}
				}
			}
			else if (_TechDesc.NormalCalc == kNormalMap)
			{
				if (_TechDesc.Specular)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][3].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][7].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][11].Activate();
						return;
					}
				}
				else
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][13].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][17].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][21].Activate();
						return;
					}
				}
			}
		}
		else if (_TechDesc.LightingModel == 2)
		{
			if (_TechDesc.NormalCalc == kNormalVertex)
			{
				if (_TechDesc.Specular)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][14].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][18].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][22].Activate();
						return;
					}
				}
				else
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][24].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][26].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][28].Activate();
						return;
					}
					return;
				}
			}
			else if (_TechDesc.NormalCalc == kNormalMap)
			{
				if (_TechDesc.Specular)
				{
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][15].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][19].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][23].Activate();
						return;
					}
				}
				else
				{ 
					if (_TechDesc.ColorCorr == 0)
					{
						m_vTechs[0][25].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 1)
					{
						m_vTechs[0][27].Activate();
						return;
					}
					else if (_TechDesc.ColorCorr == 2)
					{
						m_vTechs[0][29].Activate();
						return;
					}
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
