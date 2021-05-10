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

	void Render(HWND _hwnd, CBChangesEveryFrame& cb, ConstantBuffer& _MeshCB);

	#if defined(OGL)
	inline void SetTextureID(unsigned int _Value) { m_TextureID = _Value; }
	inline unsigned int GetTextureID() { return m_TextureID; }
	inline unsigned int* GetTextureIDAddress() { return &m_TextureID; }
	unsigned int m_TextureID = 0;
	#endif

	#if defined(DX11)
	inline ShaderResourceView* GetSRVTexture() { return m_SRVTexture; }
	#endif
private:

	Texture2D m_Color;
	SamplerState m_SamplerColor;

	#if defined(OGL)

	#endif

	#if defined(DX11)
	ShaderResourceView* m_SRVTexture;
	#endif
};

