#pragma once
#include <vector>
#include "Tech.h"

class Effect
{
public:
	Effect();
	~Effect();

private:
	std::vector<Tech> m_Techs;
};

