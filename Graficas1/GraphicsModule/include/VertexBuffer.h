#pragma once
#include "Buffer.h"
class VertexBuffer : public Buffer
{
public:
	VertexBuffer();
	~VertexBuffer();

#if defined(DX11)
	/*
		* @Function Name: GetVertexBufferAddress
		* @brief...This function will return the DX11 VertexBuffer address.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer**: VertexBuffer address
	*/
	inline ID3D11Buffer** GetVertexBufferAddress() { return &m_pVertexBuffer; }

	/*
		* @Function Name: GetVertexBuffer
		* @brief...This function will return the DX11 VertexBuffer object.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer*: VertexBuffer object
	*/
	inline ID3D11Buffer* GetVertexBuffer() { return m_pVertexBuffer; }

	/*
		* @Function Name: UpdateBD
		* @brief...This function will update the Buffer Descriptor.
		* @param...#D3D11_USAGE. The usage that the VertexBuffer will have.
		* @param...#UINT: Size in bytes.
		* @param...#UINT: Bind flags.
		* @param...#UINT: Cpu access flags.
		* @param...#UINT: Miscellaneous flags
		* @param...#UINT: 
		* @bug.....No known bugs.
	*/
	void UpdateBD(GraphicsModule::UpdateBDStruct _UpdateBDStruct);

	/*
		* @Function Name: GetBDAddress
		* @brief...This function will return the BD address.
		* @bug.....No known bugs.
		* @return..#D3D11_BUFFER_DESC*: BD address
	*/
	inline D3D11_BUFFER_DESC* GetBDAddress() { return &m_BD; }

	/*
		* @Function Name: UpdateInitData
		* @brief...This function will update the InitData.
		* @param...#void*:
		* @bug.....No known bugs.
	*/
	void UpdateInitData(void* vertex);

	/*
		* @Function Name: GetInitDataAddress
		* @brief...This function will return the InitData address.
		* @bug.....No known bugs.
		* @return..#D3D11_SUBRESOURCE_DATA*: InitData address
	*/
	inline D3D11_SUBRESOURCE_DATA* GetInitDataAddress() { return &m_InitData; }
#endif
#if defined(OGL)
	inline unsigned int* GetVAOAddress() { return &m_VAO; }
	inline unsigned int GetVAO() { return m_VAO; }

	inline unsigned int* GetVBOAddress() { return &m_VBO; }
	inline unsigned int GetVBO() { return m_VBO; }
#endif
private:
#if defined(DX11)
	ID3D11Buffer* m_pVertexBuffer;
	D3D11_BUFFER_DESC m_BD;
	D3D11_SUBRESOURCE_DATA m_InitData;
#endif
#if defined(OGL)
	unsigned int m_VAO;

	unsigned int m_VBO;
#endif
};

