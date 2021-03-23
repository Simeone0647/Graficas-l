#pragma once
#include "Texture2D.h"
#include "SamplerState.h"
#include "test.h"

struct CBChangesEveryFrame;

class Material
{
public:
	Material();
	~Material();

	/*
		* @Function Name: Render
		* @brief...This function will set the material's color texture.
		* @param...#UINT: Texture width.
		* @bug.....No known bugs.
	*/

	void Render(HWND _hwnd, GraphicsModule::test& _obj, CBChangesEveryFrame& cb);

	inline void SetID(int _Value) { m_ID = _Value; }
	inline int GetID() { return m_ID; }

	inline Texture2D GetTexture() { return m_Color; }

private:

	Texture2D m_Color;
	SamplerState m_SamplerColor;
	int m_ID;

	#if defined(DX11)
	ID3D11ShaderResourceView* m_ColorTexture;
	#endif
	//referencia a shaderresourceview
};

