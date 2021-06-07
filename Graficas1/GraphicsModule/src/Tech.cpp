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
	vector<tuple<string, string>> vThirdPassDefines;
	vector<tuple<string, string>> vFourthPassDefines;

	if (_Desc.ActualEffect == kDeferred)
	{
		m_IsDef = true;

		m_vPassesNames.push_back("GBuffer");
		m_vPassesNames.push_back("GBufferLight");
		m_vPassesNames.push_back("ToneMap");
		m_vPassesNames.push_back("Copy");

		if (_Desc.DeferredFlags == kNormalVertex || _Desc.DeferredFlags == 4 || _Desc.DeferredFlags == 8 || _Desc.DeferredFlags == 12 || _Desc.DeferredFlags == 16
			|| _Desc.DeferredFlags == 20 || _Desc.DeferredFlags == 24 || _Desc.DeferredFlags == 26 || _Desc.DeferredFlags == 28)
		{
			Defines.push_back({ "NORMAL_VERTEX", "TRUE" });
			m_vDefines.push_back(Defines);

			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (_Desc.DefLightFlags == 0)
			{
				vSecondPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({"BASIC", "TRUE"});
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
			else if (_Desc.DefLightFlags == 1)
			{
				vSecondPassDefines.push_back({ "PHONG", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
			else if (_Desc.DefLightFlags == 2)
			{
				vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
		}
		else if (_Desc.DeferredFlags == kNormalMap || _Desc.DeferredFlags == 5 || _Desc.DeferredFlags == 9 || _Desc.DeferredFlags == 13 || _Desc.DeferredFlags == 17
			|| _Desc.DeferredFlags == 21 || _Desc.DeferredFlags == 25 || _Desc.DeferredFlags == 27 || _Desc.DeferredFlags == 29)
		{
			Defines.push_back({ "NORMAL_MAP", "TRUE" });
			m_vDefines.push_back(Defines);

			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (_Desc.DefLightFlags == 0)
			{
				vSecondPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
			else if (_Desc.DefLightFlags == 1)
			{
				vSecondPassDefines.push_back({ "PHONG", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
			else if (_Desc.DefLightFlags == 2)
			{
				vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
		}
		else if (_Desc.DeferredFlags == kNVSM || _Desc.DeferredFlags == 6 || _Desc.DeferredFlags == 10 || _Desc.DeferredFlags == 14 || _Desc.DeferredFlags == 18
		|| _Desc.DeferredFlags == 22)
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

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
			else if (_Desc.DefLightFlags == 1)
			{
				vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
		}
		else if (_Desc.DeferredFlags == kNMSM || _Desc.DeferredFlags == 7 || _Desc.DeferredFlags == 11 || _Desc.DeferredFlags == 15 || _Desc.DeferredFlags == 19
		|| _Desc.DeferredFlags == 23)
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

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
			else if (_Desc.DefLightFlags == 1)
			{
				vSecondPassDefines.push_back({ "BLINN_PHONG", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);
				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				if (_Desc.ColorCorr == kBasic)
				{
					vThirdPassDefines.push_back({ "BASIC", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kReinhard)
				{
					vThirdPassDefines.push_back({ "REINHARD", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
				else if (_Desc.ColorCorr == kBurgessDowson)
				{
					vThirdPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
					m_vDefines.push_back(vThirdPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));

					vFourthPassDefines.push_back({ "", "" });
					m_vDefines.push_back(vFourthPassDefines);
					m_vPasses.push_back(Pass(m_vDefines[3], _Desc.hwnd, m_vPassesNames[3]));
				}
			}
		}
	}
	else if (_Desc.ActualEffect == kForward)
	{
		m_IsDef = false;

		m_vPassesNames.push_back("Light");
		m_vPassesNames.push_back("ForwardToneMap");
		m_vPassesNames.push_back("ForwardCopy");

		if (m_Desc.TechTypesFlag == kVertex || m_Desc.TechTypesFlag == 41 || m_Desc.TechTypesFlag == 52)
		{
			Defines.push_back({ "VERTEX_LIGHT", "TRUE" });
			m_vDefines.push_back(Defines);
			m_Name = "Vertex Lighting";

			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE"});
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kVertexPhong || m_Desc.TechTypesFlag == 42 || m_Desc.TechTypesFlag == 53)
		{
			Defines.push_back({ "VERTEX_LIGHT", "TRUE" });
			Defines.push_back({ "PHONG", "TRUE" });
			m_vDefines.push_back(Defines);

			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kVertexBlinnPhong || m_Desc.TechTypesFlag == 43 || m_Desc.TechTypesFlag == 54)
		{
			Defines.push_back({ "VERTEX_LIGHT", "TRUE" });
			Defines.push_back({ "BLINN_PHONG", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixel || m_Desc.TechTypesFlag == 44 || m_Desc.TechTypesFlag == 55)
		{
			Defines.push_back({ "PIXEL_LIGHT", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelPhong || m_Desc.TechTypesFlag == 45 || m_Desc.TechTypesFlag == 56)
		{
			Defines.push_back({ "PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "PHONG", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelBlinnPhong || m_Desc.TechTypesFlag == 46 || m_Desc.TechTypesFlag == 57)
		{
			Defines.push_back({ "BLINN_PHONG", "TRUE" });
			Defines.push_back({ "PIXEL_LIGHT", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelNM || m_Desc.TechTypesFlag == 47 || m_Desc.TechTypesFlag == 58)
		{
			Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelPhongNM || m_Desc.TechTypesFlag == 48 || m_Desc.TechTypesFlag == 59)
		{
			Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "PHONG", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelPhongNMSM || m_Desc.TechTypesFlag == 49 || m_Desc.TechTypesFlag == 60)
		{
			Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "SPECULAR_MAP_PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "PHONG", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelBlinnPhongNM || m_Desc.TechTypesFlag == 50 || m_Desc.TechTypesFlag == 61)
		{
			Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "BLINN_PHONG", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
		else if (m_Desc.TechTypesFlag == kPixelBlinnPhongNMSM || m_Desc.TechTypesFlag == 51 || m_Desc.TechTypesFlag == 62)
		{
			Defines.push_back({ "NORMAL_MAP_PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "SPECULAR_MAP_PIXEL_LIGHT", "TRUE" });
			Defines.push_back({ "BLINN_PHONG", "TRUE" });
			m_vDefines.push_back(Defines);
			
			m_vPasses.push_back(Pass(m_vDefines[0], _Desc.hwnd, m_vPassesNames[0]));

			if (m_Desc.ColorCorr == kBasic)
			{
				vSecondPassDefines.push_back({ "BASIC", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kReinhard)
			{
				vSecondPassDefines.push_back({ "REINHARD", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
			else if (m_Desc.ColorCorr == kBurgessDowson)
			{
				vSecondPassDefines.push_back({ "BURGESS_DOWSON", "TRUE" });
				m_vDefines.push_back(vSecondPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[1], _Desc.hwnd, m_vPassesNames[1]));

				vThirdPassDefines.push_back({ "", "" });
				m_vDefines.push_back(vThirdPassDefines);

				m_vPasses.push_back(Pass(m_vDefines[2], _Desc.hwnd, m_vPassesNames[2]));
			}
		}
	}

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
			if (i == 0)
			{
				RM::GetRenderManager().SetGBufferRTV();
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().GBufferRasterState.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, false);
			}
			else if (i == 1)
			{
				RM::GetRenderManager().SetGBufferToneRTV();
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().GBufferLightRasterState.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, true);
			}
			else if (i == 2)
			{
				RM::GetRenderManager().SetGBufferCopyRTV();
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().ToneRasterizer.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, true);
			}
			else if (i == m_PassNum - 1)
			{
				if (!RM::GetRenderManager().IsBackBufferCleaned())
				{ 
					RM::GetRenderManager().SetBackBuffer();
					RM::GetRenderManager().SetBackBufferCleaned(true);
				}
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().CopyRasterizer.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, true);
			}
		}
		else
		{ 
			if (i == 0)
			{
				RM::GetRenderManager().SetForwardLightRTV();
				m_vPasses[i].Render(_hwnd, _Models, false);
			}
			else if (i == 1)
			{
				RM::GetRenderManager().SetForwardToneMapRTV();
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().ToneRasterizer.GetRS());
				m_vPasses[i].Render(_hwnd, _Models, true);
			}
			else if (i == m_PassNum - 1)
			{
				GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetState(RM::GetRenderManager().ToneRasterizer.GetRS());
				if (!RM::GetRenderManager().IsBackBufferCleaned())
				{
					RM::GetRenderManager().SetBackBuffer();
					RM::GetRenderManager().SetBackBufferCleaned(true);
				}
				m_vPasses[i].Render(_hwnd, _Models, true);
			}
		}
	}
}

void Tech::Deactivate()
{
	m_Active = false;
}