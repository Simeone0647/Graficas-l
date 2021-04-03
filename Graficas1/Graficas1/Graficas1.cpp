#pragma comment(lib, "ComDlg32.lib")
#include "Windows.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_dx11.h"
#include <vector>
#include "GraphicModule.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(OGL)
#include <glad/glad.h>
#include <glfw3.h>
#endif

#if defined(DX11)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HWND g_hwnd;
GraphicsModule::test m_Obj;
std::vector<Mesh> m_vModels;
int m_ModelsNum = 0;
#if defined(OGL)
GLFWwindow* m_OGLWindow; // (En el código que viene aqui, está variable es global)
#endif
//-----------------------------------------------------------------------------------------

#if defined(OGL)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
#endif
/**
	 * @brief   Forward declare message handler from imgui_impl_win32.cpp
	 * @param   #HWND: A handle to the window.
	 * @param   #UINT: The message.
	 * @param   #WPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
	 * @param   #LPARAM: Additional message information. The contents of this parameter depend on the value of the uMsg parameter.
	 * @return  #LRESULT: The return value is the result of the message processing and depends on the message sent..
	 * @bug     No know Bugs.
	 * @return  #LRESULT: Status code.
 */

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	Vector3 cam_pos;
	// Handle UI inputs
	//if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
	//	return 1;

	// Handle Window inputs
	switch (_msg)
	{
	case WM_SIZE:
		//if (g_pd3dDevice != NULL && _wParam != SIZE_MINIMIZED)
		//{
		//}
		//return 0;
		break;

	case WM_SYSCOMMAND:
		if ((_wParam & 0xfff0) == SC_KEYMENU)
		{
			return 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	#if defined(DX11)
	case WM_KEYDOWN:
	{
		if (LOWORD(_wParam) == 'W')
		{
			
			cam_pos.SetValues(0.0f, 0.1f, 0.0f);
			m_Obj.m_Camera->MoveCamera(cam_pos);
			m_Obj.m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'A')
		{
			cam_pos.SetValues(-0.1f, 0.0f, 0.0f);
			m_Obj.m_Camera->MoveCamera(cam_pos);
			m_Obj.m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'S')
		{
			cam_pos.SetValues(0.0f, -0.1f, 0.0f);
			m_Obj.m_Camera->MoveCamera(cam_pos);
			m_Obj.m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'D')
		{
			cam_pos.SetValues(0.1f, 0.0f, 0.0f);
			m_Obj.m_Camera->MoveCamera(cam_pos);
			m_Obj.m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'Q')
		{
			cam_pos.SetValues(0.0f, 0.0f, 0.1f);
			m_Obj.m_Camera->MoveCamera(cam_pos);
			m_Obj.m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'E')
		{
			cam_pos.SetValues(0.0f, 0.0f, -0.1f);
			m_Obj.m_Camera->MoveCamera(cam_pos);
			m_Obj.m_Camera->UpdateViewMatrix();
		}
		if (_wParam == VK_TAB)
		{
			if (m_Obj.m_IsPerspectiveActive)
			{
				m_Obj.m_Camera = &m_Obj.m_OrtographicCamera;
				m_Obj.m_IsPerspectiveActive = false;
			}
			else
			{
				m_Obj.m_Camera = &m_Obj.m_PerspectiveCamera;
				m_Obj.m_IsPerspectiveActive = true;
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		m_Obj.m_IsFirstFrame = true;
		break;
	}

	case WM_LBUTTONUP:
		m_Obj.m_IsFirstFrame = false;
		break;
#endif
	}
	
	return ::DefWindowProc(_hwnd, _msg, _wParam, _lParam);
}

/*
	 * @brief   Set the style for the main window and init it.
	 * @param   #unsigned int: First window width.
	 * @param   #unsigned int: First window height.
	 * @bug     No known bugs.
	 * @return  #HRESULT: status code.
*/
HRESULT InitWindow(LONG width, LONG height)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = nullptr;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "TutorialWindowClass";
	wcex.hIconSm = nullptr;
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hwnd = CreateWindow("TutorialWindowClass", "Graficas SIME1", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL,
		NULL);
	if (!g_hwnd)
	{
		return E_FAIL;
	}

	ShowWindow(g_hwnd, SW_SHOWNORMAL);

	return S_OK;
}

/**
	 * @brief   Init the UI.
	 * @bug     No know Bugs.
	 * @return  #HRESULT: Status code.
*/
HRESULT InitImgUI()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer back ends
	ImGui_ImplWin32_Init(g_hwnd);
#if defined(DX11)
	ImGui_ImplDX11_Init(GraphicsModule::GetManagerObj(g_hwnd).GetDevice().GetDXDevice(), GraphicsModule::GetManagerObj(g_hwnd).GetDeviceContext().GetDXDC());
#endif
	return S_OK;
}

#if defined(DX11)
void LoadMesh(const aiScene* _scene)
{
	m_vModels.push_back(Mesh());
	//std::vector<Material> vMaterials;

	int TotalNumOfVertex = _scene->mMeshes[0]->mNumVertices + _scene->mMeshes[1]->mNumVertices + _scene->mMeshes[2]->mNumVertices;
	std::vector <Vertex> VectorEnteringVertexArray;

	int VertexArrayPosition = 0;

	int NumMeshes = _scene->mNumMeshes;

	int NumMaterials = _scene->mNumMaterials;

	for (int i = 0; i < NumMeshes; ++i)
	{
		//vMaterials.push_back(Material());
		//vMaterials[i].SetID(_scene->mMeshes[i]->mMaterialIndex);
		int NumOfVertex = _scene->mMeshes[i]->mNumVertices;
		for (int j = 0; j < NumOfVertex; ++j)
		{
			VectorEnteringVertexArray.push_back(Vertex(_scene->mMeshes[i]->mVertices[j].x, _scene->mMeshes[i]->mVertices[j].y, _scene->mMeshes[i]->mVertices[j].z,
				_scene->mMeshes[i]->mNormals[j].x, _scene->mMeshes[i]->mNormals[j].y, _scene->mMeshes[i]->mNormals[j].z,
				_scene->mMeshes[i]->mTextureCoords[0][j].x, 1 - _scene->mMeshes[i]->mTextureCoords[0][j].y));
		}
	}

	unsigned int* VertexIndex = new unsigned int[TotalNumOfVertex];

	for (int i = 0; i < TotalNumOfVertex; ++i)
	{
		VertexIndex[i] = i;
	}

	m_vModels[m_ModelsNum].SetVertex(VectorEnteringVertexArray.data(), TotalNumOfVertex);
	m_vModels[m_ModelsNum].SetNumOfVertex(TotalNumOfVertex);
	m_vModels[m_ModelsNum].SetVertexIndex(VertexIndex, TotalNumOfVertex);
	m_vModels[m_ModelsNum].SetNumOfVertexIndex(TotalNumOfVertex);

	delete[] VertexIndex;
	VertexIndex = nullptr;

	m_vModels[m_ModelsNum].UpdateVertexAndIndexBuffer(m_Obj, g_hwnd);
	m_ModelsNum++;
}
#endif
void UIRender()
{
	// Start the Dear ImGui frame
#if defined(DX11)
	ImGui_ImplDX11_NewFrame();
#endif
#if defined(OGL)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif
	//ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#if defined(DX11)
	// example window
	if (ImGui::Begin("Model Configuration", nullptr))
	{
		static float dir[3]{};
		if (ImGui::DragFloat3("Direccion de la luz", dir, 0.001f, -1.0f, 1.0f))
		{
			#if defined(DX11)
			m_Obj.g_DirLightBufferDesc.Dir = XMFLOAT4(dir[0], dir[1], dir[2], 0.0f);
			#endif
		}

		if (ImGui::Button("Open File"))
		{
			std::string wideStringBuffer = "";
			wideStringBuffer.resize(MAX_PATH);
			
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = " Obj Files\0*.obj\0 Stl Files\0*.stl\0 3DS Files\0*.3ds\0 FBX Files\0*.fbx\0 All files\0*.*\0";
			ofn.lpstrFile = &wideStringBuffer[0];
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Select a model file";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn))
			{
				std::cout << "Filename to load: " << wideStringBuffer << std::endl;
			}

			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(wideStringBuffer, NULL);
			if (!scene)
			{
				std::cout << "Error importing the model" << std::endl;
			}
			else
			{
				std::cout << "Archivo importado correctamente" << std::endl;

				LoadMesh(scene);
			}
			m_Obj.m_ShowingTexture = true;
		}

		if (m_Obj.m_ShowingTexture)
		{
			float TextureWidth = 256;
			float TextureHeight = 256;
			ImTextureID TextureID = m_Obj.g_SimeTextureRV.GetDXSRV();
			ImVec2 Position = ImGui::GetCursorScreenPos();
			ImVec2 UVMin = ImVec2(0.0f, 0.0f);
			ImVec2 UVMax = ImVec2(1.0f, 1.0f);
			ImVec4 Tint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 Border = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			ImGui::Image(TextureID, ImVec2(TextureWidth, TextureHeight), UVMin, UVMax, Tint, Border);
		}
	}
#endif
#if defined(OGL)
	ImGui::Begin("Model Configuration");
#endif
	ImGui::End();

	// render UI
	ImGui::Render();
#if defined(DX11)
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

#if defined(OGL)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Update()
{
#if defined(DX11)
	for (int i = 0; i < m_vModels.size(); ++i)
	{
		m_vModels[i].Update(m_Obj, g_hwnd);
	}
	m_Obj.Update();
#endif
#if defined(OGL)
	m_Obj.UpdateOGL(m_OGLWindow);
#endif
}

void Render()
{
#if defined(DX11)
	for (int i = 0; i < m_vModels.size(); ++i)
	{
		m_vModels[i].Render(m_Obj, g_hwnd);
	}
	m_Obj.Render();
	UIRender();
	GraphicsModule::GetManagerObj(g_hwnd).GetSwapChain().CPresent(0, 0);
#endif
#if defined(OGL)
	m_Obj.Render();
	UIRender();
	glfwSwapBuffers(m_OGLWindow);
	glfwPollEvents();
#endif
}



int main()
{
#if defined(DX11)
	// create the window and console
	if (FAILED(InitWindow(1280, 720)))
	{
		DestroyWindow(g_hwnd);
		return 0;
	}
	
	// create Graphic API interface
	if (FAILED(m_Obj.InitDevice(g_hwnd)))
	{
		m_Obj.CleanupDevice();
		return 0;
	}
	//// create UI
	if (FAILED(InitImgUI()))
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		return 0;
	}
	
	// main loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
			Render();
		}
	}
	
	// clean resources
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	m_Obj.CleanupDevice();
	DestroyWindow(g_hwnd);
	return (int)msg.wParam;
#endif
#if defined(OGL)
	if (!glfwInit())
	{
		fprintf(stderr, "Error al inicializar GLFW\n");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Queremos OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para hacer feliz a MacOS ; Aunque no debería ser necesaria
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No queremos el viejo OpenGL 

	//Crear una ventana y su contexto OpenGL
	m_OGLWindow = glfwCreateWindow(1024, 768, "GRAFICAS SIME1", NULL, NULL);
	if (m_OGLWindow == NULL) {
		fprintf(stderr, "Falla al abrir una ventana GLFW. Si usted tiene una GPU Intel, está no es compatible con 3.3. Intente con la versión 2.1 de los tutoriales.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_OGLWindow); // Inicializar GLEW
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(m_OGLWindow, GLFW_STICKY_KEYS, GL_TRUE);

	if (FAILED(m_Obj.InitDevice(g_hwnd)))
	{
		m_Obj.CleanupDevice();
		return 0;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_OGLWindow, true);
	ImGui_ImplOpenGL3_Init();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	glViewport(0, 0, 1024, 768);
	glfwSetFramebufferSizeCallback(m_OGLWindow, framebuffer_size_callback);
	do 
	{
		Update();
		Render();
	} // Revisar que la tecla ESC fue presionada y cerrar la ventana
	while (glfwGetKey(m_OGLWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_OGLWindow) == 0);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
#endif
}
