#include "RenderTargetView.h"

RenderTargetView::RenderTargetView()
{
#if defined (DX11)
	m_pRenderTargetView = NULL;
	m_RTVNum = 0;
#endif
}

RenderTargetView::~RenderTargetView()
{
}

