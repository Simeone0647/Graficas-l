#pragma once
#include "Model.h"
#include "Shader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderReflection.h"
#include "InputLayout.h"

class Pass
{
public:
	Pass();
	Pass(std::string _DefineName, std::string _DefineValue, HWND _hwnd, string _Name, int _ID);
	~Pass();

	void Render(HWND _hwnd, vector<Model>& _Models);
	void CleanUpShaders();
	inline string GetName() { return m_Name; }
	inline int GetID() { return m_ID; }
private:
	Shader m_Shader;

	string m_DefineName, m_DefineValue, m_Name;

	int m_ID;

	#if defined(DX11)
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;
	ShaderReflection m_ShaderReflection;
	InputLayout m_InputLayout;
	#endif
};

