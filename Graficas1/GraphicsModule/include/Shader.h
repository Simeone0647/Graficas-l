#pragma once
class Shader
{
public:
	Shader();
	~Shader();

#if defined(DX11)
	void CompileShaderFromFile();
#endif
private:
	
};

