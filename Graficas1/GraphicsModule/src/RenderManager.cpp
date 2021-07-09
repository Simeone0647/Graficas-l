#include "RenderManager.h"
#include "test.h"

namespace RM
{
	RenderManager::RenderManager()
	{
		m_BackBufferCleaned = false;

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
	}
	
	RenderManager::~RenderManager()
	{
	}

	void RenderManager::SetRenderTarget(const float _ClearColor[], RenderTargetView& _RenderTarget, DepthStencilView& _DepthStencil)
	{
		#if defined(DX11)
		HWND hwnd = NULL;
		
		if (_ClearColor != NULL)
		{
			for (unsigned int i = 0; i < _RenderTarget.GetRTVNum(); ++i)
			{
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CClearRenderTargetView(_RenderTarget.GetRTV(i), _ClearColor);
			}
		}

		if (_DepthStencil.GetDSV() != NULL)
		{ 
			GraphicsModule::ClearDepthStencilViewStruct ClearDSVStruct;
			ClearDSVStruct.pDepthStencilView = _DepthStencil.GetDSV();
			ClearDSVStruct.ClearFlags = GraphicsModule::SIME_CLEAR_DEPTH;
			ClearDSVStruct.Depth = 1.0f;
			ClearDSVStruct.Stencil = 0;

			GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);
		}
		
		vector<ShaderResourceView> FreeSRVs;
		for (unsigned int i = 0; i < _RenderTarget.GetRTVNum(); ++i)
		{
			FreeSRVs.push_back(ShaderResourceView());
			GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(i, 1, FreeSRVs[i].GetDXSRVAddress());
		}

		GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().COMSetRenderTargets(_RenderTarget.GetRTVNum(), _RenderTarget.GetRTVAdress(), _DepthStencil.GetDSV());
		#endif
	}

	void RenderManager::SetLights()
	{
		#if defined(OGL)
		int ShaderID;
		const int nameSize = 50;
		unsigned int type, index;
		int UniformsNum, size;
		char name[nameSize];

		glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

		glGetProgramiv(ShaderID, GL_ACTIVE_UNIFORMS, &UniformsNum);

		for (unsigned int i = 0; i < UniformsNum; ++i)
		{
			glGetActiveUniform(ShaderID, i, nameSize, nullptr, &size, &type, &name[0]);
			index = glGetUniformLocation(ShaderID, &name[0]);
		}

		const char* UniformNameLight;
		int UniformLight;
		UniformNameLight = "LightDir";
		UniformLight = glGetUniformLocation(ShaderID, UniformNameLight);

		if (UniformLight == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameLight);
		}
		glUniform4fv(UniformLight, 1, glm::value_ptr(glm::make_vec4(g_DirLightBufferDesc.Dir)));

		const char* UniformNameLightColor;
		int UniformLightColor;
		UniformNameLightColor = "LightDirColor";
		UniformLightColor = glGetUniformLocation(ShaderID, UniformNameLightColor);
		if (UniformLightColor == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameLightColor);
		}
		glUniform4fv(UniformLightColor, 1, glm::value_ptr(glm::make_vec4(g_DirLightBufferDesc.Color)));

		//POINTLIGHT
		const char* UniformNamePointLightPos;
		int UniformPointLightPos;
		UniformNamePointLightPos = "PointLightPos";
		UniformPointLightPos = glGetUniformLocation(ShaderID, UniformNamePointLightPos);
		if (UniformPointLightPos == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNamePointLightPos);
		}
		glUniform4fv(UniformPointLightPos, 1, glm::value_ptr(glm::make_vec4(g_PointLightBufferDesc.Position)));

		const char* UniformNamePointLightColor;
		int UniformPointLightColor;
		UniformNamePointLightColor = "PointLightColor";
		UniformPointLightColor = glGetUniformLocation(ShaderID, UniformNamePointLightColor);
		if (UniformPointLightColor == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNamePointLightColor);
		}
		glUniform4fv(UniformPointLightColor, 1, glm::value_ptr(glm::make_vec4(g_PointLightBufferDesc.Color)));

		const char* UniformNamePointLightAttenuation;
		int UniformPointLightAttenuation;
		UniformNamePointLightAttenuation = "PointLightAttenuation";
		UniformPointLightAttenuation = glGetUniformLocation(ShaderID, UniformNamePointLightAttenuation);
		if (UniformPointLightAttenuation == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNamePointLightAttenuation);
		}
		glUniform4fv(UniformPointLightAttenuation, 1, glm::value_ptr(glm::make_vec4(g_PointLightBufferDesc.Attenuation)));

		//SPOTLIGHT
		const char* UniformNameSpotLightDir;
		int UniformSpotLightDir;
		UniformNameSpotLightDir = "SpotLightDir";
		UniformSpotLightDir = glGetUniformLocation(ShaderID, UniformNameSpotLightDir);
		if (UniformSpotLightDir == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightDir);
		}
		glUniform4fv(UniformSpotLightDir, 1, glm::value_ptr(glm::make_vec4(g_SpotLightBufferDesc.Dir)));

		const char* UniformNameSpotLightPos;
		int UniformSpotLightPos;
		UniformNameSpotLightPos = "SpotLightPos";
		UniformSpotLightPos = glGetUniformLocation(ShaderID, UniformNameSpotLightPos);
		if (UniformSpotLightPos == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightPos);
		}
		glUniform4fv(UniformSpotLightPos, 1, glm::value_ptr(glm::make_vec4(g_SpotLightBufferDesc.Pos)));

		const char* UniformNameSpotLightColor;
		int UniformSpotLightColor;
		UniformNameSpotLightColor = "SpotLightColor";
		UniformSpotLightColor = glGetUniformLocation(ShaderID, UniformNameSpotLightColor);
		if (UniformSpotLightColor == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightColor);
		}
		glUniform4fv(UniformSpotLightColor, 1, glm::value_ptr(glm::make_vec4(g_SpotLightBufferDesc.Color)));

		const char* UniformNameSpotLightAttenuation;
		int UniformSpotLightAttenuation;
		UniformNameSpotLightAttenuation = "SpotLightAttenuation";
		UniformSpotLightAttenuation = glGetUniformLocation(ShaderID, UniformNameSpotLightAttenuation);
		if (UniformSpotLightAttenuation == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightAttenuation);
		}
		glUniform1f(UniformSpotLightAttenuation, g_SpotLightBufferDesc.Attenuation);

		const char* UniformNameSpotLightInner;
		int UniformSpotLightInner;
		UniformNameSpotLightInner = "SpotLightInner";
		UniformSpotLightInner = glGetUniformLocation(ShaderID, UniformNameSpotLightInner);
		if (UniformSpotLightInner == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightInner);
		}
		glUniform1f(UniformSpotLightInner, g_SpotLightBufferDesc.InnerRadius);

		const char* UniformNameSpotLightOuter;
		int UniformSpotLightOuter;
		UniformNameSpotLightOuter = "SpotLightOuter";
		UniformSpotLightOuter = glGetUniformLocation(ShaderID, UniformNameSpotLightOuter);
		if (UniformSpotLightOuter == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameSpotLightOuter);
		}
		glUniform1f(UniformSpotLightOuter, g_SpotLightBufferDesc.OuterRadius);

		const char* UniformNamekSpecular;
		int UniformkSpecular;
		UniformNamekSpecular = "kSpecular";
		UniformkSpecular = glGetUniformLocation(ShaderID, UniformNamekSpecular);
		if (UniformkSpecular == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNamekSpecular);
		}
		glUniform1f(UniformkSpecular, g_Specular.kSpecular[0]);

		const char* UniformNameShininess;
		int UniformShininess;
		UniformNameShininess = "Shininess";
		UniformShininess = glGetUniformLocation(ShaderID, UniformNameShininess);
		if (UniformShininess == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameShininess);
		}
		glUniform1f(UniformShininess, g_Specular.Shininess[0]);

		const char* UniformNamekDiffuse;
		int UniformkDiffuse;
		UniformNamekDiffuse = "kDiffuse";
		UniformkDiffuse = glGetUniformLocation(ShaderID, UniformNamekDiffuse);
		if (UniformkDiffuse == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNamekDiffuse);
		}
		glUniform1f(UniformkDiffuse, g_Diffuse.kDiffuse[0]);

		const char* UniformNameAmbientColor;
		int UniformAmbientColor;
		UniformNameAmbientColor = "AmbientLightColor";
		UniformAmbientColor = glGetUniformLocation(ShaderID, UniformNameAmbientColor);
		if (UniformAmbientColor == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameAmbientColor);
		}
		glUniform4fv(UniformAmbientColor, 1, glm::value_ptr(glm::make_vec4(g_Ambient.AmbientColor)));

		const char* UniformNamekAmbient;
		int UniformkAmbient;
		UniformNamekAmbient = "kAmbient";
		UniformkAmbient = glGetUniformLocation(ShaderID, UniformNamekAmbient);
		if (UniformkAmbient == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNamekAmbient);
		}
		glUniform4fv(UniformkAmbient, 1, glm::value_ptr(glm::make_vec4(g_Ambient.kAmbient)));
		#endif
	}

	void RenderManager::SetViewPos()
	{
		#if defined(OGL)
		int ShaderID;
		glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

		const char* UniformNameViewPos;
		int UniformViewPos;
		UniformNameViewPos = "ViewPosition";
		UniformViewPos = glGetUniformLocation(ShaderID, UniformNameViewPos);
		if (UniformViewPos == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameViewPos);
		}
		glUniform4fv(UniformViewPos, 1, glm::value_ptr(glm::make_vec4(m_Camera->m_FakeEye)));
		#endif
	}

	void RenderManager::SetExpossure()
	{
		#if defined(OGL)
		int ShaderID;
		glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

		const char* UniformNameExpossure;
		int UniformExpossure;
		UniformNameExpossure = "Expossure";
		UniformExpossure = glGetUniformLocation(ShaderID, UniformNameExpossure);
		if (UniformExpossure == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameExpossure);
		}
		glUniform1f(UniformExpossure, g_Expossure.Expo[0]);
		#endif
	}

	void RenderManager::SetSSAOValues()
	{
		#if defined(OGL)
		int ShaderID;
		glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

		const char* UniformNameScale;
		int UniformScale;
		UniformNameScale = "Scale";
		UniformScale = glGetUniformLocation(ShaderID, UniformNameScale);
		if (UniformScale == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameScale);
		}
		glUniform1f(UniformScale, g_AO.Scale);

		const char* UniformNameRadius;
		int UniformRadius;
		UniformNameRadius = "Radius";
		UniformRadius = glGetUniformLocation(ShaderID, UniformNameRadius);
		if (UniformRadius == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameRadius);
		}
		glUniform1f(UniformRadius, g_AO.Radius);

		const char* UniformNameBias;
		int UniformBias;
		UniformNameBias = "Bias";
		UniformBias = glGetUniformLocation(ShaderID, UniformNameBias);
		if (UniformBias == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameBias);
		}
		glUniform1f(UniformBias, g_AO.Bias);

		const char* UniformNameIntensity;
		int UniformIntensity;
		UniformNameIntensity = "Intensity";
		UniformIntensity = glGetUniformLocation(ShaderID, UniformNameIntensity);
		if (UniformIntensity == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameIntensity);
		}
		glUniform1f(UniformIntensity, g_AO.Intensity);

		const char* UniformNameIterations;
		int UniformIterations;
		UniformNameIterations = "Iterations";
		UniformIterations = glGetUniformLocation(ShaderID, UniformNameIterations);
		if (UniformIterations == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameIterations);
		}
		glUniform1i(UniformIterations, g_AO.Iterations);
		#endif
	}

	void RenderManager::LoadModel(const string _Filename)
	{
		m_vModels.push_back(make_unique<Model>());
		
		m_vModels.back()->Load(_Filename);
	}

	void RenderManager::Update(const float _Time)
	{
		for (unsigned int i = 0; i < m_vModels.size(); ++i)
		{
			m_vModels[i]->Update(_Time);
		}
	}

	void RenderManager::SetVP()
	{
		#if defined(OGL)
		int ShaderID;
		glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

		const char* UniformNameView;
		int UniformView;
		UniformNameView = "View";
		UniformView = glGetUniformLocation(ShaderID, UniformNameView);
		if (UniformView == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameView);
		}
		glUniformMatrix4fv(UniformView, 1, 0, glm::value_ptr(glm::make_mat4(m_Camera->m_ViewMatrix)));

		const char* UniformNameProj;
		int UniformProj;
		UniformNameProj = "Projection";
		UniformProj = glGetUniformLocation(ShaderID, UniformNameProj);
		if (UniformProj == -1)
		{
			fprintf(stderr, "Could not bind uniform %s\n", UniformNameProj);
		}
		glUniformMatrix4fv(UniformProj, 1, 0, glm::value_ptr(glm::make_mat4(m_Camera->m_ProjectionMatrix)));
		#endif
	}
	
	extern RenderManager& GetRenderManager()
	{
		static RenderManager* pRenderManager = nullptr;
		if (pRenderManager == nullptr)
		{
			pRenderManager = new RenderManager();
		}
		return *pRenderManager;
	}
}