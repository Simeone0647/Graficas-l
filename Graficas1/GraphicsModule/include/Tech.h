#pragma once

#include "Pass.h"

using std::vector;

enum TechTypes
{
	kVertex = 0,
	kVertexPhong,
	kVertexBlinnPhong,
	kPixel,
	kPixelNM,
	kPixelPhong,
	kPixelBlinnPhong,
	kPixelPhongNM,
	kPixelPhongNMSM,
	kPixelBlinnPhongNM,
	kPixelBlinnPhongNMSM

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
	Tech(const int _Flags, HWND _hwnd, int& _PassNum);
	~Tech();

	inline void SetDesc(const int _Flags) { m_Desc.TechTypesFlag = _Flags; } 

	void Render(HWND _hwnd, vector<Model>& _Models);
	//void CleanUpResources();
	inline vector<Pass> GetPasses() { return m_Passes; }
	inline string GetName() { return m_Name; }
	inline int GetLightingTech() { return m_Desc.TechTypesFlag; }
	inline void Activate() { m_Active = true; }
	void Deactivate();
	bool IsActivated() { return m_Active; }
	inline int GetTechType() { return m_Desc.TechTypesFlag; }

	inline void AddPass(Pass _Pass) { m_Passes.push_back(_Pass); m_PassNum++; }
	inline int GetPassNum() { return m_PassNum; }
	inline string GetPassName(const int _j) { return m_Passes[_j].GetName(); }
	inline int GetPassID(const int _j) { return m_Passes[_j].GetID(); }
private:
	vector<Pass> m_Passes;	
	vector<tuple<string, string>> m_vDefines;
	TechDesc m_Desc;
	bool m_Active;
	int m_PassNum;
	string m_Name;
};

