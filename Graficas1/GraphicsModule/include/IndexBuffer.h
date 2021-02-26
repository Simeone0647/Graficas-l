#pragma once
#include "Buffer.h"
class IndexBuffer : public Buffer
{
public:
	IndexBuffer();
	~IndexBuffer();

#if defined(DX11)
	inline ID3D11Buffer** GetIndexBufferAdress() { return &m_pIndexBuffer; }
	inline ID3D11Buffer* GetIndexBuffer() { return m_pIndexBuffer; }

	void UpdateBd(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride);
	inline D3D11_BUFFER_DESC* GetBdAdress() { return &m_bd; }

	void UpdateInitData(unsigned short* index);
	inline D3D11_SUBRESOURCE_DATA* GetInitDataAdress() { return &m_InitData; }
#endif
private:
#if defined(DX11)
	ID3D11Buffer* m_pIndexBuffer;

	D3D11_BUFFER_DESC m_bd;
	D3D11_SUBRESOURCE_DATA m_InitData;
#endif
};

