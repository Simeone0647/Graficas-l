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
	inline void ActivateTech(const int _i) { m_vTechs[0][_i].Activate(); }
	void DeactivateTech();

	void AddPass(Pass _Pass);
	int GetPassNum();
	int GetPassID(const int _j);
	string GetPassName(const int _j);
	int GetActiveTech();

private:
	vector<Tech>* m_vTechs;
	string m_Name;
};

