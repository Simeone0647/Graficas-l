#include "test.h"

namespace GraphicsModule
{
#if defined(DX11)
	HRESULT test::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

		#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;
		#endif

		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFileA(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;

}
#endif
HRESULT test::InitDevice(HWND hwnd)
{
#if defined(DX11)
		m_hwnd = hwnd;
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;
		g_DeviceAndSwapChainDesc.CreateDeviceFlags = 0;
#ifdef _DEBUG
		g_DeviceAndSwapChainDesc.CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		g_DeviceAndSwapChainDesc.FeatureLevels[0] = D3D_FEATURE_LEVEL_11_0;
		g_DeviceAndSwapChainDesc.FeatureLevels[1] = D3D_FEATURE_LEVEL_10_1;
		g_DeviceAndSwapChainDesc.FeatureLevels[2] = D3D_FEATURE_LEVEL_10_0;

		g_DeviceAndSwapChainDesc.NumFeatureLevels = ARRAYSIZE(g_DeviceAndSwapChainDesc.FeatureLevels);

		ZeroMemory(&g_DeviceAndSwapChainDesc.sd, sizeof(g_DeviceAndSwapChainDesc.sd));
		g_DeviceAndSwapChainDesc.sd.BufferCount = 1;
		g_DeviceAndSwapChainDesc.sd.BufferDesc.Width = width;
		g_DeviceAndSwapChainDesc.sd.BufferDesc.Height = height;
		g_DeviceAndSwapChainDesc.sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		g_DeviceAndSwapChainDesc.sd.BufferDesc.RefreshRate.Numerator = 60;
		g_DeviceAndSwapChainDesc.sd.BufferDesc.RefreshRate.Denominator = 1;
		g_DeviceAndSwapChainDesc.sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		g_DeviceAndSwapChainDesc.sd.OutputWindow = m_hwnd;
		g_DeviceAndSwapChainDesc.sd.SampleDesc.Count = 1;
		g_DeviceAndSwapChainDesc.sd.SampleDesc.Quality = 0;
		g_DeviceAndSwapChainDesc.sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			g_DeviceAndSwapChainDesc.driverType = driverTypes[driverTypeIndex];
			hr = GetManagerObj(m_hwnd).CreateDeviceAndSwapChain(g_DeviceAndSwapChainDesc);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;
			
		// Create a render target view
		Texture2D BackBuffer;
		hr = GetManagerObj(hwnd).GetSwapChain().CGetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)BackBuffer.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		hr = GetManagerObj(hwnd).GetDevice().CCreateRenderTargetView(BackBuffer.GetTexture(), NULL, g_SimeRenderTargetView.GetRTVAdress());
		BackBuffer.GetTexture()->Release();
		if (FAILED(hr))
			return hr;


		g_SimeDepthStencil.SetDescDepth(width, height, 1, 1, DXGI_FORMAT_R32_TYPELESS, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateTexture2D(g_SimeDepthStencil.GetDescDepthAddress(), NULL, g_SimeDepthStencil.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		g_SimeDepthStencilView.SetDescDSV(DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateDepthStencilView(g_SimeDepthStencil.GetTexture(), g_SimeDepthStencilView.GetDescDSVAddress(),
																				   g_SimeDepthStencilView.GetDSVAddress());
		if (FAILED(hr))
			return hr;

		// and the resource view for the shader
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1; // same as orig texture
		hr = GetManagerObj(hwnd).GetDevice().CCreateShaderResourceView(g_SimeDepthStencil.GetTexture(), &srvDesc, &g_pDepthStencilSRV);
		if (FAILED(hr))
			return hr;

		// Setup the viewport
		g_SimeViewport.InitViewport((FLOAT)width, (FLOAT)height, 0.0f, 1.0f, 0.0f, 0.0f);

		// Compile the vertex shader
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL,
				"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			return hr;
		}

		// Create the vertex shader
		hr = GetManagerObj(hwnd).GetDevice().CCreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}

		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = GetManagerObj(hwnd).GetDevice().CCreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(), &g_pVertexLayout);
		pVSBlob->Release();
		if (FAILED(hr))
			return hr;

		// Compile the pixel shader
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL,
				"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			return hr;
		}

		// Create the pixel shader
		hr = GetManagerObj(hwnd).GetDevice().CCreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;

		// Create vertex buffer
		CubeVertex[0].SetPosition(-1.0f, 1.0f, -1.0f);
		CubeVertex[0].SetTexture(0.0f, 0.0f);
		CubeVertex[0].SetNormal(0.0f, 1.0f, 0.0f);
		CubeVertex[1].SetPosition(1.0f, 1.0f, -1.0f);
		CubeVertex[1].SetTexture(1.0f, 0.0f);
		CubeVertex[1].SetNormal(0.0f, 1.0f, 0.0f);
		CubeVertex[2].SetPosition(1.0f, 1.0f, 1.0f);
		CubeVertex[2].SetTexture(1.0f, 1.0f);
		CubeVertex[2].SetNormal(0.0f, 1.0f, 0.0f);
		CubeVertex[3].SetPosition(-1.0f, 1.0f, 1.0f);
		CubeVertex[3].SetTexture(0.0f, 1.0f);
		CubeVertex[3].SetNormal(0.0f, 1.0f, 0.0f);

		CubeVertex[4].SetPosition(-1.0f, -1.0f, -1.0f);
		CubeVertex[4].SetTexture(0.0f, 0.0f);
		CubeVertex[4].SetNormal(1.0f, -1.0f, 0.0f);
		CubeVertex[5].SetPosition(1.0f, -1.0f, -1.0f);
		CubeVertex[5].SetTexture(1.0f, 0.0f);
		CubeVertex[5].SetNormal(1.0f, -1.0f, 0.0f);
		CubeVertex[6].SetPosition(1.0f, -1.0f, 1.0f);
		CubeVertex[6].SetTexture(1.0f, 1.0f);
		CubeVertex[6].SetNormal(1.0f, -1.0f, 0.0f);
		CubeVertex[7].SetPosition(-1.0f, -1.0f, 1.0f);
		CubeVertex[7].SetTexture(0.0f, 1.0f);
		CubeVertex[7].SetNormal(1.0f, -1.0f, 0.0f);

		CubeVertex[8].SetPosition(-1.0f, -1.0f, 1.0f);
		CubeVertex[8].SetTexture(0.0f, 0.0f);
		CubeVertex[8].SetNormal(-1.0f, 0.0f, 0.0f);
		CubeVertex[9].SetPosition(-1.0f, -1.0f, -1.0f);
		CubeVertex[9].SetTexture(1.0f, 0.0f);
		CubeVertex[9].SetNormal(-1.0f, 0.0f, 0.0f);
		CubeVertex[10].SetPosition(-1.0f, 1.0f, -1.0f);
		CubeVertex[10].SetTexture(1.0f, 1.0f);
		CubeVertex[10].SetNormal(-1.0f, 0.0f, 0.0f);
		CubeVertex[11].SetPosition(-1.0f, 1.0f, 1.0f);
		CubeVertex[11].SetTexture(0.0f, 1.0f);
		CubeVertex[11].SetNormal(-1.0f, 0.0f, 0.0f);

		CubeVertex[12].SetPosition(1.0f, -1.0f, 1.0f);
		CubeVertex[12].SetTexture(0.0f, 0.0f);
		CubeVertex[12].SetNormal(1.0f, 0.0f, 0.0f);
		CubeVertex[13].SetPosition(1.0f, -1.0f, -1.0f);
		CubeVertex[13].SetTexture(1.0f, 0.0f);
		CubeVertex[13].SetNormal(1.0f, 0.0f, 0.0f);
		CubeVertex[14].SetPosition(1.0f, 1.0f, -1.0f);
		CubeVertex[14].SetTexture(1.0f, 1.0f);
		CubeVertex[14].SetNormal(1.0f, 0.0f, 0.0f);
		CubeVertex[15].SetPosition(1.0f, 1.0f, 1.0f);
		CubeVertex[15].SetTexture(0.0f, 1.0f);
		CubeVertex[15].SetNormal(1.0f, 0.0f, 0.0f);

		CubeVertex[16].SetPosition(-1.0f, -1.0f, -1.0f);
		CubeVertex[16].SetTexture(0.0f, 0.0f);
		CubeVertex[16].SetNormal(0.0f, 0.0f, -1.0f);
		CubeVertex[17].SetPosition(1.0f, -1.0f, -1.0f);
		CubeVertex[17].SetTexture(1.0f, 0.0f);
		CubeVertex[17].SetNormal(0.0f, 0.0f, -1.0f);
		CubeVertex[18].SetPosition(1.0f, 1.0f, -1.0f);
		CubeVertex[18].SetTexture(1.0f, 1.0f);
		CubeVertex[18].SetNormal(0.0f, 0.0f, -1.0f);
		CubeVertex[19].SetPosition(-1.0f, 1.0f, -1.0f);
		CubeVertex[19].SetTexture(0.0f, 1.0f);
		CubeVertex[19].SetNormal(0.0f, 0.0f, -1.0f);

		CubeVertex[20].SetPosition(-1.0f, -1.0f, 1.0f);
		CubeVertex[20].SetTexture(0.0f, 0.0f);
		CubeVertex[20].SetNormal(0.0f, 0.0f, 1.0f);
		CubeVertex[21].SetPosition(1.0f, -1.0f, 1.0f);
		CubeVertex[21].SetTexture(1.0f, 0.0f);
		CubeVertex[21].SetNormal(0.0f, 0.0f, 1.0f);
		CubeVertex[22].SetPosition(1.0f, 1.0f, 1.0f);
		CubeVertex[22].SetTexture(1.0f, 1.0f);
		CubeVertex[22].SetNormal(0.0f, 0.0f, 1.0f);
		CubeVertex[23].SetPosition(-1.0f, 1.0f, 1.0f);
		CubeVertex[23].SetTexture(0.0f, 1.0f);
		CubeVertex[23].SetNormal(0.0f, 0.0f, 1.0f);

		FirstCube.SetVertex(CubeVertex, 24);
		FirstCube.SetNumOfVertex(24);

		SecondCube.SetVertex(CubeVertex, 24);
		SecondCube.SetNumOfVertex(24);

		ThirdCube.SetVertex(CubeVertex, 24);
		ThirdCube.SetNumOfVertex(24);

		FourthCube.SetVertex(CubeVertex, 24);
		FourthCube.SetNumOfVertex(24);

		g_SimeVertexBuffer.UpdateBD(D3D11_USAGE_DEFAULT, CubeVertex->GetSize() * FirstCube.GetNumOfVertex(), D3D11_BIND_VERTEX_BUFFER, 0, 0, 0);

		g_SimeVertexBuffer.UpdateInitData(FirstCube.GetVertex());

		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeVertexBuffer.GetBDAddress(), g_SimeVertexBuffer.GetInitDataAddress(),
						   g_SimeVertexBuffer.GetVertexBufferAddress());
		if (FAILED(hr))
			return hr;

		// Create index buffer

		CubeVertexIndex[0] = 3;
		CubeVertexIndex[1] = 1;
		CubeVertexIndex[2] = 0;
		CubeVertexIndex[3] = 2;
		CubeVertexIndex[4] = 1;
		CubeVertexIndex[5] = 3;
		CubeVertexIndex[6] = 6;
		CubeVertexIndex[7] = 4;
		CubeVertexIndex[8] = 5;
		CubeVertexIndex[9] = 7;
		CubeVertexIndex[10] = 4;
		CubeVertexIndex[11] = 6;
		CubeVertexIndex[12] = 11;
		CubeVertexIndex[13] = 9;
		CubeVertexIndex[14] = 8;
		CubeVertexIndex[15] = 10;
		CubeVertexIndex[16] = 9;
		CubeVertexIndex[17] = 11;
		CubeVertexIndex[18] = 14;
		CubeVertexIndex[19] = 12;
		CubeVertexIndex[20] = 13;
		CubeVertexIndex[21] = 15;
		CubeVertexIndex[22] = 12;
		CubeVertexIndex[23] = 14;
		CubeVertexIndex[24] = 19;
		CubeVertexIndex[25] = 17;
		CubeVertexIndex[26] = 16;
		CubeVertexIndex[27] = 18;
		CubeVertexIndex[28] = 17;
		CubeVertexIndex[29] = 19;
		CubeVertexIndex[30] = 22;
		CubeVertexIndex[31] = 20;
		CubeVertexIndex[32] = 21;
		CubeVertexIndex[33] = 23;
		CubeVertexIndex[34] = 20;
		CubeVertexIndex[35] = 22;

		//SET ALL FOUR MESHES VERTEX INDEX
		FirstCube.SetVertexIndex(CubeVertexIndex, 36);
		SecondCube.SetVertexIndex(CubeVertexIndex, 36);
		ThirdCube.SetVertexIndex(CubeVertexIndex, 36);
		FourthCube.SetVertexIndex(CubeVertexIndex, 36);

		//SET NUM OF VERTEX INDEX
		FirstCube.SetNumOfVertexIndex(36);
		SecondCube.SetNumOfVertexIndex(36);
		ThirdCube.SetNumOfVertexIndex(36);
		FourthCube.SetNumOfVertexIndex(36);

		g_SimeIndexBuffer.UpdateBD(D3D11_USAGE_DEFAULT, FirstCube.GetVertexIndexSize() , D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
		g_SimeIndexBuffer.UpdateInitData(FirstCube.GetVertexIndex());
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeIndexBuffer.GetBDAddress(), g_SimeIndexBuffer.GetInitDataAdress(), g_SimeIndexBuffer.GetIndexBufferAddress());
		if (FAILED(hr))
			return hr;

		// Set primitive topology
		//GetManagerObj(hwnd).GetDeviceContext().CIASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_SimeCBNeverChanges.UpdateBD(D3D11_USAGE_DEFAULT, sizeof(CBNeverChanges), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBNeverChanges.GetBDAddress(), NULL, g_SimeCBNeverChanges.GetCBNeverChangesAddress());
		if (FAILED(hr))
			return hr;

		g_SimeCBChangeOnResize.UpdateBD(D3D11_USAGE_DEFAULT, sizeof(CBChangeOnResize), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBChangeOnResize.GetBDAddress(), NULL, g_SimeCBChangeOnResize.GetCBChangeOnResizeAddress());
		if (FAILED(hr))
			return hr;

		g_SimeCBChangesEveryFrame.UpdateBD(D3D11_USAGE_DEFAULT, sizeof(CBChangesEveryFrame), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBChangesEveryFrame.GetBDAddress(), NULL, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
		if (FAILED(hr))
			return hr;

		g_DirLightBuffer.BUpdateBD(D3D11_USAGE_DEFAULT, sizeof(DirLight), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_DirLightBuffer.BGetBDAddress(), NULL, g_DirLightBuffer.BGetBufferAddress());

		// Load the Texture
		hr = D3DX11CreateShaderResourceViewFromFile(GetManagerObj(hwnd).GetDevice().GetDXDevice(), "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
		if (FAILED(hr))
			return hr;

		hr = D3DX11CreateShaderResourceViewFromFile(GetManagerObj(hwnd).GetDevice().GetDXDevice(), "viejosabroso.dds", NULL, NULL, &g_pTextureRVViejoSabroso, NULL);
		if (FAILED(hr))
			return hr;

		// Create the sample state
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = GetManagerObj(hwnd).GetDevice().CCreateSamplerState(&sampDesc, &g_pSamplerLinear);
		if (FAILED(hr))
			return hr;

		// Initialize the world matrices
		g_World = XMMatrixIdentity();

		m_PerspectiveCamera.SetEye(0.0f, 3.0f, -6.0f);
		m_PerspectiveCamera.SetAt(0.0f, 0.0f, 0.0f);
		m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
		m_PerspectiveCamera.UpdateViewMatrix();
		m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

		m_OrtographicCamera.SetEye(0.0f, 3.0f, -6.0f);
		m_OrtographicCamera.SetAt(0.0f, 0.0f, 0.0f);
		m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
		m_OrtographicCamera.UpdateViewMatrix();
		m_OrtographicCamera.UpdateOrtographicProjectionMatrix(width, height, 0.01f, 100.0f);

		m_Camera = &m_PerspectiveCamera;

		// create rasterizer state
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		hr = GetManagerObj(hwnd).GetDevice().CCreateRasterizerState(&desc, &g_Rasterizer);
		if (FAILED(hr))
			return hr;

		/* FIRST RENDER TARGET TEXTURE
			CREATE TEXTURE*/
		g_TextureRenderTarget;
		g_TextureRenderTarget.SetDescRT(width, height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(g_TextureRenderTarget.GetDescDepthAddress(), NULL, g_TextureRenderTarget.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		/*CREATE SHADER RESOURCE VIEW DESCRIPTOR FOR RENDER TARGET TEXTURE*/
		D3D11_SHADER_RESOURCE_VIEW_DESC descViewRT;
		ZeroMemory(&descViewRT, sizeof(descViewRT));
		descViewRT.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descViewRT.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descViewRT.Texture2D.MostDetailedMip = 0;
		descViewRT.Texture2D.MipLevels = 1;
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(g_TextureRenderTarget.GetTexture(), &descViewRT, &g_pViewRT2);
		if (FAILED(hr))
			return hr;
		/*CREATE RENDER TARGET*/
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(g_TextureRenderTarget.GetTexture(), NULL, g_SimeSecondCubeRTV.GetRTVAdress());
		if (FAILED(hr))
			return hr;



		g_TextureRenderTarget2;
		g_TextureRenderTarget2.SetDescRT(width, height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(g_TextureRenderTarget2.GetDescDepthAddress(), NULL, g_TextureRenderTarget2.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		D3D11_SHADER_RESOURCE_VIEW_DESC descViewRT2;
		ZeroMemory(&descViewRT2, sizeof(descViewRT2));
		descViewRT2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descViewRT2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descViewRT2.Texture2D.MostDetailedMip = 0;
		descViewRT2.Texture2D.MipLevels = 1;
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(g_TextureRenderTarget2.GetTexture(), &descViewRT2, &g_pViewRT3);
		if (FAILED(hr))
			return hr;

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(g_TextureRenderTarget2.GetTexture(), NULL, g_SecondSimeRenderTargetView.GetRTVAdress());
		if (FAILED(hr))
			return hr;



		g_TextureRenderTarget3;
		g_TextureRenderTarget3.SetDescRT(width, height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(g_TextureRenderTarget3.GetDescDepthAddress(), NULL, g_TextureRenderTarget3.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		D3D11_SHADER_RESOURCE_VIEW_DESC descViewRT3;
		ZeroMemory(&descViewRT3, sizeof(descViewRT3));
		descViewRT3.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		descViewRT3.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descViewRT3.Texture2D.MostDetailedMip = 0;
		descViewRT3.Texture2D.MipLevels = 1;
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(g_TextureRenderTarget3.GetTexture(), &descViewRT3, &g_pViewRT4);
		if (FAILED(hr))
			return hr;

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(g_TextureRenderTarget3.GetTexture(), NULL, g_ThirdSimeRenderTargetView.GetRTVAdress());
		if (FAILED(hr))
			return hr;
#endif
		return S_OK;
}

void test::Update()
{
#if defined(DX11)
	static float t = 0.0f;
	int counter = 0;

	if (m_IsFirstFrame)
	{
		if (!m_IsFirstPosStored)
		{
			GetCursorPos(MouseInitPos);
			m_IsFirstPosStored = true;
		}
		else
		{
			GetCursorPos(MouseFinalPos);
			LPPOINT Direction = new POINT;
			Direction->x = MouseFinalPos->x - MouseInitPos->x;
			Direction->y = MouseFinalPos->y - MouseInitPos->y;
			Vector3 RotateVector;
			RotateVector.SetValues(Direction->x, Direction->y, 0.0f);
			m_Camera->RotateCamera(RotateVector);
			m_Camera->UpdateViewMatrix();
			m_IsFirstPosStored = false;
			if (Direction != nullptr)
			{
				delete Direction;
			}
		}
	}

	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	// Rotate cube around the origin
	g_Translation.m[0][3] = 0;
	g_Translation.m[1][3] = 0;
	g_Translation.m[2][3] = 0;

	//FIRST CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FirstCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	g_World = g_Translation;
	g_World = XMMatrixRotationY(t);
	g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FirstCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	//SECOND CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SecondCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	g_World = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SecondCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	//THIRD CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ThirdCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	g_World = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ThirdCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	//FOURTH CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FourthCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	g_World = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FourthCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	// Modify the color
	g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SimeRenderTargetView.GetRTV(), ClearColor);
	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SimeSecondCubeRTV.GetRTV(), ClearColor);
	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SecondSimeRenderTargetView.GetRTV(), ClearColor);
	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_ThirdSimeRenderTargetView.GetRTV(), ClearColor);
	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(g_SimeDepthStencilView.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(m_Camera->GetViewMatrix());
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBNeverChanges.GetCBNeverChanges(), 0, NULL, &cbNeverChanges, 0, 0);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangeOnResize.GetCBChangesOnResize(), 0, NULL, &cbChangesOnResize, 0, 0);
	//g_CUpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

	if (m_IsPerspectiveActive)
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangeOnResize.GetCBChangesOnResize(), 0, NULL, &cbChangesOnResize, 0, 0);
	}
	else
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetOrtographicProjectionMatrix());
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangeOnResize.GetCBChangesOnResize(), 0, NULL, &cbChangesOnResize, 0, 0);
	}

#endif
}

void test::Render()
{
#if defined(DX11)

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		CBChangesEveryFrame cb;
		cb.mWorld = XMMatrixTranspose(FirstCube.GetWorldMatrix());
		cb.vMeshColor = g_vMeshColor;
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);

		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_DirLightBuffer.BGetBuffer(), 0, NULL, &g_DirLightBufferDesc, 0, 0);

		GetManagerObj(m_hwnd).GetDeviceContext().CRSSetViewports(1, g_SimeViewport.GetViewportAddress());
		GetManagerObj(m_hwnd).GetDeviceContext().CIASetInputLayout(g_pVertexLayout);

		GetManagerObj(m_hwnd).GetDeviceContext().CIASetVertexBuffers(0, 1, g_SimeVertexBuffer.GetVertexBufferAddress(), &stride, &offset);
		GetManagerObj(m_hwnd).GetDeviceContext().CIASetIndexBuffer(g_SimeIndexBuffer.GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		GetManagerObj(m_hwnd).GetDeviceContext().CIASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//
		// Render the cube
		//
		// Set the input layout

		GetManagerObj(m_hwnd).GetDeviceContext().CRSSetState(g_Rasterizer);
		GetManagerObj(m_hwnd).GetDeviceContext().CVSSetShader(g_pVertexShader, NULL, 0);
		GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(0, 1, g_SimeCBNeverChanges.GetCBNeverChangesAddress());
		GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(1, 1, g_SimeCBChangeOnResize.GetCBChangeOnResizeAddress());
		GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(2, 1, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
		GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(3, 1, g_DirLightBuffer.BGetBufferAddress());
		GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShader(g_pPixelShader, NULL, 0);
		GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(2, 1, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
		GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pTextureRV);
		GetManagerObj(m_hwnd).GetDeviceContext().CPSSetSamplers(0, 1, &g_pSamplerLinear);
		GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());



		GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);

		/*ONE CUBE IN THE SECOND CUBE*/
		//GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SimeRenderTargetView.GetRTV(), g_ClearColor);
		//GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(g_SimeDepthStencilView.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pViewRT2);
		//
		//cb.mWorld = XMMatrixTranspose(SecondCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pTextureRVViejoSabroso);
		//
		///* FIRST NORMAL CUBE*/
		//cb.mWorld = XMMatrixTranspose(FirstCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		///*TWO CUBES IN THE THIRD CUBE*/
		////GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SecondSimeRenderTargetView.GetRTV(), g_ClearColor);
		//GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(g_SimeDepthStencilView.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SecondSimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pTextureRVViejoSabroso);
		//
		//cb.mWorld = XMMatrixTranspose(FirstCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pViewRT2);
		//
		//
		//cb.mWorld = XMMatrixTranspose(SecondCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pViewRT3);
		//
		//cb.mWorld = XMMatrixTranspose(ThirdCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		///*THREE CUBES IN THE FOURTH CUBE*/
		////GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_ThirdSimeRenderTargetView.GetRTV(), g_ClearColor);
		//GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(g_SimeDepthStencilView.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_ThirdSimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pTextureRVViejoSabroso);
		//
		//cb.mWorld = XMMatrixTranspose(FirstCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pViewRT2);
		//
		//cb.mWorld = XMMatrixTranspose(SecondCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pViewRT3);
		//
		//cb.mWorld = XMMatrixTranspose(ThirdCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
		//
		//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
		//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, &g_pViewRT4);
		//
		//cb.mWorld = XMMatrixTranspose(FourthCube.GetWorldMatrix());
		//cb.vMeshColor = g_vMeshColor;
		//GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);
		//GetManagerObj(m_hwnd).GetDeviceContext().CDrawIndexed(36, 0, 0);
#endif
}

	void test::CleanupDevice()
	{
	#if defined(DX11)
		if (GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()) GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()->ClearState();

		if (g_pSamplerLinear) g_pSamplerLinear->Release();
		if (g_pTextureRV) g_pTextureRV->Release();
		if (g_pTextureRVViejoSabroso) g_pTextureRVViejoSabroso->Release();
		if (g_SimeCBNeverChanges.GetCBNeverChanges()) g_SimeCBNeverChanges.GetCBNeverChanges()->Release();
		if (g_SimeCBChangeOnResize.GetCBChangesOnResize()) g_SimeCBChangeOnResize.GetCBChangesOnResize()->Release();
		if (g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()) g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()->Release();
		if (g_DirLightBuffer.BGetBuffer()) g_DirLightBuffer.BGetBuffer()->Release();
		if (g_SimeVertexBuffer.GetVertexBuffer()) g_SimeVertexBuffer.GetVertexBuffer()->Release();
		if (g_SimeIndexBuffer.GetIndexBuffer()) g_SimeIndexBuffer.GetIndexBuffer()->Release();
		if (g_pVertexLayout) g_pVertexLayout->Release();
		if (g_pVertexShader) g_pVertexShader->Release();
		if (g_pPixelShader) g_pPixelShader->Release();
		if (g_SimeDepthStencil.GetTexture()) g_SimeDepthStencil.GetTexture()->Release();
		if (g_SimeDepthStencilView.GetDSV()) g_SimeDepthStencilView.GetDSV()->Release();
		if (g_SimeRenderTargetView.GetRTV()) g_SimeRenderTargetView.GetRTV()->Release();
		if (GetManagerObj(m_hwnd).GetSwapChain().GetDXSC()) GetManagerObj(m_hwnd).GetSwapChain().GetDXSC()->Release();
		if (GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()) GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()->Release();
		if (GetManagerObj(m_hwnd).GetDevice().GetDXDevice()) GetManagerObj(m_hwnd).GetDevice().GetDXDevice()->Release();
	#endif
	}

	extern Manager& GetManagerObj(HWND hwnd)
	{
		static Manager* pManager = nullptr;
		if (pManager == nullptr)
		{
			pManager = new Manager();
		}
		return *pManager;
	}
}