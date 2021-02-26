#pragma once
#include "Buffer.h"
class VertexBuffer : public Buffer
{
public:
	VertexBuffer();
	~VertexBuffer();

#if defined(DX11)
	inline ID3D11Buffer** GetVertexBufferAdress() { return &m_pVertexBuffer; }
	inline ID3D11Buffer* GetVertexBuffer() { return m_pVertexBuffer; }

	void UpdateBd(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride);
	inline D3D11_BUFFER_DESC* GetBdAdress() { return &m_bd; }

	void UpdateInitData(void* vertex);
	inline D3D11_SUBRESOURCE_DATA* GetInitDataAdress() { return &m_InitData; }
#endif
private:
#if defined(DX11)
	ID3D11Buffer* m_pVertexBuffer;
	D3D11_BUFFER_DESC m_bd;
	D3D11_SUBRESOURCE_DATA m_InitData;
#endif
};

