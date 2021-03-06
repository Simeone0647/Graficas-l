#pragma once
#include "Buffer.h"
class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

#if defined(DX11)
	/*
		* @Function Name: GetCBNeverChangesAddress
		* @brief...This function will return the CBNeverChanges's Address.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer**: CBNeverChanges's Address.
	*/
	inline ID3D11Buffer** GetCBNeverChangesAddress() { return &m_pCBNeverChanges; }

	/*
		* @Function Name: GetCBNeverChanges
		* @brief...This function will return the CBNeverChanges object.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer*: CBNeverChanges object.
	*/
	inline ID3D11Buffer* GetCBNeverChanges() { return m_pCBNeverChanges; }

	/*
		* @Function Name: GetCBChangeOnResizeAddress
		* @brief...This function will return the CBChangeOnResize's Address.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer**: CBNeverChanges's Address.
	*/
	inline ID3D11Buffer** GetCBChangeOnResizeAddress() { return &m_pCBChangeOnResize; }

	/*
		* @Function Name: GetCBChangesOnResize
		* @brief...This function will return the CBChangesOnResize object.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer*: CBChangesOnResize object.
	*/
	inline ID3D11Buffer* GetCBChangesOnResize() { return m_pCBChangeOnResize; }

	/*
		* @Function Name: GetCBChangesEveryFrameAddress
		* @brief...This function will return the CBChangesEveryFrame's Address.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer**: CBChangesEveryFrame's Address.
	*/
	inline ID3D11Buffer** GetCBChangesEveryFrameAddress() { return &m_pCBChangesEveryFrame; }

	/*
		* @Function Name: GetCBChangesEveryFrame
		* @brief...This function will return the CBChangesEveryFrame object.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer*: CBChangesEveryFrame object.
	*/
	inline ID3D11Buffer* GetCBChangesEveryFrame() { return m_pCBChangesEveryFrame; }

	/*
		* @Function Name: UpdateBD (Buffer Descriptor)
		* @brief...This function will update the Buffer Descriptor.
		* @bug.....No known bugs.
	*/
	void UpdateBD(D3D11_USAGE usage, UINT bytewidth, UINT bindflags, UINT cpuaccessflags, UINT miscflags, UINT structurebytestride);

	/*
		* @Function Name: GetBDAddress
		* @brief...This function will return the BD's Address.
		* @bug.....No known bugs.
		* @return..#D3D11_BUFFER_DESC*: BD's Address.
	*/
	inline D3D11_BUFFER_DESC* GetBDAddress() { return &m_BD; }
#endif

private:
#if defined(DX11)
	/*
		* @Variable Name: m_pCBNeverChanges
		* @Type....#1D3D11Buffer*
		* @brief...This variable is DX11 CBNeverChanges.
	*/
	ID3D11Buffer* m_pCBNeverChanges;

	/*
		* @Variable Name: m_pCBChangeOnResize
		* @Type....#1D3D11Buffer*
		* @brief...This variable is DX11 CBChangeOnResize.
	*/
	ID3D11Buffer* m_pCBChangeOnResize;

	/*
		* @Variable Name: m_pCBChangesEveryFrame
		* @Type....#1D3D11Buffer*
		* @brief...This variable is DX11 CBChangesEveryFrame.
	*/
	ID3D11Buffer* m_pCBChangesEveryFrame;

	/*
		* @Variable Name: m_BD
		* @Type....#D3D11_BUFFER_DESC
		* @brief...This variable is DX11 Buffer Descriptor..
	*/
	D3D11_BUFFER_DESC m_BD;
#endif
};

