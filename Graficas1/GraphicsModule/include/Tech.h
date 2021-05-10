#pragma once
#include <vector>
#include "Pass.h"

class Tech
{
public:
	Tech();
	~Tech();

private:
	std::vector<Pass> m_Effects;	
};

