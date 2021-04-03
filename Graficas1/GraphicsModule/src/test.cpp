#include "test.h"

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
		int InfoLogLength;


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

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	 programID = LoadShaders("SimpleVertexShader.txt", "SimpleFragmentShader.txt");

	glUseProgram(programID);

	static const GLfloat g_vertex_buffer_data[] = {
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	};
	

	// Generar un buffer, poner el resultado en el vertexbuffer que acabamos de crear
	glGenBuffers(1, &vertexbuffer);
	// Los siguientes comandos le darán características especiales al 'vertexbuffer' 
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Darle nuestros vértices a  OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	static const GLfloat g_color_buffer_data[] = {
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	// back colors
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0,
	1.0, 1.0, 1.0
	};


	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	GLushort index_cube_elements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_cube_elements), index_cube_elements, GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST);



	 //model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
	 modelmatrix[0] = 1.0f;
	 modelmatrix[1] = 0.0f;
	 modelmatrix[2] = 0.0f;
	 modelmatrix[3] = 0.0f;

	 modelmatrix[4] = 0.0f;
	 modelmatrix[5] = 1.0f;
	 modelmatrix[6] = 0.0f;
	 modelmatrix[7] = 0.0f;

	 modelmatrix[8] = 0.0f;
	 modelmatrix[9] = 0.0f;
	 modelmatrix[10] = 1.0f;
	 modelmatrix[11] = 0.0f;

	 modelmatrix[12] = 0.0f;
	 modelmatrix[13] = 0.0f;
	 modelmatrix[14] = -4.0f;
	 modelmatrix[15] = 1.0f;
	 //view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	 //
	 //projection = glm::perspective(0.7863f, 1024.0f/768.0f, 0.1f, 10.0f);

	 UpdateProjectionMatrixStruct PMStruct;
	 PMStruct.AngleY = SIME_PIDIV4;
	 PMStruct.Ratio = 1024 / (FLOAT)768;
	 PMStruct.NearPlane = 0.01f;
	 PMStruct.FarPlane = 1000.0f;
	 PMStruct.Width = 1024;
	 PMStruct.Height = 768;

	 m_PerspectiveCamera.SetEye(0.0f, 2.0f, 0.0f);
	 m_PerspectiveCamera.SetAt(0.0f, 0.0f, -4.0f);
	 m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
	 m_PerspectiveCamera.UpdateViewMatrix();
	 m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(PMStruct);

	 m_OrtographicCamera.SetEye(0.0f, 2.0f, 0.0f);
	 m_OrtographicCamera.SetAt(0.0f, 0.0f, -4.0f);
	 m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
	 m_OrtographicCamera.UpdateViewMatrix();
	 m_OrtographicCamera.UpdateOrtographicProjectionMatrix(PMStruct);

	 m_Camera = &m_PerspectiveCamera;
	
	 ekisde = Matrix::MatrixMultiplication(m_Camera->GetPerspectiveProjectionMatrix(), m_Camera->GetViewMatrix());
	 truematrix = Matrix::MatrixMultiplication(ekisde, modelmatrix);

	 //mvp = glm::make_mat4(m_Camera->GetPerspectiveProjectionMatrix()) * glm::make_mat4(m_Camera->GetViewMatrix()) * glm::make_mat4(modelmatrix);
	 mvp = glm::make_mat4(truematrix);
	const char* uniform_name;
	uniform_name = "mvp";
	uniform_mvp = glGetUniformLocation(programID, uniform_name);
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	}
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
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
			
		// Create a render target view
		Texture2D BackBuffer;
		hr = GetManagerObj(hwnd).GetSwapChain().CGetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)BackBuffer.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		hr = GetManagerObj(hwnd).GetDevice().CCreateRenderTargetView(BackBuffer.GetTexture(), NULL, g_SimeRenderTargetView.GetRTVAdress());
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

		g_SimeDepthStencil.SetDescDepth(g_DepthDesc);
		hr = GetManagerObj(hwnd).GetDevice().CCreateTexture2D(g_SimeDepthStencil.GetDescDepthAddress(), NULL, g_SimeDepthStencil.GetTextureAddress());
		if (FAILED(hr))
			return hr;

		g_SimeDepthStencilView.SetDescDSV(SIME_FORMAT_D32_FLOAT, SIME_DSV_DIMENSION_TEXTURE2D, 0);
		hr = GetManagerObj(hwnd).GetDevice().CCreateDepthStencilView(g_SimeDepthStencil.GetTexture(), g_SimeDepthStencilView.GetDescDSVAddress(),																   g_SimeDepthStencilView.GetDSVAddress());
		if (FAILED(hr))
			return hr;

		g_SimeDepthStencilSRV.SetDesc(SIME_FORMAT_R32_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);
		hr = GetManagerObj(hwnd).GetDevice().CCreateShaderResourceView(g_SimeDepthStencil.GetTexture(), g_SimeDepthStencilSRV.GetDXSRVDescAddress(),
																	   g_SimeDepthStencilSRV.GetDXSRVAddress());
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

		// Compile the vertex shader
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile("Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
		if (FAILED(hr))
		{
			MessageBox(NULL,
				"The FX file cannot be compiled. Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			return hr;
		}

		CreateVertexShaderStruct VSStruct;
		VSStruct.pShaderBytecode = pVSBlob->GetBufferPointer();
		VSStruct.BytecodeLength = pVSBlob->GetBufferSize();
		VSStruct.pClassLinkage = NULL;
		VSStruct.ppVertexShader = g_SimeVertexShader.GetDXVertexShaderAddress();

		// Create the vertex shader
		hr = GetManagerObj(hwnd).GetDevice().CCreateVertexShader(VSStruct);
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

		CreateInputLayoutStruct ILStruct;
		ILStruct.pInputElementDescs = g_SimeInputLayout.GetDXInputLayoutDescAddress();
		ILStruct.NumElements = g_SimeInputLayout.GetDXInputLayoutDescSize();
		ILStruct.pShaderBytecodeWithInputSignature = pVSBlob->GetBufferPointer();
		ILStruct.BytecodeLength = pVSBlob->GetBufferSize();
		ILStruct.ppInputLayout = g_SimeInputLayout.GetDXInputLayoutAddress();

		hr = GetManagerObj(hwnd).GetDevice().CCreateInputLayout(ILStruct);
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

		CreatePixelShaderStruct PSStruct;
		PSStruct.pShaderBytecode = pPSBlob->GetBufferPointer();
		PSStruct.BytecodeLength = pPSBlob->GetBufferSize();
		PSStruct.pClassLinkage = NULL;
		PSStruct.ppPixelShader = g_SimePixelShader.GetDXPixelShaderAddress();

		// Create the pixel shader
		hr = GetManagerObj(hwnd).GetDevice().CCreatePixelShader(PSStruct);
		pPSBlob->Release();
		if (FAILED(hr))
			return hr;

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

		BDStruct.ByteWidth = sizeof(CBChangesEveryFrame);
		g_SimeCBChangesEveryFrame.UpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_SimeCBChangesEveryFrame.GetBDAddress(), NULL, g_SimeCBChangesEveryFrame.GetCBChangesEveryFrameAddress());
		if (FAILED(hr))
			return hr;

		BDStruct.ByteWidth = sizeof(DirLight);
		g_DirLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(g_DirLightBuffer.BGetBDAddress(), NULL, g_DirLightBuffer.BGetBufferAddress());

		// Load the Texture
		hr = D3DX11CreateShaderResourceViewFromFile(GetManagerObj(hwnd).GetDevice().GetDXDevice(), "base_albedo.dds", NULL, NULL, g_SimeTextureRV.GetDXSRVAddress(), NULL);
		if (FAILED(hr))
			return hr;

		g_SimeSamplerState.SetDesc();

		hr = GetManagerObj(hwnd).GetDevice().CCreateSamplerState(g_SimeSamplerState.GetDXSamplerDescAddress(), g_SimeSamplerState.GetDXSamplerStateAddress());
		if (FAILED(hr))
			return hr;

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
		RTDescStruct.Format = SIME_FORMAT_R8G8B8A8_UNORM;
		RTDescStruct.Count = 1;
		RTDescStruct.Quality = 0;
		RTDescStruct.Usage = SIME_USAGE_DEFAULT;
		RTDescStruct.CPUAccessFlags = 0;
		RTDescStruct.MiscFlags = 0;
		RTDescStruct.BindFlags = SIME_BIND_SHADER_RESOURCE | SIME_BIND_RENDER_TARGET;

		g_TextureRenderTarget.SetDescRT(RTDescStruct);
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

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	GetManagerObj(m_hwnd).GetDeviceContext().CClearRenderTargetView(g_SimeRenderTargetView.GetRTV(), ClearColor);

	ClearDepthStencilViewStruct ClearDSVStruct;
	ClearDSVStruct.pDepthStencilView = g_SimeDepthStencilView.GetDSV();
	ClearDSVStruct.ClearFlags = SIME_CLEAR_DEPTH;
	ClearDSVStruct.Depth = 1.0f;
	ClearDSVStruct.Stencil = 0;

	GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(m_Camera->GetViewMatrix());
	UpdateSubResourceStruct UpdateSBStruct;
	UpdateSBStruct.pDstResource = g_SimeCBNeverChanges.GetCBNeverChanges();
	UpdateSBStruct.DstSubresource = 0;
	UpdateSBStruct.pDstBox = NULL;
	UpdateSBStruct.pSrcData = &cbNeverChanges;
	UpdateSBStruct.SrcRowPitch = 0;
	UpdateSBStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
	UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
	UpdateSBStruct.pSrcData = &cbChangesOnResize;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);

	if (m_IsPerspectiveActive)
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
		UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
		UpdateSBStruct.pSrcData = &cbChangesOnResize;
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);
	}
	else
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetOrtographicProjectionMatrix());
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

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glVertexAttribPointer(
		0,                  // atributo 0. No hay razón particular para el 0, pero debe corresponder en el shader.
		3,                  // tamaño
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		0,                    // Paso
		(void*)0            // desfase del buffer
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // Atributo. No hay razón especial para el 1, pero debe corresponder al número en el shader.
		3,                                // tamaño
		GL_FLOAT,                         // tipo
		GL_FALSE,                         // normalizado?
		0,                                // corrimiento
		(void*)0                          // corrimiento de buffer
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);
#endif
#if defined(DX11)


	//MESH


	//AQUI
	GetManagerObj(m_hwnd).GetDeviceContext().COMSetRenderTargets(1, g_SimeRenderTargetView.GetRTVAdress(), g_SimeDepthStencilView.GetDSV());
	GetManagerObj(m_hwnd).GetDeviceContext().CRSSetViewports(1, g_SimeViewport.GetViewportAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CIASetInputLayout(g_SimeInputLayout.GetDXInputLayout());

	//LUEGO
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

#if defined(OGL)
void test::UpdateOGL(GLFWwindow* _Window)
{
	static float t;
	t += 10;
	int display_w, display_h;


	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
	//
	//glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	//
	//glm::mat4 projection = glm::perspective(0.7863f, 1024.0f / 768.0f, 0.1f, 10.0f);

	//mvp = glm::make_mat4(m_Camera->GetPerspectiveProjectionMatrix()) * glm::make_mat4(m_Camera->GetViewMatrix()) * glm::make_mat4(modelmatrix);
	//mvp = glm::make_mat4(truematrix);

	//const char* uniform_name;
	//uniform_name = "mvp";
	//uniform_mvp = glGetUniformLocation(programID, uniform_name);
	//if (uniform_mvp == -1) {
	//	fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	//}
	//glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

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