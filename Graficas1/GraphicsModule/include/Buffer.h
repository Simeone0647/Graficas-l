#pragma once
#if defined(DX11)

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#endif

namespace GraphicsModule
{
	enum SIME_USAGE;
	enum SIME_BIND_FLAG;
	struct UpdateBDStruct;
}

class Buffer
{
public:
	Buffer();
	~Buffer();
	
#if defined(DX11)
	/*
		* @Function Name: GetBufferAddress
		* @brief...This function will return the DX11 Buffer address.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer**: Buffer address
	*/
	inline ID3D11Buffer** BGetBufferAddress() { return &m_pBuffer; }

	/*
		* @Function Name: GetBuffer
		* @brief...This function will return the DX11 Buffer object.
		* @bug.....No known bugs.
		* @return..#ID3D11Buffer*: Buffer object
	*/
	inline ID3D11Buffer* BGetBuffer() { return m_pBuffer; }

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
	void BUpdateBD(GraphicsModule::UpdateBDStruct _BDStruct);

	/*
		* @Function Name: GetBDAddress
		* @brief...This function will return the BD address.
		* @bug.....No known bugs.
		* @return..#D3D11_BUFFER_DESC*: BD address
	*/
	inline D3D11_BUFFER_DESC* BGetBDAddress() { return &m_BD; }
#endif

private:
#if defined(DX11)
	ID3D11Buffer* m_pBuffer;
	D3D11_BUFFER_DESC m_BD;
#endif
};

