#include "VertexShader.h"

VertexShader::VertexShader()
{
#if defined(DX11)
	m_pVertexShader = NULL;
#endif
}

VertexShader::~VertexShader()
{
}
