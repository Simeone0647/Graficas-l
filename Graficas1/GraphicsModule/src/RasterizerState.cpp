#include "RasterizerState.h"
#include "test.h"

RasterizerState::RasterizerState()
{
}

RasterizerState::~RasterizerState()
{
}

#if defined(DX11)
void RasterizerState::SetRasterizerDesc(GraphicsModule::SetRasterizerStruct _RasterizerStruct)
{
	m_RasterStateDesc.CullMode = (D3D11_CULL_MODE)_RasterizerStruct.Cull;
	m_RasterStateDesc.FillMode = (D3D11_FILL_MODE)_RasterizerStruct.Fill;
	m_RasterStateDesc.FrontCounterClockwise = _RasterizerStruct.FrontCCW;
}
#endif
