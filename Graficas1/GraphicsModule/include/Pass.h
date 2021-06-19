#pragma once
#include "Model.h"
#include "Shader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderReflection.h"
#include "InputLayout.h"
#include "RasterizerState.h"

class Pass
{
public:
	Pass();
	Pass(const vector<tuple<string, string>> _Macros, HWND _hwnd, string _Name);
	~Pass();

	void Render(bool _ReadSAQ, bool _ReadSkybox, bool _IsDef);
	void CleanUpShaders();
	inline string GetName() { return m_Name; }
	inline int GetID() { return m_ID; }


private:
	Shader m_Shader;

	string m_Name;
	string m_ShaderFilename;

	int m_ID;

	#if defined(DX11)
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;
	ShaderReflection m_ShaderReflection;
	InputLayout m_InputLayout;
	RasterizerState m_Rasterizer;
	#endif
};

