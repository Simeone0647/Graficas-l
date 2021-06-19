#pragma once
#include "Pass.h"

enum ColorCorrection
{
	kBasic = 0,
	kReinhard,
	kBurgessDowson
};

enum Effects
{
	kDeferred = 0,
	kForward
};

enum DeferredDefines
{
	kNormalVertex = 0,
	kNormalMap,
	kNVSM,
	kNMSM,
	kDefPhong,
	kDefBlinnPhong
};

enum TechTypes
{
	kVertex = 30,
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
	int DeferredFlags;
	int DefLightFlags;
	HWND hwnd;
	int PassNum;
	int EffectNum;
	int ActualEffect;
	int ColorCorr;
};

struct SetTechDesc
{
	bool ForwardRender = false;
	int LightingPlace;

	bool DeferredRender = false;
	int LightingModel;
	int NormalCalc;
	bool Specular;
	int Features;
	int ColorCorr;
};

class Tech
{
public:
	Tech(const TechDesc _Desc);
	~Tech();

	inline void SetDesc(const int _Flags) { m_Desc.TechTypesFlag = _Flags; } 

	void Render();
	inline vector<Pass> GetPasses() { return m_vPasses; }
	inline string GetName() { return m_Name; }
	inline int GetLightingTech() { return m_Desc.TechTypesFlag; }
	inline void Activate() { m_Active = true; }
	void Deactivate();
	bool IsActivated() { return m_Active; }
	inline int GetTechType() { return m_Desc.TechTypesFlag; }

	inline void AddPass(Pass _Pass) { m_vPasses.push_back(_Pass); m_PassNum++; }
	inline int GetPassNum() { return m_PassNum; }
	inline string GetPassName(const int _j) { return m_vPasses[_j].GetName(); }
	inline int GetPassID(const int _j) { return m_vPasses[_j].GetID(); }
private:

	vector<Pass> m_vPasses;	
	vector<vector<tuple<string, string>>> m_vDefines;
	vector<string> m_vPassesNames;
	TechDesc m_Desc;
	bool m_Active;
	bool m_IsDef;
	int m_PassNum;
	string m_Name;
};

