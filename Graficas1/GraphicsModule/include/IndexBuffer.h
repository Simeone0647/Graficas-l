#pragma once
#include "Buffer.h"
class IndexBuffer : public Buffer
{
public:
	IndexBuffer();
	~IndexBuffer();

#if defined(DX11)
	/*
		* @Function Name: GetIndexBufferAddress
		* @brief...This function will return the DX11 IndexBuffer's Address.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer**: IndexBuffer's Address.
	*/
	inline ID3D11Buffer** GetIndexBufferAddress() { return &m_pIndexBuffer; }

	/*
		* @Function Name: GetIndexBuffer
		* @brief...This function will return the DX11 IndexBuffer object.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer*: IndexBuffer's Object.
	*/
	inline ID3D11Buffer* GetIndexBuffer() { return m_pIndexBuffer; }

	/*
		* @Function Name: UpdateBD
		* @brief...This function will update the BufferDescriptor.
		* @param...#D3D11_USAGE: How the buffer will be used.
		* @param...#UINT: Size in bytes.
		* @param...#UINT: Bind flags
		* @param...#UINT: CPU access flags.
		* @param...#UINT: Micellaneous flags.
		* @param...#UINT:
	*/
	void UpdateBD(GraphicsModule::UpdateBDStruct _UpdateBDStruct);

	/*
		* @Function Name: GetBDAddress
		* @brief...This function will return the BD's address
		* @bug.....No known bugs.
		* @return..#D3D11_BUFFER_DESC*: BD's address.
	*/
	inline D3D11_BUFFER_DESC* GetBDAddress() { return &m_BD; }

	/*
		* @Function Name: UpdateInitData
		* @brief...This function will update Init Data.
		* @bug.....No known bugs.
	*/
	void UpdateInitData(unsigned int* index);

	/*
		* @Function Name: GetInitDataAddress
		* @brief...This function will return the InitData's address.
		* @bug.....No known bugs.
		* @return..#D3D11_SUBRESOURCE_DATA*: InitData's address.
	*/
	inline D3D11_SUBRESOURCE_DATA* GetInitDataAdress() { return &m_InitData; }
#endif
#if defined(OGL)
	inline unsigned int GetEBO() { return m_EBO; }
	inline unsigned int* GetEBOAddress() { return &m_EBO; }
#endif
private:
#if defined(DX11)
	/*
		* @Variable Name: m_pIndexBuffer;
		* @Type....#ID3D11Buffer*
		* @brief...DX11 IndexBuffer
	*/
	ID3D11Buffer* m_pIndexBuffer;

	/*
		* @Variable Name: m_BD
		* @Type....#D3D11_BUFFER_DESC
		* @brief...IndexBuffer's descriptor
	*/
	D3D11_BUFFER_DESC m_BD;

	/*
		* @Variable Name: m_InitData
		* @Type....#D3D11_SUBRESOURCE_DATA
		* @brief...IndexBuffer's InitData
	*/
	D3D11_SUBRESOURCE_DATA m_InitData;
#endif
#if defined(OGL)
	unsigned int m_EBO;
#endif
};

