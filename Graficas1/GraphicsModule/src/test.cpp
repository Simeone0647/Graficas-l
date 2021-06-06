#include "test.h"
#include "RenderManager.h"

namespace GraphicsModule
{
#if defined(DX11)
	HRESULT test::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = SIMECOMPILE_ENABLE_STRICTNESS;

		#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= SIMECOMPILE_DEBUG;
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
#if defined(OGL)
	GLuint test::LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
	{
		// Crear los shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Leer el Vertex Shader desde archivo
		std::string VertexShaderCode;
		std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
		if (VertexShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << VertexShaderStream.rdbuf();
			VertexShaderCode = sstr.str();
			VertexShaderStream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
			getchar();
			return 0;
		}

		// Leer el Fragment Shader desde archivo
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
		if (FragmentShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << FragmentShaderStream.rdbuf();
			FragmentShaderCode = sstr.str();
			FragmentShaderStream.close();
		}

		GLint Result = GL_FALSE;
		GLint InfoLogLength;


		// Compilar Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path);
		char const* VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Revisar Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			printf("%s\n", &VertexShaderErrorMessage[0]);
		}

		// Compilar Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path);
		char const* FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Revisar Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			printf("%s\n", &FragmentShaderErrorMessage[0]);
		}

		// Vincular el programa por medio del ID
		printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Revisar el programa
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}


		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}
#endif

HRESULT test::InitDevice(HWND hwnd)
{
#if defined(OGL)

	programID = LoadShaders("SimpleVertexShader.txt", "SimpleFragmentShader.txt");

	glUseProgram(programID);

	glEnable(GL_DEPTH_TEST);

	UpdateProjectionMatrixStruct PMStruct;
	PMStruct.AngleY = SIME_PIDIV4;
	PMStruct.Ratio = 1024 / (FLOAT)768;
	PMStruct.NearPlane = 0.01f;
	PMStruct.FarPlane = 1000.0f;
	PMStruct.Width = 1024;
	PMStruct.Height = 768;

	m_PerspectiveCamera.SetEye(0.0f, 3.0f, -6.0f);
	m_PerspectiveCamera.SetAt(0.0f, 0.0f, 0.0f);
	m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
	m_PerspectiveCamera.UpdateViewMatrix();
	m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(PMStruct);

	m_OrtographicCamera.SetEye(0.0f, 2.0f, 0.0f);
	m_OrtographicCamera.SetAt(0.0f, 0.0f, -4.0f);
	m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
	m_OrtographicCamera.UpdateViewMatrix();
	m_OrtographicCamera.UpdateOrtographicProjectionMatrix(PMStruct);

	m_Camera = &m_PerspectiveCamera;

#endif

#if defined(DX11)
		m_hwnd = hwnd;
		HRESULT hr = S_OK;

		RECT rc;
		GetClientRect(m_hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;
		g_DeviceAndSwapChainDesc.CreateDeviceFlags = 0;
#ifdef _DEBUG
		g_DeviceAndSwapChainDesc.CreateDeviceFlags |= SIME_CREATE_DEVICE_DEBUG;
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
		g_DeviceAndSwapChainDesc.sd.BufferUsage = SIME_USAGE_RENDER_TARGET_OUTPUT;
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
			
		// Create a render target view El que se manda al backbuffer.
		Texture2D BackBuffer;
		hr = GetManagerObj(hwnd).GetSwapChain().CGetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)BackBuffer.GetTextureAddress());
		if (FAILED(hr))
			return hr;
		
		RM::GetRenderManager().BackBufferRTV.AddRTV();
		//g_SimeRenderTargetView.AddRTV();

		hr = GetManagerObj(hwnd).GetDevice().CCreateRenderTargetView(BackBuffer.GetTexture(), NULL, RM::GetRenderManager().BackBufferRTV.GetLastElementAddress());
		BackBuffer.GetTexture()->Release();
		if (FAILED(hr))
			return hr;

		g_DepthDesc.Width = width;
		g_DepthDesc.Height = height;
		g_DepthDesc.Mips = 1;
		g_DepthDesc.ArraySize = 1;
		g_DepthDesc.Format = SIME_FORMAT_R32_TYPELESS;
		g_DepthDesc.Count = 1;
		g_DepthDesc.Quality = 0;
		g_DepthDesc.BindFlags = SIME_BIND_DEPTH_STENCIL | SIME_BIND_SHADER_RESOURCE;
		g_DepthDesc.Usage = SIME_USAGE_DEFAULT;
		g_DepthDesc.CPUAccessFlags = 0;
		g_DepthDesc.MiscFlags = 0;

		RM::GetRenderManager().DepthStencil.SetDescDepth(g_DepthDesc);
		hr = GetManagerObj(hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().DepthStencil.GetDescDepthAddress(), NULL, RM::GetRenderManager().DepthStencil.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		RM::GetRenderManager().DSView.SetDescDSV(SIME_FORMAT_D32_FLOAT, SIME_DSV_DIMENSION_TEXTURE2D, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateDepthStencilView(RM::GetRenderManager().DepthStencil.GetTexture(), RM::GetRenderManager().DSView.GetDescDSVAddress(), RM::GetRenderManager().DSView.GetDSVAddress());
		if (FAILED(hr))
			return hr;

		RM::GetRenderManager().DepthStencilSRV.SetDesc(SIME_FORMAT_R32_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);
		hr = GetManagerObj(hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().DepthStencil.GetTexture(), RM::GetRenderManager().DepthStencilSRV.GetDXSRVDescAddress(),
																	   RM::GetRenderManager().DepthStencilSRV.GetDXSRVAddress());
		if (FAILED(hr))
			return hr;

		// Setup the viewport
		InitViewportStruct InitVP;
		InitVP.Width = (FLOAT)width;
		InitVP.Height = (FLOAT)height;
		InitVP.MinDepth = 0.0f;
		InitVP.MaxDepth = 1.0f;
		InitVP.TopLeftX = 0.0f;
		InitVP.TopLeftY = 0.0f;

		g_SimeViewport.InitViewport(InitVP);

		UpdateBDStruct BDStruct;
		BDStruct.Usage = SIME_USAGE_DEFAULT;
		BDStruct.ByteWidth = sizeof(CBNeverChanges);
		BDStruct.BindFlags = SIME_BIND_CONSTANT_BUFFER;
		BDStruct.CPUAccessFlags = 0;
		BDStruct.StructureBytestride = 0;
		BDStruct.MiscFlags = 0;

		g_SimeCBNeverChanges.UpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBNeverChanges.GetBDAddress(), NULL, g_SimeCBNeverChanges.GetCBNeverChangesAddress());
		if (FAILED(hr))
			return hr;
		
		BDStruct.ByteWidth = sizeof(CBChangeOnResize);
		g_SimeCBChangeOnResize.UpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBChangeOnResize.GetBDAddress(), NULL, g_SimeCBChangeOnResize.GetCBChangeOnResizeAddress());
		if (FAILED(hr))
			return hr;

		//BDStruct.ByteWidth = sizeof(CBChangesEveryFrame);
		//g_SimeCBChangesEveryFrame.UpdateBD(BDStruct);
		//hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBChangesEveryFrame.GetBDAddress(), NULL, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
		//if (FAILED(hr))
		//	return hr;

		BDStruct.ByteWidth = sizeof(DirLight);
		g_DirLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_DirLightBuffer.BGetBDAddress(), NULL, g_DirLightBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(PointLight);
		g_PointLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_PointLightBuffer.BGetBDAddress(), NULL, g_PointLightBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(SpotLight);
		g_SpotLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SpotLightBuffer.BGetBDAddress(), NULL, g_SpotLightBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(Diffuse);
		g_DiffuseBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_DiffuseBuffer.BGetBDAddress(), NULL, g_DiffuseBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(Ambient);
		g_AmbientBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_AmbientBuffer.BGetBDAddress(), NULL, g_AmbientBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(Specular);
		g_SpecularBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SpecularBuffer.BGetBDAddress(), NULL, g_SpecularBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(Vector3);
		g_CameraFrontBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_CameraFrontBuffer.BGetBDAddress(), NULL, g_CameraFrontBuffer.BGetBufferAddress());
		// Load the Texture
		//hr = D3DX11CreateShaderResourceViewFromFile(GetManagerObj(hwnd).GetDevice().GetDXDevice(), "base_albedo.dds", NULL, NULL, g_SimeTextureRV.GetDXSRVAddress(), NULL);
		if (FAILED(hr))
			std::cout << "Fallo spotlight";

		UpdateProjectionMatrixStruct PMStruct;
		PMStruct.AngleY = SIME_PIDIV4;
		PMStruct.Ratio = width / (FLOAT)height;
		PMStruct.NearPlane = 0.01f;
		PMStruct.FarPlane = 1000.0f;
		PMStruct.Width = width;
		PMStruct.Height = height;

		m_PerspectiveCamera.SetEye(0.0f, 3.0f, -6.0f);
		m_PerspectiveCamera.SetAt(0.0f, 0.0f, 0.0f);
		m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
		m_PerspectiveCamera.UpdateViewMatrix();
		m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(PMStruct);

		m_OrtographicCamera.SetEye(0.0f, 3.0f, -6.0f);
		m_OrtographicCamera.SetAt(0.0f, 0.0f, 0.0f);
		m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
		m_OrtographicCamera.UpdateViewMatrix();
		m_OrtographicCamera.UpdateOrtographicProjectionMatrix(PMStruct);

		m_Camera = &m_PerspectiveCamera;

		/* FIRST RENDER TARGET TEXTURE
			CREATE TEXTURE*/
		SetRTDescStruct RTDescStruct;
		RTDescStruct.Width = width;
		RTDescStruct.Height = height;
		RTDescStruct.MipLevels = 1;
		RTDescStruct.Arraysize = 1;
		RTDescStruct.Format = SIME_FORMAT_R16G16B16A16_FLOAT;
		RTDescStruct.Count = 1;
		RTDescStruct.Quality = 0;
		RTDescStruct.Usage = SIME_USAGE_DEFAULT;
		RTDescStruct.CPUAccessFlags = 0;
		RTDescStruct.MiscFlags = 0;
		RTDescStruct.BindFlags = SIME_BIND_SHADER_RESOURCE | SIME_BIND_RENDER_TARGET;

		for (unsigned int i = 0; i < 4; ++i)
		{
			RM::GetRenderManager().GBufferRTV.AddRTV();
			//g_GBufferRTV.AddRTV();

			RM::GetRenderManager().GBufferTextures[i].SetDescRT(RTDescStruct);
			//g_GBufferTextures[i].SetDescRT(RTDescStruct);
			hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().GBufferTextures[i].GetDescDepthAddress(), NULL,
							   RM::GetRenderManager().GBufferTextures[i].GetTextureAddress());
			if (FAILED(hr))
			{
				cout << "Error tekstur 2d" << endl;
			}

			RM::GetRenderManager().GBufferSRV[i].SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);

			hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().GBufferTextures[i].GetTexture(), RM::GetRenderManager().GBufferSRV[i].GetDXSRVDescAddress(),
																			 RM::GetRenderManager().GBufferSRV[i].GetDXSRVAddress());
			if (FAILED(hr))
			{
				cout << "Error cheiderresoursviu" << endl;
			}

			hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().GBufferTextures[i].GetTexture(), NULL,
																		   RM::GetRenderManager().GBufferRTV.GetLastElementAddress());
		}

		SetRasterizerStruct RasterStruct;
		RasterStruct.Fill = SIME11_FILL_SOLID;
		RasterStruct.Cull = SIME11_CULL_BACK;
		RasterStruct.FrontCCW = false;

		RM::GetRenderManager().GBufferRasterState.SetRasterizerDesc(RasterStruct);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRasterizerState(RM::GetRenderManager().GBufferRasterState.GetRSDescAddress(),
																	  RM::GetRenderManager().GBufferRasterState.GetRSAddress());
		if (FAILED(hr))
		{
			cout << "Fallo en crear el Rasterizer" << endl;
		}

		SetRasterizerStruct RasterStructGBufferLight;
		RasterStructGBufferLight.Fill = SIME11_FILL_SOLID;
		RasterStructGBufferLight.Cull = SIME11_CULL_NONE;
		RasterStructGBufferLight.FrontCCW = false;

		RM::GetRenderManager().GBufferLightRasterState.SetRasterizerDesc(RasterStructGBufferLight);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRasterizerState(RM::GetRenderManager().GBufferLightRasterState.GetRSDescAddress(),
			RM::GetRenderManager().GBufferLightRasterState.GetRSAddress());
		if (FAILED(hr))
		{
			cout << "Fallo en crear el RasterizerLight" << endl;
		}
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

	//if (m_IsFirstFrame)
	//{
	//	if (!m_IsFirstPosStored)
	//	{
	//		GetCursorPos(MouseInitPos);
	//		m_IsFirstPosStored = true;
	//	}
	//	else
	//	{
	//		GetCursorPos(MouseFinalPos);
	//		LPPOINT Direction = new POINT;
	//		Direction->x = MouseFinalPos->x - MouseInitPos->x;
	//		Direction->y = MouseFinalPos->y - MouseInitPos->y;
	//		Vector3 RotateVector;
	//		RotateVector.SetValues(Direction->x, Direction->y, 0.0f);
	//		m_Camera->RotateCamera(RotateVector);
	//		m_Camera->UpdateViewMatrix();
	//		m_IsFirstPosStored = false;
	//		if (Direction != nullptr)
	//		{
	//			delete Direction;
	//		}
	//	}
	//}

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

	//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(RM::GetRenderManager().GBufferRTV.GetRTVNum(),
	//															 RM::GetRenderManager().GBufferRTV.GetRTVAdress(), RM::GetRenderManager().DSView.GetDSV());
	//
	// Clear the back buffer
	//
	//float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	//float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // red, green, blue, alpha
	//
	//for (unsigned int i = 0; i < RM::GetRenderManager().GBufferRTV.GetRTVNum(); ++i)
	//{
	//	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(RM::GetRenderManager().GBufferRTV.GetRTV(i), ClearColor);
	//}
	//GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SimeRenderTargetView.GetRTV(), ClearColor);

	ClearDepthStencilViewStruct ClearDSVStruct;
	ClearDSVStruct.pDepthStencilView = RM::GetRenderManager().DSView.GetDSV();
	ClearDSVStruct.ClearFlags = SIME_CLEAR_DEPTH;
	ClearDSVStruct.Depth = 1.0f;
	ClearDSVStruct.Stencil = 0;

	GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(m_Camera->m_ViewMatrix);

	UpdateSubResourceStruct UpdateSBStruct;
	UpdateSBStruct.pDstResource = g_SimeCBNeverChanges.GetCBNeverChanges();
	UpdateSBStruct.DstSubresource = 0;
	UpdateSBStruct.pDstBox = NULL;
	UpdateSBStruct.pSrcData = &cbNeverChanges;
	UpdateSBStruct.SrcRowPitch = 0;
	UpdateSBStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->m_ProjectionMatrix);
	UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
	UpdateSBStruct.pSrcData = &cbChangesOnResize;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);

	if (m_IsPerspectiveActive)
	{
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->m_ProjectionMatrix);
		UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
		UpdateSBStruct.pSrcData = &cbChangesOnResize;
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);
	}
	else
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->m_ProjectionMatrix);
		UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
		UpdateSBStruct.pSrcData = &cbChangesOnResize;
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);
	}

#endif
#if defined(OGL)

#endif
}

void test::Render()
{
#if defined(OGL)
	//DIRECTIONAL LIGHT
	const char* UniformNameLight;
	int UniformLight;
	UniformNameLight = "dirlight";
	UniformLight = glGetUniformLocation(programID, UniformNameLight);
	if (UniformLight == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameLight);
	}
	glUniform4fv(UniformLight, 1, glm::value_ptr(glm::make_vec4(g_DirLightBufferDesc.Dir)));

	const char* UniformNameLightColor;
	int UniformLightColor;
	UniformNameLightColor = "dirlightcolor";
	UniformLightColor = glGetUniformLocation(programID, UniformNameLightColor);
	if (UniformLightColor == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameLightColor);
	}
	glUniform4fv(UniformLightColor, 1, glm::value_ptr(glm::make_vec4(g_DirLightBufferDesc.Color)));

	//POINTLIGHT
	const char* UniformNamePointLightPos;
	int UniformPointLightPos;
	UniformNamePointLightPos = "PointLightPos";
	UniformPointLightPos = glGetUniformLocation(programID, UniformNamePointLightPos);
	if (UniformPointLightPos == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNamePointLightPos);
	}
	glUniform4fv(UniformPointLightPos, 1, glm::value_ptr(glm::make_vec4(g_PointLightBufferDesc.Position)));

	const char* UniformNamePointLightColor;
	int UniformPointLightColor;
	UniformNamePointLightColor = "PointLightColor";
	UniformPointLightColor = glGetUniformLocation(programID, UniformNamePointLightColor);
	if (UniformPointLightColor == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNamePointLightColor);
	}
	glUniform4fv(UniformPointLightColor, 1, glm::value_ptr(glm::make_vec4(g_PointLightBufferDesc.Color)));

	const char* UniformNamePointLightAttenuation;
	int UniformPointLightAttenuation;
	UniformNamePointLightAttenuation = "PointLightAttenuation";
	UniformPointLightAttenuation = glGetUniformLocation(programID, UniformNamePointLightAttenuation);
	if (UniformPointLightAttenuation == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNamePointLightAttenuation);
	}
	glUniform4fv(UniformPointLightAttenuation, 1, glm::value_ptr(glm::make_vec4(g_PointLightBufferDesc.Attenuation)));

	//SPOTLIGHT
	const char* UniformNameSpotLightDir;
	int UniformSpotLightDir;
	UniformNameSpotLightDir = "SpotLightDir";
	UniformSpotLightDir = glGetUniformLocation(programID, UniformNameSpotLightDir);
	if (UniformSpotLightDir == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightDir);
	}
	glUniform4fv(UniformSpotLightDir, 1, glm::value_ptr(glm::make_vec4(g_SpotLightBufferDesc.Dir)));

	const char* UniformNameSpotLightPos;
	int UniformSpotLightPos;
	UniformNameSpotLightPos = "SpotLightPos";
	UniformSpotLightPos = glGetUniformLocation(programID, UniformNameSpotLightPos);
	if (UniformSpotLightPos == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightPos);
	}
	glUniform4fv(UniformSpotLightPos, 1, glm::value_ptr(glm::make_vec4(g_SpotLightBufferDesc.Pos)));

	const char* UniformNameSpotLightColor;
	int UniformSpotLightColor;
	UniformNameSpotLightColor = "SpotLightColor";
	UniformSpotLightColor = glGetUniformLocation(programID, UniformNameSpotLightColor);
	if (UniformSpotLightColor == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightColor);
	}
	glUniform4fv(UniformSpotLightColor, 1, glm::value_ptr(glm::make_vec4(g_SpotLightBufferDesc.Color)));

	const char* UniformNameSpotLightAttenuation;
	int UniformSpotLightAttenuation;
	UniformNameSpotLightAttenuation = "SpotLightAttenuation";
	UniformSpotLightAttenuation = glGetUniformLocation(programID, UniformNameSpotLightAttenuation);
	if (UniformSpotLightAttenuation == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightAttenuation);
	}
	glUniform1f(UniformSpotLightAttenuation, g_SpotLightBufferDesc.Attenuation);

	const char* UniformNameSpotLightInner;
	int UniformSpotLightInner;
	UniformNameSpotLightInner = "SpotLightInner";
	UniformSpotLightInner = glGetUniformLocation(programID, UniformNameSpotLightInner);
	if (UniformSpotLightInner == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightInner);
	}
	glUniform1f(UniformSpotLightInner, g_SpotLightBufferDesc.InnerRadius);

	const char* UniformNameSpotLightOuter;
	int UniformSpotLightOuter;
	UniformNameSpotLightOuter = "SpotLightOuter";
	UniformSpotLightOuter = glGetUniformLocation(programID, UniformNameSpotLightOuter);
	if (UniformSpotLightOuter == -1)
	{
		fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightOuter);
	}
	glUniform1f(UniformSpotLightOuter, g_SpotLightBufferDesc.OuterRadius);
	
#endif
#if defined(DX11)
	GraphicsModule::UpdateSubResourceStruct UpdateSRStruct;
	UpdateSRStruct.pDstResource = g_DirLightBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &g_DirLightBufferDesc;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = g_PointLightBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &g_PointLightBufferDesc;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = g_SpotLightBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &g_SpotLightBufferDesc;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = g_AmbientBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &g_Ambient;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = g_DiffuseBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &g_Diffuse;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = g_SpecularBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &g_Specular;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = g_CameraFrontBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = m_Camera->GetEyeAddress();
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	//AQUI
	//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_PositionRTV.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
	//GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
	GetManagerObj(m_hwnd).GetDeviceContext().CRSSetViewports(1, g_SimeViewport.GetViewportAddress());
	//GetManagerObj(m_hwnd).GetDeviceContext().CIASetInputLayout(g_SimeInputLayout.GetDXInputLayout());
	//
	////LUEGO
	//GetManagerObj(m_hwnd).GetDeviceContext().CVSSetShader(g_SimeVertexShader.GetDXVertexShader(), NULL, 0);
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(0, 1, g_SimeCBNeverChanges.GetCBNeverChangesAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(1, 1, g_SimeCBChangeOnResize.GetCBChangeOnResizeAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(3, 1, g_DirLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(4, 1, g_PointLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(5, 1, g_SpotLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(6, 1, g_SpecularBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(7, 1, g_AmbientBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(8, 1, g_DiffuseBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(9, 1, g_CameraFrontBuffer.BGetBufferAddress());

	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(3, 1, g_DirLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(4, 1, g_PointLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(5, 1, g_SpotLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(6, 1, g_SpecularBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(7, 1, g_AmbientBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(8, 1, g_DiffuseBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(9, 1, g_CameraFrontBuffer.BGetBufferAddress());
	//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetShader(g_SimePixelShader.GetDXPixelShader(), NULL, 0);
	//GetManagerObj(m_hwnd).GetDeviceContext().CPSSetSamplers(0, 1, g_SimeSamplerState.GetDXSamplerStateAddress());
	if (!RM::GetRenderManager().IsBackBufferCleaned())
	{
		RM::GetRenderManager().SetBackBuffer();
		RM::GetRenderManager().SetBackBufferCleaned(true);
	}
#endif
}

void test::CleanupDevice()
	{
	#if defined(DX11)
		if (GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()) GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()->ClearState();

		//if (g_SimeSamplerState.GetDXSamplerState()) g_SimeSamplerState.GetDXSamplerState()->Release();
		if (g_SimeTextureRV.GetDXSRV()) g_SimeTextureRV.GetDXSRV()->Release();
		if (g_SimeCBNeverChanges.GetCBNeverChanges()) g_SimeCBNeverChanges.GetCBNeverChanges()->Release();
		if (g_SimeCBChangeOnResize.GetCBChangesOnResize()) g_SimeCBChangeOnResize.GetCBChangesOnResize()->Release();
		if (g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()) g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()->Release();
		if (g_DirLightBuffer.BGetBuffer()) g_DirLightBuffer.BGetBuffer()->Release();
		if (g_PointLightBuffer.BGetBuffer()) g_PointLightBuffer.BGetBuffer()->Release();
		if (g_SpotLightBuffer.BGetBuffer()) g_SpotLightBuffer.BGetBuffer()->Release();
		if (g_SimeVertexBuffer.GetVertexBuffer()) g_SimeVertexBuffer.GetVertexBuffer()->Release();
		if (g_SimeIndexBuffer.GetIndexBuffer()) g_SimeIndexBuffer.GetIndexBuffer()->Release();
		//if (g_SimeInputLayout.GetDXInputLayout()) g_SimeInputLayout.GetDXInputLayout()->Release();
		//if (g_SimeVertexShader.GetDXVertexShader()) g_SimeVertexShader.GetDXVertexShader()->Release();
		//if (g_SimePixelShader.GetDXPixelShader()) g_SimePixelShader.GetDXPixelShader()->Release();
		if (RM::GetRenderManager().DepthStencil.GetTexture()) RM::GetRenderManager().DepthStencil.GetTexture()->Release();
		if (RM::GetRenderManager().DSView.GetDSV()) RM::GetRenderManager().DSView.GetDSV()->Release();
		//if (g_SimeRenderTargetView.GetRTV()) g_SimeRenderTargetView.GetRTV()->Release();
		//if (g_PositionRTV.GetRTV()) g_PositionRTV.GetRTV()->Release();

		for (unsigned int i = 0; i < RM::GetRenderManager().GBufferRTV.GetRTVNum(); ++i)
		{
			if (RM::GetRenderManager().GBufferRTV.GetRTV(i))
			{
				RM::GetRenderManager().GBufferRTV.GetRTV(i)->Release();
			}
		}
		for (unsigned int i = 0; i < RM::GetRenderManager().BackBufferRTV.GetRTVNum(); ++i)
		{
			if (RM::GetRenderManager().BackBufferRTV.GetRTV(i))
			{
				RM::GetRenderManager().BackBufferRTV.GetRTV(i)->Release();
			}
		}
		if (GetManagerObj(m_hwnd).GetSwapChain().GetDXSC()) GetManagerObj(m_hwnd).GetSwapChain().GetDXSC()->Release();
		if (GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()) GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()->Release();
		if (GetManagerObj(m_hwnd).GetDevice().GetDXDevice()) GetManagerObj(m_hwnd).GetDevice().GetDXDevice()->Release();
	#endif
	}

#if defined(OGL)
void test::UpdateOGL(GLFWwindow* _Window)
{
	int display_w, display_h;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);

	glfwGetFramebufferSize(_Window, &display_w, &display_h);
}
#endif

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