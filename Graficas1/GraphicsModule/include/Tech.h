#pragma once
#include <vector>
#include "Pass.h"
#include "Model.h"

enum TechTypes
{
	kNone = 0,
	kIlumPerVertex,
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
	Tech(const int _Flags, std::vector<Model>* _Models, HWND _hwnd);
	~Tech();

	inline void SetDesc(const int _Flags) { m_Desc.TechTypesFlag = _Flags; } 

	void Render(HWND _hwnd);
	void CleanUpResources();
	inline std::vector<Pass> GetPasses() { return m_Passes; }
	inline std::string GetName() { return m_Name; }
	inline int GetLightingTech() { return m_Desc.TechTypesFlag; }
private:
	std::vector<Pass> m_Passes;	

	TechDesc m_Desc;
	std::string m_DefineValue, m_DefineName, m_Name;
};

