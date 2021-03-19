#include "PixelShader.h"

PixelShader::PixelShader()
{
#if defined(DX11)
	m_pPixelShader = NULL;
#endif
}

PixelShader::~PixelShader()
{

}


