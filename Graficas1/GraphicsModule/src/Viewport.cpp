#include "Viewport.h"

Viewport::Viewport()
{
}

Viewport::~Viewport()
{
}


void Viewport::InitViewport(float width, float height, float mindepth, float maxdepth, float topleftx, float toplefty)
{
#if defined(DX11)
	m_vp.Width = width;
	m_vp.Height = height;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = 0;
#endif
}
