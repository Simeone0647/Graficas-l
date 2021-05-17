#pragma once
#include <vector>
#include "Pass.h"
#include "Model.h"

enum TechTypes
{
	kIlumPerVertex = 0,
	kIlumPerPixel
};

//inline TechTypes operator|(TechTypes a, TechTypes b)
//{
//	return static_cast<TechTypes>(static_cast<int>(a) | static_cast<int>(b));
//}

//constexpr TechTypes operator|(TechTypes X, TechTypes Y) {
//	return static_cast<TechTypes>(
//		static_cast<unsigned int>(X) | static_cast<unsigned int>(Y));
//}
//
//TechTypes& operator|=(TechTypes& X, TechTypes Y) {
//	X = X | Y; return X;
//}

struct TechDesc
{
	int TechTypesFlag;
};

class Tech
{
public:
	Tech(const int _Flags, std::vector<Model> _Models);
	~Tech();

	inline void SetDesc(const int _Flags) { m_Desc.TechTypesFlag = _Flags; } 

private:
	std::vector<Pass> m_Passes;	

	TechDesc m_Desc;
};

