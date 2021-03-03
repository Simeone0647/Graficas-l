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
		//ID3D11Texture2D* pBackBuffer = NULL;
		Texture2D BackBuffer;
		hr = GetManagerObj(hwnd).GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)BackBuffer.GetTextureAdress());
		if (FAILED(hr))
			return hr;

		hr = GetManagerObj(hwnd).GetDevice()->CreateRenderTargetView(BackBuffer.GetTexture(), NULL, g_SimeRenderTargetView.GetRTVAdress());
		BackBuffer.GetTexture()->Release();
		if (FAILED(hr))
			return hr;

		g_SimeDepthStencil.SetDescDepth(width, height, 1, 1, DXGI_FORMAT_R32_TYPELESS, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice()->CreateTexture2D(g_SimeDepthStencil.GetDescDepthAdress(), NULL, g_SimeDepthStencil.GetTextureAdress());
		if (FAILED(hr))
			return hr;

		g_SimeDepthStencilView.SetDescDSV(DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2D, 0);
		hr = GetManagerObj(hwnd).GetDevice()->CreateDepthStencilView(g_SimeDepthStencil.GetTexture(), g_SimeDepthStencilView.GetDescDSVAdress(), g_SimeDepthStencilView.GetDSVAdress());
		if (FAILED(hr))
			return hr;

		// and the resource view for the shader
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1; // same as orig texture
		hr = GetManagerObj(hwnd).GetDevice()->CreateShaderResourceView(g_SimeDepthStencil.GetTexture(), &srvDesc, &g_pDepthStencilSRV);
		if (FAILED(hr))
			return hr;

		GetManagerObj(hwnd).GetDeviceContext()->OMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());

		// Setup the viewport
		g_SimeViewport.InitViewport((FLOAT)width, (FLOAT)height, 0.0f, 1.0f, 0.0f, 0.0f);
		GetManagerObj(hwnd).GetDeviceContext()->RSSetViewports(1, g_SimeViewport.GetViewportAdress());

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
		hr = GetManagerObj(hwnd).GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
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
		};
		UINT numElements = ARRAYSIZE(layout);

		// Create the input layout
		hr = GetManagerObj(hwnd).GetDevice()->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
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
		hr = GetManagerObj(hwnd).GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;

		// Create vertex buffer
		SimpleVertex vertices[] =
		{
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		};

		g_SimeVertexBuffer.UpdateBd(D3D11_USAGE_DEFAULT, sizeof(SimpleVertex) * 24, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0);

		g_SimeVertexBuffer.UpdateInitData(vertices);

		hr = GetManagerObj(hwnd).GetDevice()->CreateBuffer(g_SimeVertexBuffer.GetBdAdress(), g_SimeVertexBuffer.GetInitDataAdress(), g_SimeVertexBuffer.GetVertexBufferAdress());
		if (FAILED(hr))
			return hr;

		// Create index buffer
		WORD indices[] =
		{
			3,1,0,
			2,1,3,

			6,4,5,
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
		};

		g_SimeIndexBuffer.UpdateBd(D3D11_USAGE_DEFAULT, sizeof(WORD) * 36, D3D11_BIND_INDEX_BUFFER, 0, 0, 0);
		g_SimeIndexBuffer.UpdateInitData(indices);
		hr = GetManagerObj(hwnd).GetDevice()->CreateBuffer(g_SimeIndexBuffer.GetBdAdress(), g_SimeIndexBuffer.GetInitDataAdress(), g_SimeIndexBuffer.GetIndexBufferAdress());
		if (FAILED(hr))
			return hr;

		// Set primitive topology
		GetManagerObj(hwnd).GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		g_SimeCBNeverChanges.UpdateBd(D3D11_USAGE_DEFAULT, sizeof(CBNeverChanges), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice()->CreateBuffer(g_SimeCBNeverChanges.GetBdAdress(), NULL, g_SimeCBNeverChanges.GetCBNeverChangesAdress());
		if (FAILED(hr))
			return hr;

		g_SimeCBChangeOnResize.UpdateBd(D3D11_USAGE_DEFAULT, sizeof(CBChangeOnResize), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice()->CreateBuffer(g_SimeCBChangeOnResize.GetBdAdress(), NULL, g_SimeCBChangeOnResize.GetCBChangeOnResizeAdress());
		if (FAILED(hr))
			return hr;

		g_SimeCBChangesEveryFrame.UpdateBd(D3D11_USAGE_DEFAULT, sizeof(CBChangesEveryFrame), D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0);
		hr = GetManagerObj(hwnd).GetDevice()->CreateBuffer(g_SimeCBChangesEveryFrame.GetBdAdress(), NULL, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAdress());
		if (FAILED(hr))
			return hr;

		
		// Load the Texture
		hr = D3DX11CreateShaderResourceViewFromFile(GetManagerObj(hwnd).GetDevice(), "seafloor.dds", NULL, NULL, &g_pTextureRV, NULL);
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
		hr = GetManagerObj(hwnd).GetDevice()->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
		if (FAILED(hr))
			return hr;

		// Initialize the world matrices
		g_World = XMMatrixIdentity();

		// Initialize the view matrix
		XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
		XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		g_View = XMMatrixLookAtLH(Eye, At, Up);

		CBNeverChanges cbNeverChanges;
		cbNeverChanges.mView = XMMatrixTranspose(g_View);
		GetManagerObj(hwnd).GetDeviceContext()->UpdateSubresource(g_SimeCBNeverChanges.GetCBNeverChanges(), 0, NULL, &cbNeverChanges, 0, 0);

		// Initialize the projection matrix
		g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(g_Projection);
		GetManagerObj(hwnd).GetDeviceContext()->UpdateSubresource(g_SimeCBChangeOnResize.GetCBChangesOnResize(), 0, NULL, &cbChangesOnResize, 0, 0);


		// create rasterizer state
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		hr = GetManagerObj(hwnd).GetDevice()->CreateRasterizerState(&desc, &g_Rasterizer);
		if (FAILED(hr))
			return hr;

#endif
		return S_OK;
}

void test::Render()
{
#if defined(DX11)
		// Update our time
		static float t = 0.0f;
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
		g_World = XMMatrixRotationY(t);

		// Modify the color
		g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
		g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
		g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

		//
		// Clear the back buffer
		//
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
		GetManagerObj(m_hwnd).GetDeviceContext()->ClearRenderTargetView(g_SimeRenderTargetView.GetRTV(), ClearColor);

		//
		// Clear the depth buffer to 1.0 (max depth)
		//
		GetManagerObj(m_hwnd).GetDeviceContext()->ClearDepthStencilView(g_SimeDepthStencilView.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		//
		// Update variables that change once per frame
		//
		CBChangesEveryFrame cb;
		cb.mWorld = XMMatrixTranspose(g_World);
		cb.vMeshColor = g_vMeshColor;
		GetManagerObj(m_hwnd).GetDeviceContext()->UpdateSubresource(g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame(), 0, NULL, &cb, 0, 0);


		UINT stride = sizeof(SimpleVertex);
		UINT offset = 0;

		//
		// Render the cube
		//
		// Set the input layout
		GetManagerObj(m_hwnd).GetDeviceContext()->IASetInputLayout(g_pVertexLayout);
		GetManagerObj(m_hwnd).GetDeviceContext()->RSSetState(g_Rasterizer);
		GetManagerObj(m_hwnd).GetDeviceContext()->IASetVertexBuffers(0, 1, g_SimeVertexBuffer.GetVertexBufferAdress(), &stride, &offset);
		GetManagerObj(m_hwnd).GetDeviceContext()->IASetIndexBuffer(g_SimeIndexBuffer.GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		GetManagerObj(m_hwnd).GetDeviceContext()->VSSetShader(g_pVertexShader, NULL, 0);
		GetManagerObj(m_hwnd).GetDeviceContext()->VSSetConstantBuffers(0, 1, g_SimeCBNeverChanges.GetCBNeverChangesAdress());
		GetManagerObj(m_hwnd).GetDeviceContext()->VSSetConstantBuffers(1, 1, g_SimeCBChangeOnResize.GetCBChangeOnResizeAdress());
		GetManagerObj(m_hwnd).GetDeviceContext()->VSSetConstantBuffers(2, 1, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAdress());
		GetManagerObj(m_hwnd).GetDeviceContext()->PSSetShader(g_pPixelShader, NULL, 0);
		GetManagerObj(m_hwnd).GetDeviceContext()->PSSetConstantBuffers(2, 1, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAdress());
		GetManagerObj(m_hwnd).GetDeviceContext()->PSSetShaderResources(0, 1, &g_pTextureRV);
		GetManagerObj(m_hwnd).GetDeviceContext()->PSSetSamplers(0, 1, &g_pSamplerLinear);
		GetManagerObj(m_hwnd).GetDeviceContext()->DrawIndexed(36, 0, 0);

#endif
}

	void test::CleanupDevice()
	{
	#if defined(DX11)
		if (GetManagerObj(m_hwnd).GetDeviceContext()) GetManagerObj(m_hwnd).GetDeviceContext()->ClearState();

		if (g_pSamplerLinear) g_pSamplerLinear->Release();
		if (g_pTextureRV) g_pTextureRV->Release();
		if (g_SimeCBNeverChanges.GetCBNeverChanges()) g_SimeCBNeverChanges.GetCBNeverChanges()->Release();
		if (g_SimeCBChangeOnResize.GetCBChangesOnResize()) g_SimeCBChangeOnResize.GetCBChangesOnResize()->Release();
		if (g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()) g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()->Release();
		if (g_SimeVertexBuffer.GetVertexBuffer()) g_SimeVertexBuffer.GetVertexBuffer()->Release();
		if (g_SimeIndexBuffer.GetIndexBuffer()) g_SimeIndexBuffer.GetIndexBuffer()->Release();
		if (g_pVertexLayout) g_pVertexLayout->Release();
		if (g_pVertexShader) g_pVertexShader->Release();
		if (g_pPixelShader) g_pPixelShader->Release();
		if (g_SimeDepthStencil.GetTexture()) g_SimeDepthStencil.GetTexture()->Release();
		if (g_SimeDepthStencilView.GetDSV()) g_SimeDepthStencilView.GetDSV()->Release();
		if (g_SimeRenderTargetView.GetRTV()) g_SimeRenderTargetView.GetRTV()->Release();
		if (GetManagerObj(m_hwnd).GetSwapChain()) GetManagerObj(m_hwnd).GetSwapChain()->Release();
		if (GetManagerObj(m_hwnd).GetDeviceContext()) GetManagerObj(m_hwnd).GetDeviceContext()->Release();
		if (GetManagerObj(m_hwnd).GetDevice()) GetManagerObj(m_hwnd).GetDevice()->Release();
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