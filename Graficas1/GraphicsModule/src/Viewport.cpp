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
	m_VP.Width = width;
	m_VP.Height = height;
	m_VP.MinDepth = 0.0f;
	m_VP.MaxDepth = 1.0f;
	m_VP.TopLeftX = 0;
	m_VP.TopLeftY = 0;
#endif
}
