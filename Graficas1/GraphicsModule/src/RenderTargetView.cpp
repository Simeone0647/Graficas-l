#include "RenderTargetView.h"

RenderTargetView::RenderTargetView()
{
#if defined (DX11)
	m_RTVNum = 0;
#endif
}

RenderTargetView::~RenderTargetView()
{
}

