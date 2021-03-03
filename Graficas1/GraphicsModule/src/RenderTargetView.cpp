#include "RenderTargetView.h"

RenderTargetView::RenderTargetView()
{
#if defined (DX11)
	m_pRenderTargetView = NULL;
#endif
}

RenderTargetView::~RenderTargetView()
{
}

