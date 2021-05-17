#pragma once
#include "Model.h"

class Pass
{
public:
	Pass();
	Pass(std::vector<Model> _Models, std::string _Defines);
	~Pass();

private:
	std::vector<Model> m_Models;
	std::string m_Defines;
};

