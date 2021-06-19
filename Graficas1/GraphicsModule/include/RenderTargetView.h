#pragma once
#include "Texture2D.h"
#include <vector>

using std::vector;

class RenderTargetView : public Texture2D
{
public:
	RenderTargetView();
	~RenderTargetView();

#if defined(DX11)
	inline ID3D11RenderTargetView** GetRTVAdress() { return m_vRTVs.data(); }

	inline ID3D11RenderTargetView** GetLastElementAddress() { return &m_vRTVs.back(); }

	inline void AddRTV() { ID3D11RenderTargetView* RTV = NULL; m_vRTVs.push_back(RTV); m_RTVNum++; }

	inline ID3D11RenderTargetView* GetRTV(const int _i) { return m_vRTVs[_i]; }

	inline int GetRTVNum() { return m_RTVNum; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_vRTVs.
		* @Type....#vector<ID3D11RenderTargetView*>.
		* @brief...Vector of DX11 RTVs.
	*/
	vector<ID3D11RenderTargetView*> m_vRTVs;

	int m_RTVNum;
#endif
};

