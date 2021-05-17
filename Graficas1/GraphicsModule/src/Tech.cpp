#include "Tech.h"

Tech::Tech(const int _Flags, std::vector<Model> _Models)
{
	std::string Defines = "";

	m_Desc.TechTypesFlag = _Flags;
	if (m_Desc.TechTypesFlag == 0) Defines += "#define VERTEX_LIGHT";
	m_Passes.push_back(Pass(_Models, Defines));
}

Tech::~Tech()
{
}
