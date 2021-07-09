#include "Pass.h"
#include "test.h"
#include "RenderManager.h"

Pass::Pass()
{

}

Pass::Pass(const vector<tuple<string, string>> _Macros, HWND _hwnd, string _Name)
{	
	m_ID = 0;

	m_Name = _Name;

	if (m_Name == "GBuffer")
	{
		m_ShaderFilename = "GBuffer";
	}
	else if (m_Name == "GBufferLight")
	{
		m_ShaderFilename = "GBufferLight";
	}
	else if (m_Name == "ToneMap")
	{
		m_ShaderFilename = "ToneMap";
	}
	else if (m_Name == "Copy" || m_Name == "ForwardCopy")
	{
		m_ShaderFilename = "Copy";
	}
	else if (m_Name == "ForwardToneMap")
	{
		m_ShaderFilename = "ForwardToneMap";
	}
	else if (m_Name == "Light")
	{
		m_ShaderFilename = "Light";
	}
	else if (m_Name == "SSAO")
	{
		m_ShaderFilename = "SSAO";
	}
	else if (m_Name == "Skybox")
	{
		m_ShaderFilename = "Skybox";
	}

	m_Shader.SetMacros(_Macros);
	#if defined(OGL)
	m_Shader.CompileShaders(m_ShaderFilename);
	#endif
	#if defined(DX11)
	m_Shader.CompileShaders(m_VertexShader, m_InputLayout, m_ShaderReflection, m_PixelShader, m_ShaderFilename);
	#endif
}

Pass::~Pass()
{
}

void Pass::Render(bool _ReadSAQ, bool _ReadSkybox, bool _IsDef)
{
	HWND hwnd = NULL;

	#if defined(DX11)
	GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CVSSetShader(m_VertexShader.GetDXVertexShader(), NULL, 0);

	GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShader(m_PixelShader.GetDXPixelShader(), NULL, 0);

	GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CIASetInputLayout(m_InputLayout.GetDXInputLayout());
	#endif

	#if defined(OGL)
	if (m_Name == "Skybox")
	{
	//	glDepthMask(GL_FALSE);
	}
	glUseProgram(m_Shader.GetShaderID());
	#endif

	if (_ReadSAQ)
	{
		if (RM::GetRenderManager().m_vModels.size() > 2)
		{ 
			#if defined(DX11)
			if (m_Name == "GBufferLight")
			{ 
				for (unsigned int i = 0; i < 4; ++i)
				{
					GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(i, 1, RM::GetRenderManager().GBufferSRV[i].GetDXSRVAddress());
				}
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, 4, RM::GetRenderManager().GBufferSamplers.GetDXSamplerStateAddress());

				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(4, 1, RM::GetRenderManager().DiffSkyBoxSRVResource.GetDXSRVAddress());
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(4, 1, RM::GetRenderManager().SkyboxSam.GetSamplerAddress(1));
			}
			else if (m_Name == "ToneMap")
			{
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, RM::GetRenderManager().SkyboxSRV.GetDXSRVAddress());
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, 1, RM::GetRenderManager().DefToneMapSamplers.GetSamplerAddress(0));
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(1, 1, RM::GetRenderManager().DefSSAOSRV.GetDXSRVAddress());
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(1, 1, RM::GetRenderManager().DefToneMapSamplers.GetSamplerAddress(1));
			}
			else if (m_Name == "Copy")
			{
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, RM::GetRenderManager().DefCopySRV.GetDXSRVAddress());
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, 1, RM::GetRenderManager().DefCopySamplers.GetSamplerAddress(0));
			}
			else if (m_Name == "ForwardToneMap")
			{
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, RM::GetRenderManager().ForwardSRV[0].GetDXSRVAddress());
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, 1, RM::GetRenderManager().ForwardSamplers.GetSamplerAddress(0));
			}
			else if (m_Name == "ForwardCopy")
			{
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, RM::GetRenderManager().ForwardSRV[1].GetDXSRVAddress());
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, 1, RM::GetRenderManager().ForwardSamplers.GetSamplerAddress(1));
			}
			else if (m_Name == "SSAO")
			{
				for (unsigned int i = 0; i < 2; ++i)
				{
					GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(i, 1, RM::GetRenderManager().GBufferSRV[i].GetDXSRVAddress());
				}
				GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, RM::GetRenderManager().DefSSAOSamplers.GetSamplerNum(),
																					   RM::GetRenderManager().DefSSAOSamplers.GetDXSamplerStateAddress());
			}
			#endif
			#if defined(OGL)
			if (m_Name == "ForwardToneMap")
			{
				// Render to our framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().FrameBuffer2);
				// Set "renderedTexture" as our colour attachement #0
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM::GetRenderManager().LightTexOGL, 0);
				
				int ShaderID;
				glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);
				
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cout << "Error en el framebuffer" << endl;
				}
				
				RM::GetRenderManager().SetExpossure();
				
				glUniform1i(glGetUniformLocation(ShaderID, "RTLight"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SkyboxTexOGL);

				// Set the list of draw buffers.
				GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				
				glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
				//glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, 1920, 1080); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			}

			else if (m_Name == "ForwardCopy")
			{	
				int ShaderID;
				glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);
				
				glUniform1i(glGetUniformLocation(ShaderID, "RTFinal"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().LightTexOGL);
				
				if (!RM::GetRenderManager().IsBackBufferCleaned())
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0);

					glEnable(GL_DEPTH_TEST);

					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
					
					glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
					glViewport(0, 0, 1920, 1080);
					RM::GetRenderManager().SetBackBufferCleaned(true);
				}
			}
			else if (m_Name == "GBufferLight")
			{
				//glFrontFace(GL_CCW);
				//glCullFace(GL_FRONT);
				//glDisable(GL_CULL_FACE);
				const int nameSize = 50;
				unsigned int type, index;
				int UniformsNum, size;
				char name[nameSize];
				
				glGetProgramiv(m_Shader.GetShaderID(), GL_ACTIVE_UNIFORMS, &UniformsNum);
				
				for (unsigned int i = 0; i < UniformsNum; ++i)
				{
					glGetActiveUniform(m_Shader.GetShaderID(), i, nameSize, nullptr, &size, &type, &name[0]);
					index = glGetUniformLocation(m_Shader.GetShaderID(), &name[0]);
				}
				
				RM::GetRenderManager().SetLights();
				RM::GetRenderManager().SetViewPos();
				
				glUniform1i(glGetUniformLocation(m_Shader.GetShaderID(), "RTAlbedo"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().AlbedoTex);
				
				glUniform1i(glGetUniformLocation(m_Shader.GetShaderID(), "RTNormal"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().NormalTex);
				
				glUniform1i(glGetUniformLocation(m_Shader.GetShaderID(), "RTSpecular"), 2);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SpecularTex);
				
				glUniform1i(glGetUniformLocation(m_Shader.GetShaderID(), "RTPosition"), 3);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().PositionTex);
				
				glClear(SIME_DEPTH_BUFFER_BIT);
			}
			else if (m_Name == "SSAO")
			{
				// Render to our framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().SSAOFB);
				// Set "renderedTexture" as our colour attachement #0
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM::GetRenderManager().SSAOTex, 0);

				int ShaderID;
				glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cout << "Error en el framebuffer" << endl;
				}

				RM::GetRenderManager().SetSSAOValues();

				glUniform1i(glGetUniformLocation(ShaderID, "RTNormal"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().NormalTex);

				glUniform1i(glGetUniformLocation(ShaderID, "RTPosition"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().PositionTex);

				// Set the list of draw buffers.
				GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

				glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
				//glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, 1920, 1080); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			}
			else if (m_Name == "ToneMap")
			{
				// Render to our framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().ToneFB);
				// Set "renderedTexture" as our colour attachement #0
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM::GetRenderManager().ToneTex, 0);

				int ShaderID;
				glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cout << "Error en el framebuffer" << endl;
				}

				RM::GetRenderManager().SetExpossure();

				glUniform1i(glGetUniformLocation(ShaderID, "RTLight"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().DefSkyboxTex);

				glUniform1i(glGetUniformLocation(ShaderID, "RTSSAO"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SSAOTex);

				// Set the list of draw buffers.
				GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

				glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
				//glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, 1920, 1080); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			}
			else if (m_Name == "Copy")
			{
				int ShaderID;
				glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

				glUniform1i(glGetUniformLocation(ShaderID, "RTFinal"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().ToneTex);

				if (!RM::GetRenderManager().IsBackBufferCleaned())
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0);

					glEnable(GL_DEPTH_TEST);

					glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

					glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
					glViewport(0, 0, 1920, 1080);
					RM::GetRenderManager().SetBackBufferCleaned(true);
				}
			}
			#endif
			RM::GetRenderManager().m_vModels[0]->Render(hwnd);
		}
	}
	else
	{ 
		if (_ReadSkybox)
		{
			#if defined(DX11)
			GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetShaderResources(0, 1, RM::GetRenderManager().SkyboxSRVResource.GetDXSRVAddress());
			GraphicsModule::GetManagerObj(hwnd).GetDeviceContext().CPSSetSamplers(0, 1, RM::GetRenderManager().SkyboxSam.GetSamplerAddress(0));
			#endif
			#if defined(OGL)
			if (_IsDef)
			{
				//glFrontFace(GL_CW);
				//glCullFace(GL_BACK);

				glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().DefSkyboxFB);
				// Set "renderedTexture" as our colour attachement #0
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM::GetRenderManager().DefSkyboxTex, 0);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cout << "Error en el framebuffer" << endl;
				}

				RM::GetRenderManager().SetVP();

				glUniform1i(glGetUniformLocation(m_Shader.GetShaderID(), "SkyboxMap"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, RM::GetRenderManager().SkyboxTexResourceOGL);

				// Set the list of draw buffers.
				GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

				glClear(SIME_DEPTH_BUFFER_BIT);
				//glEnable(GL_DEPTH_TEST);
				glViewport(0, 0, 1920, 1080);
			}
			else
			{
				// Render to our framebuffer
				glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().FrameBuffer1);

				// Set "renderedTexture" as our colour attachement #0
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM::GetRenderManager().SkyboxTexOGL, 0);

				int ShaderID;
				glGetIntegerv(GL_CURRENT_PROGRAM, &ShaderID);

				//glEnable(GL_CULL_FACE);
				//glCullFace(GL_BACK);
				//glFrontFace(GL_CW);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cout << "Error en el framebuffer" << endl;
				}

				RM::GetRenderManager().SetVP();

				glUniform1i(glGetUniformLocation(ShaderID, "SkyboxMap"), 0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, RM::GetRenderManager().SkyboxTexResourceOGL);

				GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
				glDrawBuffers(1, DrawBuffers);

				glClear(SIME_DEPTH_BUFFER_BIT);
				glViewport(0, 0, 1920, 1080);
			}
			#endif
			RM::GetRenderManager().m_vModels[1]->Render(hwnd);
		}
		else
		{ 
			#if defined(OGL)
			if (_IsDef)
			{
				//glPolygonMode(GL_BACK, GL_FILL);
				//glEnable(GL_CULL_FACE);
				//glFrontFace(GL_CCW);
				//glCullFace(GL_BACK);

				glBindFramebuffer(GL_FRAMEBUFFER, RM::GetRenderManager().PositionFB);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RM::GetRenderManager().PositionTex, 0);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, RM::GetRenderManager().NormalTex, 0);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, RM::GetRenderManager().SpecularTex, 0);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, RM::GetRenderManager().AlbedoTex, 0);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				{
					cout << "Error en el framebuffer" << endl;
				}

				RM::GetRenderManager().SetVP();

				GLenum DrawBuffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
				glDrawBuffers(4, DrawBuffers);

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

				glClear(SIME_COLOR_BUFFER_BIT | SIME_DEPTH_BUFFER_BIT);
				//glClear(SIME_COLOR_BUFFER_BIT);
				glViewport(0, 0, 1920, 1080);
			}
			else
			{
				//glCullFace(GL_FRONT);
				//glDisable(GL_CULL_FACE);

				RM::GetRenderManager().SetVP();
				RM::GetRenderManager().SetLights();
				RM::GetRenderManager().SetViewPos();
			}
			#endif
			for (unsigned int i = 2; i < RM::GetRenderManager().m_vModels.size(); ++i)
			{
				RM::GetRenderManager().m_vModels[i]->Render(hwnd);
			}
		}
	}
}

void Pass::CleanUpShaders()
{
	#if defined(DX11)
	if (m_InputLayout.GetDXInputLayout()) m_InputLayout.GetDXInputLayout()->Release();
	if (m_VertexShader.GetDXVertexShader()) m_VertexShader.GetDXVertexShader()->Release();
	if (m_PixelShader.GetDXPixelShader()) m_PixelShader.GetDXPixelShader()->Release();
	#endif
}
