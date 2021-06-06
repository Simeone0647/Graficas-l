#pragma once
#include <string>
#include <vector>
#include "Tech.h"

using std::vector;
using std::string;

class Effect
{
public:
	Effect(std::vector<Tech>* _Techs, string _Name, const SetTechDesc _TechDesc);
	~Effect();

	void Render(HWND _hwnd, vector<Model>& _Models);
	inline string GetName() { return m_Name; }
	void ActivateIlumTechPlace(const int _LightPlace); 
	void ActivateTech(const SetTechDesc _TechDesc);
	void DeactivateTech();

	void AddPass(Pass _Pass);
	int GetPassNum();
	int GetPassID(const int _j);
	string GetPassName(const int _j);
	int GetActiveTech();

	//inline bool IsForward() { return m_Forward; }
	//inline bool IsDeferred() { return m_Deferred; }
	//inline void SetForward(const bool _Value) { m_Forward = _Value; }
	//inline void SetDeferred(const bool _Value) { m_Deferred = _Value; }
	
	inline void SetRenderTech(const unsigned int _NewTech) { m_ActiveTech = _NewTech; }
	inline unsigned int GetActiveRenderTech() { return m_ActiveTech; }
	bool m_ImGuiNormalMap, m_ImGuiSpecularMap;

	bool m_ImGuiDefNormalVertex, m_ImGuiDefNormalMap, ImGuiDefSM;
	int LastFeature;
private:
	vector<Tech>* m_vTechs;

	//bool m_Deferred;
	//bool m_Forward;

	unsigned int m_ActiveTech;

	string m_Name;
};

