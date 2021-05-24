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
	Pass(std::string _DefineName, std::string _DefineValue, HWND _hwnd);
	~Pass();

	void Init(HWND _hwnd);
	void Render(HWND _hwnd);
	void CleanUpModels();
	void CleanUpShaders();
	inline std::vector<Model>* GetModels() { return m_Models; }
	inline void SetModels(std::vector<Model>* _Models) { m_Models = _Models; }
private:
	Shader m_Shader;
	std::vector<Model>* m_Models;
	std::string m_DefineName, m_DefineValue, m_ID;

	#if defined(DX11)
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;
	ShaderReflection m_ShaderReflection;
	InputLayout m_InputLayout;
	#endif
};

