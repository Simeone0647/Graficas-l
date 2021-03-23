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

		SIME_DRIVER_TYPE driverTypes[] =
		{
			SIME_DRIVER_TYPE_HARDWARE,
			SIME_DRIVER_TYPE_WARP,
			SIME_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		g_DeviceAndSwapChainDesc.FeatureLevels[0] = SIME_FEATURE_LEVEL_11_0;
		g_DeviceAndSwapChainDesc.FeatureLevels[1] = SIME_FEATURE_LEVEL_10_1;
		g_DeviceAndSwapChainDesc.FeatureLevels[2] = SIME_FEATURE_LEVEL_10_0;

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

		g_SimeDepthStencilSRV.SetDesc(DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2D, 1);
		hr = GetManagerObj(hwnd).GetDevice().CCreateShaderResourceView(g_SimeDepthStencil.GetTexture(), g_SimeDepthStencilSRV.GetDXSRVDescAddress(),
																	   g_SimeDepthStencilSRV.GetDXSRVAddress());
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
		hr = GetManagerObj(hwnd).GetDevice().CCreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, g_SimeVertexShader.GetDXVertexShaderAddress());
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}

		hr = g_SimeVertexShaderReflection.DoReflect(pVSBlob);
		if (FAILED(hr))
		{
			pVSBlob->Release();
			return hr;
		}

		//CREATE INPUT LAYOUT WITH REFLECTION
		g_SimeVertexShaderReflection.GetDesc();
		g_SimeInputLayout.DefineInputLayout(g_SimeVertexShaderReflection.GetDXShaderReflectionDesc(), g_SimeVertexShaderReflection.GetDXShaderReflection());
		hr = GetManagerObj(hwnd).GetDevice().CCreateInputLayout(g_SimeInputLayout.GetDXInputLayoutDescAddress() , g_SimeInputLayout.GetDXInputLayoutDescSize(), pVSBlob->GetBufferPointer(),
																 pVSBlob->GetBufferSize(), g_SimeInputLayout.GetDXInputLayoutAddress());
		pVSBlob->Release();
		if (FAILED(hr))
		{
			return hr;
		}

		//Free allocation shader reflection memory
		g_SimeVertexShaderReflection.GetDXShaderReflection()->Release();


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
		hr = GetManagerObj(hwnd).GetDevice().CCreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, g_SimePixelShader.GetDXPixelShaderAddress());
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;

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
		hr = D3DX11CreateShaderResourceViewFromFile(GetManagerObj(hwnd).GetDevice().GetDXDevice(), "base_albedo.dds", NULL, NULL, g_SimeTextureRV.GetDXSRVAddress(), NULL);
		if (FAILED(hr))
			return hr;

		g_SimeSamplerState.SetDesc();

		hr = GetManagerObj(hwnd).GetDevice().CCreateSamplerState(g_SimeSamplerState.GetDXSamplerDescAddress(), g_SimeSamplerState.GetDXSamplerStateAddress());
		if (FAILED(hr))
			return hr;


		m_PerspectiveCamera.SetEye(0.0f, 3.0f, -6.0f);
		m_PerspectiveCamera.SetAt(0.0f, 0.0f, 0.0f);
		m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
		m_PerspectiveCamera.UpdateViewMatrix();
		m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(XM_PIDIV4, width / (FLOAT)height, 0.01f, 1000.0f);

		m_OrtographicCamera.SetEye(0.0f, 3.0f, -6.0f);
		m_OrtographicCamera.SetAt(0.0f, 0.0f, 0.0f);
		m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
		m_OrtographicCamera.UpdateViewMatrix();
		m_OrtographicCamera.UpdateOrtographicProjectionMatrix(width, height, 0.01f, 1000.0f);

		m_Camera = &m_PerspectiveCamera;

		/* FIRST RENDER TARGET TEXTURE
			CREATE TEXTURE*/
		g_TextureRenderTarget;
		g_TextureRenderTarget.SetDescRT(width, height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(g_TextureRenderTarget.GetDescDepthAddress(), NULL, g_TextureRenderTarget.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		//g_TextureRenderTarget2;
		//g_TextureRenderTarget2.SetDescRT(width, height, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, 0, 0);
		//hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(g_TextureRenderTarget2.GetDescDepthAddress(), NULL, g_TextureRenderTarget2.GetTextureAddress());
		//if (FAILED(hr))
		//	return hr;
		//
		//D3D11_SHADER_RESOURCE_VIEW_DESC descViewRT2;
		//ZeroMemory(&descViewRT2, sizeof(descViewRT2));
		//descViewRT2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//descViewRT2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//descViewRT2.Texture2D.MostDetailedMip = 0;
		//descViewRT2.Texture2D.MipLevels = 1;
		//hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(g_TextureRenderTarget2.GetTexture(), &descViewRT2, &g_pViewRT3);
		//if (FAILED(hr))
		//	return hr;
		//
		//hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(g_TextureRenderTarget2.GetTexture(), NULL, g_SecondSimeRenderTargetView.GetRTVAdress());
		//if (FAILED(hr))
		//	return hr;
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

	if (g_driverType == SIME_DRIVER_TYPE_REFERENCE)
	{
		t += 3.141592654f * 0.0125f;
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
	//g_Translation.m[0][3] = 0;
	//g_Translation.m[1][3] = 0;
	//g_Translation.m[2][3] = 0;
	//
	////FIRST CUBE
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		FirstCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	//
	//g_World = g_Translation;
	//g_World = XMMatrixRotationY(t);
	//g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);
	//
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		FirstCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	
	////SECOND CUBE
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		SecondCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	//
	//g_World = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	//g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	//g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);
	//
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		SecondCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	//
	////THIRD CUBE
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		ThirdCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	//
	//g_World = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	//g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	//g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);
	//
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		ThirdCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	//
	////FOURTH CUBE
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		FourthCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;
	//
	//g_World = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	//g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	//g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);
	//
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		FourthCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
	//		counter++;
	//	}
	//}
	//counter = 0;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SimeRenderTargetView.GetRTV(), ClearColor);

	GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(g_SimeDepthStencilView.GetDSV(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(m_Camera->GetViewMatrix());
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBNeverChanges.GetCBNeverChanges(), 0, NULL, &cbNeverChanges, 0, 0);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(g_SimeCBChangeOnResize.GetCBChangesOnResize(), 0, NULL, &cbChangesOnResize, 0, 0);

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

	GetManagerObj(m_hwnd).GetDeviceContext().CIASetVertexBuffers(0, 1, g_SimeVertexBuffer.GetVertexBufferAddress(), &stride, &offset);
	GetManagerObj(m_hwnd).GetDeviceContext().CIASetIndexBuffer(g_SimeIndexBuffer.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	GetManagerObj(m_hwnd).GetDeviceContext().CIASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
	GetManagerObj(m_hwnd).GetDeviceContext().CRSSetViewports(1, g_SimeViewport.GetViewportAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CIASetInputLayout(g_SimeInputLayout.GetDXInputLayout());

	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetShader(g_SimeVertexShader.GetDXVertexShader(), NULL, 0);
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(0, 1, g_SimeCBNeverChanges.GetCBNeverChangesAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(1, 1, g_SimeCBChangeOnResize.GetCBChangeOnResizeAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShader(g_SimePixelShader.GetDXPixelShader(), NULL, 0);

#endif
}

	void test::CleanupDevice()
	{
	#if defined(DX11)
		if (GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()) GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()->ClearState();

		if (g_SimeSamplerState.GetDXSamplerState()) g_SimeSamplerState.GetDXSamplerState()->Release();
		if (g_SimeTextureRV.GetDXSRV()) g_SimeTextureRV.GetDXSRV()->Release();
		if (g_SimeCBNeverChanges.GetCBNeverChanges()) g_SimeCBNeverChanges.GetCBNeverChanges()->Release();
		if (g_SimeCBChangeOnResize.GetCBChangesOnResize()) g_SimeCBChangeOnResize.GetCBChangesOnResize()->Release();
		if (g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()) g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()->Release();
		if (g_DirLightBuffer.BGetBuffer()) g_DirLightBuffer.BGetBuffer()->Release();
		if (g_SimeVertexBuffer.GetVertexBuffer()) g_SimeVertexBuffer.GetVertexBuffer()->Release();
		if (g_SimeIndexBuffer.GetIndexBuffer()) g_SimeIndexBuffer.GetIndexBuffer()->Release();
		if (g_SimeInputLayout.GetDXInputLayout()) g_SimeInputLayout.GetDXInputLayout()->Release();
		if (g_SimeVertexShader.GetDXVertexShader()) g_SimeVertexShader.GetDXVertexShader()->Release();
		if (g_SimePixelShader.GetDXPixelShader()) g_SimePixelShader.GetDXPixelShader()->Release();
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