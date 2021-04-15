#pragma comment(lib, "ComDlg32.lib")

#include "Windows.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include <vector>
#include "GraphicModule.h"
#include <stdio.h>
#include <stdlib.h>

#if defined(OGL)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <glfw3.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

#if defined(DX11)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "imgui_impl_dx11.h"
#endif
//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HWND g_hwnd;
GraphicsModule::test m_Obj;
#if defined(DX11) || defined(OGL)
std::vector<Model> m_vModels;
int ModelNum = 0;
static float dir[3]{};
#endif
#if defined(OGL)
bool OnW = false;
bool OnA = false;
bool OnS = false;
bool OnD = false;
bool OnQ = false;
bool OnE = false;

GLFWwindow* m_OGLWindow; // (En el código que viene aqui, está variable es global)
#endif
//-----------------------------------------------------------------------------------------

#if defined(OGL)
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Vector3 CameraPosition;

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		OnW = true;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		OnW = false;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		OnA = true;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		OnA = false;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		OnS = true;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		OnS = false;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		OnD = true;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		OnD = false;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		OnQ = true;
	}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		OnQ = false;
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		OnE = true;
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		OnE = false;
	}

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
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

	if (OnW)
	{
		CameraPosition.SetValues(0.0f, 0.1f, 0.0f);
		m_Obj.m_Camera->MoveCamera(CameraPosition);
		m_Obj.m_Camera->UpdateViewMatrix();
	}
	if (OnA)
	{
		CameraPosition.SetValues(-0.1f, 0.0f, 0.0f);
		m_Obj.m_Camera->MoveCamera(CameraPosition);
		m_Obj.m_Camera->UpdateViewMatrix();
	}
	if (OnS)
	{
		CameraPosition.SetValues(0.0f, -0.1f, 0.0f);
		m_Obj.m_Camera->MoveCamera(CameraPosition);
		m_Obj.m_Camera->UpdateViewMatrix();
	}
	if (OnD)
	{
		CameraPosition.SetValues(0.1f, 0.0f, 0.0f);
		m_Obj.m_Camera->MoveCamera(CameraPosition);
		m_Obj.m_Camera->UpdateViewMatrix();
	}
	if (OnQ)
	{
		CameraPosition.SetValues(0.0f, 0.0f, 0.1f);
		m_Obj.m_Camera->MoveCamera(CameraPosition);
		m_Obj.m_Camera->UpdateViewMatrix();
	}
	if (OnE)
	{
		CameraPosition.SetValues(0.0f, 0.0f, -0.1f);
		m_Obj.m_Camera->MoveCamera(CameraPosition);
		m_Obj.m_Camera->UpdateViewMatrix();
	}
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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	Vector3 cam_pos;
	// Handle UI inputs
	if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
		return 1;

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

	//case WM_LBUTTONDOWN:
	//{
	//	m_Obj.m_IsFirstFrame = true;
	//	break;
	//}
	//
	//case WM_LBUTTONUP:
	//	m_Obj.m_IsFirstFrame = false;
	//	break;
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

#if defined(OGL)
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(m_OGLWindow, true);
	ImGui_ImplOpenGL3_Init();
#endif

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer back ends
	ImGui_ImplWin32_Init(g_hwnd);
#if defined(DX11)
	ImGui_ImplDX11_Init(GraphicsModule::GetManagerObj(g_hwnd).GetDevice().GetDXDevice(), GraphicsModule::GetManagerObj(g_hwnd).GetDeviceContext().GetDXDC());
#endif
	return S_OK;
}
#if defined(DX11) || defined(OGL)
Mesh LoadMesh(aiMesh* _Mesh, const aiScene* _Scene, bool _LoadRGBA, bool _LoadBGRA, bool _LoadTriangles, bool _LoadPoints)
{
	std::vector<Vertex> MeshVertex;
	std::vector<unsigned int> MeshIndices;

	aiString Path;
	std::vector<std::string> vFilename;
	unsigned int NumVertex = _Mesh->mNumVertices;

	for (int i = 0; i < NumVertex; ++i)
	{
		Vertex vertex;

		vertex.SetPosition(_Mesh->mVertices[i].x, _Mesh->mVertices[i].y, _Mesh->mVertices[i].z);
		if (_Mesh->HasNormals())
		{
			vertex.SetNormal(_Mesh->mNormals[i].x, _Mesh->mNormals[i].y, _Mesh->mNormals[i].z);
		}
		if (_Mesh->HasTextureCoords(0))
		{
#if defined(DX11)
			vertex.SetTexture(_Mesh->mTextureCoords[0][i].x, _Mesh->mTextureCoords[0][i].y);
#endif
#if defined(OGL)
			vertex.SetTexture(_Mesh->mTextureCoords[0][i].x, 1 - _Mesh->mTextureCoords[0][i].y);
#endif
		}
		MeshVertex.push_back(vertex);
	}

	for (unsigned int i = 0; i < _Mesh->mNumFaces ; i++)
	{
		aiFace& Face = _Mesh->mFaces[i];
		for (unsigned int j = 0; j < Face.mNumIndices; j++)
		{
			MeshIndices.push_back(Face.mIndices[j]);
		}
	}

		aiString tmpMatName;
		const aiMaterial* pMaterial = _Scene->mMaterials[_Mesh->mMaterialIndex];
		pMaterial->Get(AI_MATKEY_NAME, tmpMatName);
		for (unsigned int j = 1; j < aiTextureType_UNKNOWN; j++)
		{
			if (pMaterial->GetTextureCount(aiTextureType(j)) > 0)
			{
				if (pMaterial->GetTexture(aiTextureType(j), 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					char drive[_MAX_DRIVE];
					char dir[_MAX_DIR];
					char fname[_MAX_FNAME];
					char ext[_MAX_EXT];

					std::string Filename = "";

					_splitpath_s(Path.data, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);

					Filename = fname;
					Filename += ext;
					vFilename.push_back(Filename);
				}
			}
		}

	return Mesh(MeshVertex, MeshIndices, vFilename, NumVertex, _LoadRGBA, _LoadBGRA, _LoadTriangles, _LoadPoints, _Mesh->mName.data, m_Obj.programID);
}

void LoadModel(const aiScene* _scene, std::string _ModelName, bool _LoadRGBA, bool _LoadBGRA, bool _LoadTriangles, bool _LoadPoints)
{
	m_vModels.push_back(Model());
	m_vModels[ModelNum].SetName(_ModelName);

	int NumMeshes = _scene->mNumMeshes;
	m_vModels[ModelNum].SetMeshNum(NumMeshes);
	
	for (int i = 0; i < NumMeshes; ++i)
	{
		aiMesh* ActualMesh = _scene->mMeshes[i];

		m_vModels[ModelNum].AddMesh(LoadMesh(ActualMesh, _scene, _LoadRGBA, _LoadBGRA, _LoadTriangles, _LoadPoints));
	}

	m_vModels[ModelNum].SetUpModel(g_hwnd);
	ModelNum++;
}

void OpenMeshMenu(bool _LoadRGBA, bool _LoadBGRA, bool _LoadTriangles, bool _LoadPoints)
{
	std::string wideStringBuffer = "";
	wideStringBuffer.resize(MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "All files\0*.*\0 Obj \0*.obj\0 Stl Files\0*.stl\0 3DS Files\0*.3ds\0 FBX Files\0*.fbx\0";
	ofn.lpstrFile = &wideStringBuffer[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a model file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		std::cout << "Filename to load: " << wideStringBuffer << std::endl;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(wideStringBuffer, aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		std::cout << "Error importing the model" << std::endl;
	}
	else
	{

		//Usar string
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		std::string Filename = "";

		_splitpath_s(wideStringBuffer.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);

		Filename = fname;
		Filename += ext;

		for (unsigned int i = 0; i < ModelNum; ++i)
		{
			if (Filename == m_vModels[i].GetName())
			{
				std::cout << "Model Already Imported!" << std::endl;
				return;
			}
		}

		std::cout << "Archivo importado correctamente" << std::endl;
		LoadModel(scene, Filename, _LoadRGBA, _LoadBGRA, _LoadTriangles, _LoadPoints);
	}
}

void ShowMenuOptions()
{
	if (ImGui::BeginMenu("Open File"))
	{
		if (ImGui::BeginMenu("...as Triangles"))
		{
			if (ImGui::MenuItem("...as RGBA"))
			{
				OpenMeshMenu(true, false, true, false);
			}
			if (ImGui::MenuItem("...as BGRA"))
			{
				OpenMeshMenu(false, true, true, false);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("...as Points"))
		{
			if (ImGui::MenuItem("...as RGBA"))
			{
				OpenMeshMenu(true, false, false, true);
			}
			if (ImGui::MenuItem("...as BGRA"))
			{
				OpenMeshMenu(false, true, false, true);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}

void ShowMeshesMenu(const unsigned int _i)
{
	for (unsigned int j = 0; j < m_vModels[_i].GetMeshes().size(); ++j)
	{
		if (ImGui::CollapsingHeader(m_vModels[_i].GetMeshes()[j].GetName().c_str()))
		{
			if (ImGui::CollapsingHeader("Textures"))
			{
				float TextureWidth = 256;
				float TextureHeight = 256;
				#if defined(OGL)
				ImTextureID TextureID = (void*)m_vModels[_i].GetMeshes()[j].GetTexId();
				#endif
				#if defined(DX11)
				ImTextureID TextureID = m_vModels[_i].GetMeshes()[j].GetMaterial()->GetSRVTexture()->GetDXSRV();//m_Obj.g_SimeTextureRV.GetDXSRV();
				#endif
				ImVec2 Position = ImGui::GetCursorScreenPos();
				ImVec2 UVMin = ImVec2(0.0f, 0.0f);
				ImVec2 UVMax = ImVec2(1.0f, 1.0f);
				ImVec4 Tint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 Border = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
				ImGui::Image(TextureID, ImVec2(TextureWidth, TextureHeight), UVMin, UVMax, Tint, Border);
			}
		}
	}
}

void UpdateTransformMenu(const unsigned int _i)
{
	if (ImGui::DragFloat3("Position", m_vModels[_i].GetGuiPos(), 0.01f))
	{
		m_vModels[_i].UpdateTranslationMatrix(m_vModels[_i].GetGuiPos()[0] * 10.0f, m_vModels[_i].GetGuiPos()[1] * 10.0f, m_vModels[_i].GetGuiPos()[2] * 10.0f);
	}
	if (ImGui::DragFloat3("Rotation", m_vModels[_i].GetGuiRot(), 1.0f))
	{
		m_vModels[_i].UpdateRotationMatrix(m_vModels[_i].GetGuiRot()[0], m_vModels[_i].GetGuiRot()[1], m_vModels[_i].GetGuiRot()[2]);
	}
	if (ImGui::DragFloat3("Scale", m_vModels[_i].GetGuiScale(), 0.001f))
	{
		m_vModels[_i].UpdateScaleMatrix(m_vModels[_i].GetGuiScale()[0], m_vModels[_i].GetGuiScale()[1], m_vModels[_i].GetGuiScale()[2]);
	}
}

void UIRender()
{
	// Start the Dear ImGui frame
#if defined(DX11)
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
#endif
#if defined(OGL)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif
	ImGui::NewFrame();

	ImGui::Begin("Model Configuration", NULL, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ShowMenuOptions();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	for (unsigned int i = 0; i < m_vModels.size(); ++i)
	{
		if (ImGui::CollapsingHeader(m_vModels[i].GetName().c_str()))
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				UpdateTransformMenu(i);
			}
			if (ImGui::CollapsingHeader("Meshes"))
			{
				ShowMeshesMenu(i);
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Enviroment", NULL, 0);
	if (ImGui::CollapsingHeader("Light"))
	{
		if (ImGui::DragFloat3("Direction", dir, 0.001f, -1.0f, 1.0f))
		{
		#if defined(DX11)
			m_Obj.g_DirLightBufferDesc.Dir = XMFLOAT4(dir[0], dir[1], dir[2], 0.0f);
		#endif
		#if defined(OGL)
			m_Obj.g_DirLightBufferDesc.Dir[0] = dir[0];
			m_Obj.g_DirLightBufferDesc.Dir[1] = dir[1];
			m_Obj.g_DirLightBufferDesc.Dir[2] = dir[2];
			m_Obj.g_DirLightBufferDesc.Dir[3] = 0.0f;
		#endif
		}
	}
	ImGui::End();
	//ImGui::ShowDemoWindow();
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
		//m_vModels[i].UpdateModelMatrix();
		m_vModels[i].Update(m_Obj, g_hwnd);
	}
	m_Obj.Update();
#endif
#if defined(OGL)
	for (int i = 0; i < m_vModels.size(); ++i)
	{
		m_vModels[i].Update(m_Obj, g_hwnd);
	}
	m_Obj.UpdateOGL(m_OGLWindow);
#endif
}

void Render()
{
#if defined(DX11)
	for (int i = 0; i < m_vModels.size(); ++i)
	{
		m_vModels[i].Render(g_hwnd);
	}
	m_Obj.Render();
	UIRender();
	GraphicsModule::GetManagerObj(g_hwnd).GetSwapChain().CPresent(0, 0);
#endif
#if defined(OGL)
	for (int i = 0; i < m_vModels.size(); ++i)
	{
		m_vModels[i].Render(g_hwnd);
	}
	m_Obj.Render();
	UIRender();
	glfwSwapBuffers(m_OGLWindow);
	glfwPollEvents();
#endif
}


#endif
int main()
{
#if !defined(DX11) && !defined(OGL)
	// create the window and console
	if (FAILED(InitWindow(1024, 768)))
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
		//	Update();
		//	Render();
		}
	}

	// clean resources
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//for (int i = 0; i < m_vModels.size(); ++i)
	//{
	//	m_vModels[i].CleanUpDXResources();
	//}
	m_Obj.CleanupDevice();
	DestroyWindow(g_hwnd);
	return (int)msg.wParam;
#endif
#if defined(DX11)
	// create the window and console
	if (FAILED(InitWindow(1024, 768)))
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
	for (int i = 0; i < m_vModels.size(); ++i)
	{
		m_vModels[i].CleanUpDXResources();
	}
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

	if (FAILED(InitImgUI()))
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return 0;
	}

	glViewport(0, 0, 1024, 768);
	glfwSetFramebufferSizeCallback(m_OGLWindow, framebuffer_size_callback);

	glfwSetKeyCallback(m_OGLWindow, KeyCallback);

	FreeImage_Initialise();
	std::cout << "FreeImage" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";
	
	do 
	{
		Update();
		Render();
	} 
	while (glfwGetKey(m_OGLWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(m_OGLWindow) == 0);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	FreeImage_DeInitialise();
	glfwTerminate();
#endif
}
