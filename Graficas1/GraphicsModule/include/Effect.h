#pragma once
#include <string>
#include <vector>
#include "Tech.h"

using std::vector;
using std::string;

class Effect
{
public:
	Effect(std::vector<Tech>* _Techs, string _Name);
	~Effect();

	void Render(HWND _hwnd, vector<Model>& _Models);
	inline string GetName() { return m_Name; }
	void ActivateTech(const int _i);
	void DeactivateTech();

	void AddPass(Pass _Pass);
	int GetPassNum();
	int GetPassID(const int _j);
	string GetPassName(const int _j);
	int GetActiveTech();

	bool m_ImGuiNormalMap;

private:
	vector<Tech>* m_vTechs;
	string m_Name;
};

