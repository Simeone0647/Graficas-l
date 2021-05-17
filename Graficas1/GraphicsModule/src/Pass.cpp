#include "Pass.h"

Pass::Pass()
{
}

Pass::Pass(std::vector<Model> _Models, std::string _Defines)
{
	m_Models = _Models;
	m_Defines = _Defines;
}

Pass::~Pass()
{
}
