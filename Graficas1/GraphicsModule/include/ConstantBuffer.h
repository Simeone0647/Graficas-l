#pragma once
#include "Buffer.h"
class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

#if defined(DX11)
	inline ID3D11Buffer** GetCBNeverChangesAdress() { return &m_pCBNeverChanges; }
	inline ID3D11Buffer* GetCBNeverChanges() { return m_pCBNeverChanges; }

	inline ID3D11Buffer** GetCBChangeOnResizeAdress() { return &m_pCBChangeOnResize; }
	inline ID3D11Buffer* GetCBChangesOnResize() { return m_pCBChangeOnResize; }

	inline ID3D11Buffer** GetCBChangesEveryFrameAdress() { return &m_pCBChangesEveryFrame; }
	inline ID3D11Buffer* GetCBChangesEveryFrame() { return m_pCBChangesEveryFrame; }

	void UpdateBd(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride);
	inline D3D11_BUFFER_DESC* GetBdAdress() { return &m_bd; }
#endif
private:
#if defined(DX11)
	ID3D11Buffer* m_pCBNeverChanges;
	ID3D11Buffer* m_pCBChangeOnResize;
	ID3D11Buffer* m_pCBChangesEveryFrame;
	D3D11_BUFFER_DESC m_bd;
#endif
};

