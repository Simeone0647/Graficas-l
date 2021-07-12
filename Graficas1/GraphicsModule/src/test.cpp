#include "test.h"
#include "RenderManager.h"

namespace GraphicsModule
{
	
	HRESULT test::InitDevice(HWND hwnd)
	{
	#if defined(OGL)
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_STENCIL_TEST);

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer. Framebuffer is the RenderTarget.
	//There's a default framebuffer that is the backbuffer.
	RM::GetRenderManager().FrameBuffer1 = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().FrameBuffer1);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().FrameBuffer1);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().SkyboxTexOGL);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SkyboxTexOGL);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().DepthBuffer1);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().DepthBuffer1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().DepthBuffer1);


	RM::GetRenderManager().FrameBuffer2 = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().FrameBuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().FrameBuffer2);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().LightTexOGL);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().LightTexOGL);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().DepthBuffer2);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().DepthBuffer2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().DepthBuffer2);
	
	//DEFERRED
	//ALBEDO
	RM::GetRenderManager().AlbedoFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().AlbedoFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().AlbedoFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().AlbedoTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().AlbedoTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().AlbedoDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().AlbedoDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().AlbedoDepth);

	//NORMAL
	RM::GetRenderManager().NormalFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().NormalFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().NormalFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().NormalTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().NormalTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().NormalDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().NormalDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().NormalDepth);
	//SPECULAR
	RM::GetRenderManager().SpecularFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().SpecularFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().SpecularFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().SpecularTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SpecularTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().SpecularDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().SpecularDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().SpecularDepth);
	//POSITION
	RM::GetRenderManager().PositionFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().PositionFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().PositionFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().PositionTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().PositionTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().PositionDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().PositionDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().PositionDepth);
	//SKYBOX
	RM::GetRenderManager().DefSkyboxFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().DefSkyboxFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().DefSkyboxFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().DefSkyboxTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().DefSkyboxTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().DefSkyboxDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().DefSkyboxDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().DefSkyboxDepth);
	//SSAO
	RM::GetRenderManager().SSAOFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().SSAOFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().SSAOFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().SSAOTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SSAOTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().SSAODepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().SSAODepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().SSAODepth);
	//TONE
	RM::GetRenderManager().ToneFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().ToneFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().ToneFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().ToneTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().ToneTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().ToneDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().ToneDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().ToneDepth);


	//SKELETON
	RM::GetRenderManager().SkeletonFB = 0;
	glGenFramebuffers(1, &RM::GetRenderManager().SkeletonFB);
	glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().SkeletonFB);

	// The texture we're going to render to
	glGenTextures(1, &RM::GetRenderManager().SkeletonTex);
	glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SkeletonTex);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &RM::GetRenderManager().SkeletonDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, RM::GetRenderManager().SkeletonDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RM::GetRenderManager().SkeletonDepth);
	//LOAD THE SKYBOX
	vector <string> Faces;
	Faces.push_back("left.jpg");
	Faces.push_back("right.jpg");
	Faces.push_back("bottom.jpg");
	Faces.push_back("top.jpg");
	Faces.push_back("back.jpg");
	Faces.push_back("front.jpg");

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	FIBITMAP* FirstDib;

	float Width;
	float Height;

	const unsigned char* Bits(0);
	for (unsigned int i = 0; i < Faces.size(); ++i)
	{
		FREE_IMAGE_FORMAT FIF = FIF_UNKNOWN;

		FIF = FreeImage_GetFileType(Faces[i].c_str(), 0);
		FirstDib = FreeImage_Load(FIF, Faces[i].c_str());

		if (Faces[i] == "top.jpg" || Faces[i] == "bottom.jpg")
		{
			FreeImage_FlipVertical(FirstDib);
			FreeImage_FlipHorizontal(FirstDib);
		}

		Bits = FreeImage_GetBits(FirstDib);
		Height = FreeImage_GetHeight(FirstDib);
		Width = FreeImage_GetWidth(FirstDib);

		if (!FirstDib)
		{
			std::cerr << "Error al cargar la imagen" << std::endl;
		}
		else
		{

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, Bits);

			//FreeImage_Unload(SecondDib);
			FreeImage_Unload(FirstDib);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	RM::GetRenderManager().SkyboxTexResourceOGL = textureID;
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

		BDStruct.ByteWidth = sizeof(RM::DirLight);
		RM::GetRenderManager().g_DirLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_DirLightBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_DirLightBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::PointLight);
		RM::GetRenderManager().g_PointLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_PointLightBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_PointLightBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::SpotLight);
		RM::GetRenderManager().g_SpotLightBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_SpotLightBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_SpotLightBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::Diffuse);
		RM::GetRenderManager().g_DiffuseBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_DiffuseBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_DiffuseBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::Ambient);
		RM::GetRenderManager().g_AmbientBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_AmbientBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_AmbientBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::Specular);
		RM::GetRenderManager().g_SpecularBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_SpecularBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_SpecularBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(Vector3);
		RM::GetRenderManager().g_CameraFrontBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_CameraFrontBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_CameraFrontBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::Expossure);
		RM::GetRenderManager().g_ExpossureBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_ExpossureBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_ExpossureBuffer.BGetBufferAddress());

		BDStruct.ByteWidth = sizeof(RM::AmbientOcclusion);
		RM::GetRenderManager().g_AOBuffer.BUpdateBD(BDStruct);
		hr = GetManagerObj(hwnd).GetDevice().CCreateBuffer(RM::GetRenderManager().g_AOBuffer.BGetBDAddress(), NULL, RM::GetRenderManager().g_AOBuffer.BGetBufferAddress());

		RM::UpdateProjectionMatrixStruct PMStruct;
		PMStruct.AngleY = SIME_PIDIV4;
		PMStruct.Ratio = width / (FLOAT)height;
		PMStruct.NearPlane = 0.01f;
		PMStruct.FarPlane = 1000.0f;
		PMStruct.Width = width;
		PMStruct.Height = height;

		RM::GetRenderManager().m_PerspectiveCamera.SetEye(0.0f, 3.0f, -6.0f);
		RM::GetRenderManager().m_PerspectiveCamera.SetAt(0.0f, 0.0f, 0.0f);
		RM::GetRenderManager().m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
		RM::GetRenderManager().m_PerspectiveCamera.UpdateViewMatrix();
		RM::GetRenderManager().m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(PMStruct);

		RM::GetRenderManager().m_OrtographicCamera.SetEye(0.0f, 3.0f, -6.0f);
		RM::GetRenderManager().m_OrtographicCamera.SetAt(0.0f, 0.0f, 0.0f);
		RM::GetRenderManager().m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
		RM::GetRenderManager().m_OrtographicCamera.UpdateViewMatrix();
		RM::GetRenderManager().m_OrtographicCamera.UpdateOrtographicProjectionMatrix(PMStruct);

		RM::GetRenderManager().m_Camera = &RM::GetRenderManager().m_PerspectiveCamera;


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

		//---------------------------- SKYBOX OUTPUT ----------------------------------- 
		RM::GetRenderManager().SkyboxRTV.AddRTV();

		RM::GetRenderManager().SkyboxTex.SetDescRT(RTDescStruct);
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().SkyboxTex.GetDescDepthAddress(), NULL,
			RM::GetRenderManager().SkyboxTex.GetTextureAddress());
		if (FAILED(hr))
		{
			cout << "Error TEXTURE2D" << endl;
		}

		RM::GetRenderManager().SkyboxSRV.SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().SkyboxTex.GetTexture(), RM::GetRenderManager().SkyboxSRV.GetDXSRVDescAddress(),
			RM::GetRenderManager().SkyboxSRV.GetDXSRVAddress());
		if (FAILED(hr))
		{
			cout << "Error SRV" << endl;
		}

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().SkyboxTex.GetTexture(), NULL, RM::GetRenderManager().SkyboxRTV.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Error RT" << endl;
		}

		//------------------------------------ GBUFFER INIT -------------------------------------
		for (unsigned int i = 0; i < 4; ++i)
		{
			RM::GetRenderManager().GBufferRTV.AddRTV();

			RM::GetRenderManager().GBufferTextures[i].SetDescRT(RTDescStruct);
			hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().GBufferTextures[i].GetDescDepthAddress(), NULL,
							   RM::GetRenderManager().GBufferTextures[i].GetTextureAddress());
			if (FAILED(hr))
			{
				cout << "Error TEXTURE2D" << endl;
			}

			RM::GetRenderManager().GBufferSRV[i].SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);

			hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().GBufferTextures[i].GetTexture(), RM::GetRenderManager().GBufferSRV[i].GetDXSRVDescAddress(),
																			 RM::GetRenderManager().GBufferSRV[i].GetDXSRVAddress());
			if (FAILED(hr))
			{
				cout << "Error SRV" << endl;
			}

			hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().GBufferTextures[i].GetTexture(), NULL,
																		   RM::GetRenderManager().GBufferRTV.GetLastElementAddress());

			RM::GetRenderManager().GBufferSamplers.AddSampler();
			RM::GetRenderManager().GBufferSamplers.AddDesc();

			RM::GetRenderManager().GBufferSamplers.SetDesc(false, i);

			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(RM::GetRenderManager().GBufferSamplers.GetDXSamplerDescAddress(i),
																					 RM::GetRenderManager().GBufferSamplers.GetLastElementAddress());
			if (FAILED(hr))
			{
				cout << "Fallo sampler" << endl;
			}
		}

		//---------------------------------- DEF TONEMAP INIT ----------------------------------------------------
		RM::GetRenderManager().DefToneMapRTV.AddRTV();

		RM::GetRenderManager().DefToneMapTex.SetDescRT(RTDescStruct);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().DefToneMapTex.GetDescDepthAddress(), NULL,
															    RM::GetRenderManager().DefToneMapTex.GetTextureAddress());
		if (FAILED(hr))
		{
			cout << "Error TEXTURE2D" << endl;
		}

		RM::GetRenderManager().DefToneMapSRV.SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().DefToneMapTex.GetTexture(), RM::GetRenderManager().DefToneMapSRV.GetDXSRVDescAddress(),
																		 RM::GetRenderManager().DefToneMapSRV.GetDXSRVAddress());
		if (FAILED(hr))
		{
			cout << "Error SRV" << endl;
		}

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().DefToneMapTex.GetTexture(), NULL,
																	   RM::GetRenderManager().DefToneMapRTV.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Error RTV" << endl;
		}

		for (unsigned int i = 0; i < 2; ++i)
		{
			RM::GetRenderManager().DefToneMapSamplers.AddSampler();
			RM::GetRenderManager().DefToneMapSamplers.AddDesc();
			RM::GetRenderManager().DefToneMapSamplers.SetDesc(true, i);

			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(RM::GetRenderManager().DefToneMapSamplers.GetDXSamplerDescAddress(i),
				RM::GetRenderManager().DefToneMapSamplers.GetLastElementAddress());
			if (FAILED(hr))
			{
				cout << "Fallo sampler" << endl;
			}
		}

		//----------------------------------- DEF COPY INIT ------------------------------------------------
		RM::GetRenderManager().DefCopyRTV.AddRTV();

		RM::GetRenderManager().DefCopyTex.SetDescRT(RTDescStruct);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().DefCopyTex.GetDescDepthAddress(), NULL,
			RM::GetRenderManager().DefCopyTex.GetTextureAddress());
		if (FAILED(hr))
		{
			cout << "Error TEXTURE2D" << endl;
		}

		RM::GetRenderManager().DefCopySRV.SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().DefCopyTex.GetTexture(), RM::GetRenderManager().DefCopySRV.GetDXSRVDescAddress(),
																		 RM::GetRenderManager().DefCopySRV.GetDXSRVAddress());
		if (FAILED(hr))
		{
			cout << "Error SRV" << endl;
		}

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().DefCopyTex.GetTexture(), NULL,
			RM::GetRenderManager().DefCopyRTV.GetLastElementAddress());

		if (FAILED(hr))
		{
			cout << "Error RTV" << endl;
		}

		RM::GetRenderManager().DefCopySamplers.AddSampler();
		RM::GetRenderManager().DefCopySamplers.AddDesc();
		RM::GetRenderManager().DefCopySamplers.SetDesc(true, 0);

		hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(RM::GetRenderManager().DefCopySamplers.GetDXSamplerDescAddress(0),
			RM::GetRenderManager().DefCopySamplers.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Fallo sampler" << endl;
		}
		//---------------------------- DEF SSAO INIT ----------------------------------------------------------
		RM::GetRenderManager().DefSSAORTV.AddRTV();

		RM::GetRenderManager().DefSSAOTex.SetDescRT(RTDescStruct);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().DefSSAOTex.GetDescDepthAddress(), NULL,
																RM::GetRenderManager().DefSSAOTex.GetTextureAddress());
		if (FAILED(hr))
		{
			cout << "Error TEXTURE2D" << endl;
		}

		RM::GetRenderManager().DefSSAOSRV.SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().DefSSAOTex.GetTexture(), RM::GetRenderManager().DefSSAOSRV.GetDXSRVDescAddress(),
																		 RM::GetRenderManager().DefSSAOSRV.GetDXSRVAddress());
		if (FAILED(hr))
		{
			cout << "Error SRV" << endl;
		}

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().DefSSAOTex.GetTexture(), NULL,
																	   RM::GetRenderManager().DefSSAORTV.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Error RTV" << endl;
		}

		for (unsigned int i = 0; i < 2; ++i)
		{
			RM::GetRenderManager().DefSSAOSamplers.AddSampler();
			RM::GetRenderManager().DefSSAOSamplers.AddDesc();
			RM::GetRenderManager().DefSSAOSamplers.SetDesc(true, i);

			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(RM::GetRenderManager().DefSSAOSamplers.GetDXSamplerDescAddress(i),
																					 RM::GetRenderManager().DefSSAOSamplers.GetLastElementAddress());
			if (FAILED(hr))
			{
				cout << "Fallo sampler" << endl;
			}
		}
		//----------------------------------------- GBUFFER RASTERIZER INIT ------------------------------------
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
		//--------------------------------------- LIGHT RASTERIZER INIT ------------------------------------------
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

		//--------------------------------------- TONE RASTERIZER INIT ---------------------------------------------
		RM::GetRenderManager().ToneRasterizer.SetRasterizerDesc(RasterStructGBufferLight);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRasterizerState(RM::GetRenderManager().ToneRasterizer.GetRSDescAddress(),
			RM::GetRenderManager().ToneRasterizer.GetRSAddress());
		if (FAILED(hr))
		{
			cout << "Fallo en crear el RasterizerLight" << endl;
		}

		//------------------------------------------ COPY RASTERIZER INIT ----------------------------------------
		RM::GetRenderManager().CopyRasterizer.SetRasterizerDesc(RasterStructGBufferLight);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRasterizerState(RM::GetRenderManager().CopyRasterizer.GetRSDescAddress(),
			RM::GetRenderManager().CopyRasterizer.GetRSAddress());
		if (FAILED(hr))
		{
			cout << "Fallo en crear el RasterizerLight" << endl;
		}
		
		// --------------------------- FORWARD RENDER --------------------------------------------------
		for (unsigned int i = 0; i < 3; ++i)
		{
			RM::GetRenderManager().ForwardTextures[i].SetDescRT(RTDescStruct);

			hr = GetManagerObj(m_hwnd).GetDevice().CCreateTexture2D(RM::GetRenderManager().ForwardTextures[i].GetDescDepthAddress(), NULL,
				RM::GetRenderManager().ForwardTextures[i].GetTextureAddress());

			if (FAILED(hr))
			{
				cout << "Error tekstur 2d" << endl;
			}

			RM::GetRenderManager().ForwardSRV[i].SetDesc(SIME_FORMAT_R16G16B16A16_FLOAT, SIME11_SRV_DIMENSION_TEXTURE2D, 1);

			hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().ForwardTextures[i].GetTexture(), RM::GetRenderManager().ForwardSRV[i].GetDXSRVDescAddress(),
				RM::GetRenderManager().ForwardSRV[i].GetDXSRVAddress());
			if (FAILED(hr))
			{
				cout << "Error cheiderresoursviu" << endl;
			}

			RM::GetRenderManager().ForwardSamplers.AddSampler();
			RM::GetRenderManager().ForwardSamplers.AddDesc();
			RM::GetRenderManager().ForwardSamplers.SetDesc(true, i);

			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(RM::GetRenderManager().ForwardSamplers.GetDXSamplerDescAddress(i),
																					 RM::GetRenderManager().ForwardSamplers.GetLastElementAddress());
			if (FAILED(hr))
			{
				cout << "Fallo sampler" << endl;
			}
		}

		RM::GetRenderManager().ForwardLightRTV.AddRTV();
		RM::GetRenderManager().ForwardToneMapRTV.AddRTV();
		RM::GetRenderManager().ForwardSkeletonRTV.AddRTV();

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().ForwardTextures[0].GetTexture(), NULL,
			RM::GetRenderManager().ForwardLightRTV.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Error Render Target" << endl;
		}
		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().ForwardTextures[1].GetTexture(), NULL,
			RM::GetRenderManager().ForwardToneMapRTV.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Error Render Target" << endl;
		}

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRenderTargetView(RM::GetRenderManager().ForwardTextures[2].GetTexture(), NULL,
			RM::GetRenderManager().ForwardSkeletonRTV.GetLastElementAddress());
		if (FAILED(hr))
		{
			cout << "Error Render Target" << endl;
		}

		//----------------------------------- SKYBOX TEXTURE RESOURCE -----------------------------------------------
		hr = D3DX11CreateTextureFromFile(GetManagerObj(m_hwnd).GetDevice().GetDXDevice(), "Mars.dds", NULL, 0,
										(ID3D11Resource**)RM::GetRenderManager().SkyboxTexResource.GetTextureAddress(), 0);
		if (FAILED(hr))
		{
			cout << "Error TEXTURE2D" << endl;
		}

		D3D11_TEXTURE2D_DESC SMTextureDesc;
		RM::GetRenderManager().SkyboxTexResource.GetTexture()->GetDesc(&SMTextureDesc);

		RM::GetRenderManager().SkyboxSRVResource.SetDesc((SIME_FORMAT)SMTextureDesc.Format, SIME11_SRV_DIMENSION_TEXTURECUBE, SMTextureDesc.MipLevels);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().SkyboxTexResource.GetTexture(),
																		 RM::GetRenderManager().SkyboxSRVResource.GetDXSRVDescAddress(),
																		 RM::GetRenderManager().SkyboxSRVResource.GetDXSRVAddress());
		if (FAILED(hr))
		{
			cout << "Error SRV" << endl;
		}

		//--------------------------------- SKYBOX DIFFUSE RESOURCE ---------------------------------------------------
		hr = D3DX11CreateTextureFromFile(GetManagerObj(m_hwnd).GetDevice().GetDXDevice(), "Diffuse_Mars.dds", NULL, 0,
			(ID3D11Resource**)RM::GetRenderManager().DiffSkyBoxTexResource.GetTextureAddress(), 0);
		if (FAILED(hr))
		{
			cout << "Error TEXTURE2D" << endl;
		}


		D3D11_TEXTURE2D_DESC SMTextureDesc2;
		RM::GetRenderManager().DiffSkyBoxTexResource.GetTexture()->GetDesc(&SMTextureDesc2);

		RM::GetRenderManager().DiffSkyBoxSRVResource.SetDesc((SIME_FORMAT)SMTextureDesc2.Format, SIME11_SRV_DIMENSION_TEXTURECUBE, SMTextureDesc2.MipLevels);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateShaderResourceView(RM::GetRenderManager().DiffSkyBoxTexResource.GetTexture(),
																		 RM::GetRenderManager().DiffSkyBoxSRVResource.GetDXSRVDescAddress(),
																		 RM::GetRenderManager().DiffSkyBoxSRVResource.GetDXSRVAddress());
		if (FAILED(hr))
		{
			cout << "Error SRV" << endl;
		}


		//------------------------------- SKYBOX RASTERIZER -------------------------------------------
		SetRasterizerStruct RasterStructSkybox;
		RasterStructSkybox.Fill = SIME11_FILL_SOLID;
		RasterStructSkybox.Cull = SIME11_CULL_BACK;
		RasterStructSkybox.FrontCCW = true;

		RM::GetRenderManager().SkyboxRaster.SetRasterizerDesc(RasterStructSkybox);

		hr = GetManagerObj(m_hwnd).GetDevice().CCreateRasterizerState(RM::GetRenderManager().SkyboxRaster.GetRSDescAddress(),
			RM::GetRenderManager().SkyboxRaster.GetRSAddress());
		if (FAILED(hr))
		{
			cout << "Error RASTERIZER" << endl;
		}

		//------------------------------- SKYBOX SAMPLERS ----------------------------------------------
		for (unsigned int i = 0; i < 2; ++i)
		{
			RM::GetRenderManager().SkyboxSam.AddSampler();
			RM::GetRenderManager().SkyboxSam.AddDesc();
			RM::GetRenderManager().SkyboxSam.SetDesc(false, i);
			hr = GraphicsModule::GetManagerObj(hwnd).GetDevice().CCreateSamplerState(RM::GetRenderManager().SkyboxSam.GetDXSamplerDescAddress(i),
																				   	 RM::GetRenderManager().SkyboxSam.GetLastElementAddress());
			if (FAILED(hr))
			{
				cout << "Fallo sampler" << endl;
			}
		}
#endif
		return S_OK;
}

void test::Update()
{
#if defined(DX11)
	static float t = 0.0f;
	int counter = 0;

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

	ClearDepthStencilViewStruct ClearDSVStruct;
	ClearDSVStruct.pDepthStencilView = RM::GetRenderManager().DSView.GetDSV();
	ClearDSVStruct.ClearFlags = SIME_CLEAR_DEPTH;
	ClearDSVStruct.Depth = 1.0f;
	ClearDSVStruct.Stencil = 0;

	GetManagerObj(m_hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);

	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(RM::GetRenderManager().m_Camera->m_ViewMatrix);

	UpdateSubResourceStruct UpdateSBStruct;
	UpdateSBStruct.pDstResource = g_SimeCBNeverChanges.GetCBNeverChanges();
	UpdateSBStruct.DstSubresource = 0;
	UpdateSBStruct.pDstBox = NULL;
	UpdateSBStruct.pSrcData = &cbNeverChanges;
	UpdateSBStruct.SrcRowPitch = 0;
	UpdateSBStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(RM::GetRenderManager().m_Camera->m_ProjectionMatrix);
	UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
	UpdateSBStruct.pSrcData = &cbChangesOnResize;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);

	if (m_IsPerspectiveActive)
	{
		cbChangesOnResize.mProjection = XMMatrixTranspose(RM::GetRenderManager().m_Camera->m_ProjectionMatrix);
		UpdateSBStruct.pDstResource = g_SimeCBChangeOnResize.GetCBChangesOnResize();
		UpdateSBStruct.pSrcData = &cbChangesOnResize;
		GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);
	}
	else
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(RM::GetRenderManager().m_Camera->m_ProjectionMatrix);
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
	if (!RM::GetRenderManager().IsBackBufferCleaned())
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, 1920, 1080);
		RM::GetRenderManager().SetBackBufferCleaned(true);
	}
#endif
#if defined(DX11)
	GraphicsModule::UpdateSubResourceStruct UpdateSRStruct;
	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_DirLightBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_DirLightBufferDesc;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_PointLightBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_PointLightBufferDesc;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_SpotLightBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_SpotLightBufferDesc;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_AmbientBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_Ambient;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_DiffuseBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_Diffuse;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_SpecularBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_Specular;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_CameraFrontBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = RM::GetRenderManager().m_Camera->GetEyeAddress();
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_ExpossureBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_Expossure;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	UpdateSRStruct.pDstResource = RM::GetRenderManager().g_AOBuffer.BGetBuffer();
	UpdateSRStruct.DstSubresource = 0;
	UpdateSRStruct.pDstBox = NULL;
	UpdateSRStruct.pSrcData = &RM::GetRenderManager().g_AO;
	UpdateSRStruct.SrcRowPitch = 0;
	UpdateSRStruct.SrcDepthPitch = 0;
	GetManagerObj(m_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSRStruct);

	GetManagerObj(m_hwnd).GetDeviceContext().CRSSetViewports(1, g_SimeViewport.GetViewportAddress());

	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(0, 1, g_SimeCBNeverChanges.GetCBNeverChangesAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(1, 1, g_SimeCBChangeOnResize.GetCBChangeOnResizeAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(3, 1, RM::GetRenderManager().g_DirLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(4, 1, RM::GetRenderManager().g_PointLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(5, 1, RM::GetRenderManager().g_SpotLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(6, 1, RM::GetRenderManager().g_SpecularBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(7, 1, RM::GetRenderManager().g_AmbientBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(8, 1, RM::GetRenderManager().g_DiffuseBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CVSSetConstantBuffers(9, 1, RM::GetRenderManager().g_CameraFrontBuffer.BGetBufferAddress());

	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(3, 1, RM::GetRenderManager().g_DirLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(4, 1, RM::GetRenderManager().g_PointLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(5, 1, RM::GetRenderManager().g_SpotLightBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(6, 1, RM::GetRenderManager().g_SpecularBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(7, 1, RM::GetRenderManager().g_AmbientBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(8, 1, RM::GetRenderManager().g_DiffuseBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(9, 1, RM::GetRenderManager().g_CameraFrontBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(10, 1, RM::GetRenderManager().g_ExpossureBuffer.BGetBufferAddress());
	GetManagerObj(m_hwnd).GetDeviceContext().CPSSetConstantBuffers(11, 1, RM::GetRenderManager().g_AOBuffer.BGetBufferAddress());

	if (!RM::GetRenderManager().IsBackBufferCleaned())
	{
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		RM::GetRenderManager().SetRenderTarget(ClearColor, RM::GetRenderManager().BackBufferRTV, RM::GetRenderManager().DSView);
		RM::GetRenderManager().SetBackBufferCleaned(true);
	}
#endif
}

void test::CleanupDevice()
	{
	#if defined(DX11)
		if (GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()) GetManagerObj(m_hwnd).GetDeviceContext().GetDXDC()->ClearState();

		if (g_SimeTextureRV.GetDXSRV()) g_SimeTextureRV.GetDXSRV()->Release();
		if (g_SimeCBNeverChanges.GetCBNeverChanges()) g_SimeCBNeverChanges.GetCBNeverChanges()->Release();
		if (g_SimeCBChangeOnResize.GetCBChangesOnResize()) g_SimeCBChangeOnResize.GetCBChangesOnResize()->Release();
		if (g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()) g_SimeCBChangesEveryFrame.GetCBChangesEveryFrame()->Release();
		if (RM::GetRenderManager().g_DirLightBuffer.BGetBuffer()) RM::GetRenderManager().g_DirLightBuffer.BGetBuffer()->Release();
		if (RM::GetRenderManager().g_PointLightBuffer.BGetBuffer()) RM::GetRenderManager().g_PointLightBuffer.BGetBuffer()->Release();
		if (RM::GetRenderManager().g_SpotLightBuffer.BGetBuffer()) RM::GetRenderManager().g_SpotLightBuffer.BGetBuffer()->Release();
		if (g_SimeVertexBuffer.GetVertexBuffer()) g_SimeVertexBuffer.GetVertexBuffer()->Release();
		if (g_SimeIndexBuffer.GetIndexBuffer()) g_SimeIndexBuffer.GetIndexBuffer()->Release();
		if (RM::GetRenderManager().DepthStencil.GetTexture()) RM::GetRenderManager().DepthStencil.GetTexture()->Release();
		if (RM::GetRenderManager().DSView.GetDSV()) RM::GetRenderManager().DSView.GetDSV()->Release();

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