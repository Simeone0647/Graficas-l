#include "Viewport.h"
#include "test.h"

Viewport::Viewport()
{
}

Viewport::~Viewport()
{
}


void Viewport::InitViewport(GraphicsModule::InitViewportStruct _InitVP)
{
#if defined(DX11)
	m_VP.Width = _InitVP.Width;
	m_VP.Height = _InitVP.Height;
	m_VP.MinDepth = _InitVP.MinDepth;
	m_VP.MaxDepth = _InitVP.MaxDepth;
	m_VP.TopLeftX = _InitVP.TopLeftX;
	m_VP.TopLeftY = _InitVP.TopLeftY;
#endif
}
