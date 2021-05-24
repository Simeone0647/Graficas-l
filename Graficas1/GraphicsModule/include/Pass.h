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
	Pass(std::vector<Model>* _Models, std::string _DefineName, std::string _DefineValue, HWND _hwnd);
	~Pass();

	void Init(HWND _hwnd);
	void Render(HWND _hwnd);
	void CleanUpDevice();
	inline std::vector<Model>* GetModels() { return m_Models; }
private:
	Shader m_Shader;
	std::vector<Model>* m_Models;
	std::string m_DefineName, m_DefineValue;

	#if defined(DX11)
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;
	ShaderReflection m_ShaderReflection;
	InputLayout m_InputLayout;
	#endif
};

