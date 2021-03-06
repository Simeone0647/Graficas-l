#pragma comment(lib, "ComDlg32.lib")

#include "Windows.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include <vector>
#include "GraphicModule.h"
#include <stdio.h>
#include <stdlib.h>
#include "Windowsx.h"
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

using namespace RM;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HWND g_hwnd;
GraphicsModule::test m_Obj;
#if defined(DX11) || defined(OGL)

std::vector<Tech> m_vTechs;
std::vector<Effect> m_vEffects;

float StartTime = GetTickCount64();
//------------------------- LIGHTS -------------------------------------------------------
static float DirectionLightDir[3] { 0.0f, 0.0f, 0.0f };

static float PointLightPos[3] { 0.0f, 0.0f, 0.0f };
static float PointLightAttenuation = 0.0f;

static float SpotLightDir[3]{ 0.0f, 0.0f, 0.0f} ;
static float SpotLightPos[3]{ 0.0f, 0.0f, 0.0f} ;
static float SpotLightAttenuation = 0.0f;
static float InnerRadius = 0.0f;
static float OuterRadius = 0.0f;

static float kAmbient = 0.0f;
static float kSpecular = 0.0f;
static float kDiffuse = 0.0f;
static float Shininess = 0.1f;
static float ExpossureGui = 0.1f;

static float AORadius = 0.0f;
static float AOScale = 0.0f;
static float AOBias = 0.0f;
static float AOIntensity = 0.0f;
static int AOIterations = 1;
static int SelectedAnimation = -1;
static ImVec4 DirLightColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
static ImVec4 PointLightColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
static ImVec4 SpotLightColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
static ImVec4 AmbientLightColor = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

ImGuiColorEditFlags Flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueBar;
//------------------------- MOUSE ----------------------------------------------------
static double MouseRelativePosition[2]{};
static double PreviousMouseRelativePosition[2]{};
static bool OverImGuiWindow = false;
static bool NormalMap = false;
static bool ShowSkeleton = false;

bool g_NewEffect;
bool g_NewPass;
int g_LastFeature = 0;

bool g_ForwardRender = false;
bool g_DeferredRender = false;

static bool LightModelSelected[2] = { false, false };

bool LeftClick = g_NewEffect = false;

const char* LightingModels[] = { "None", "Phong", "Blinn-Phong" };
static int CurrentLightingModels = 0;
const char* LightingModelsLabel = LightingModels[CurrentLightingModels];

const char* LightingPlaces[] = { "Vertex Shader", "Pixel Shader"};
static int CurrentLightingPlaces = 0;
const char* LightingPlacesLabel = LightingPlaces[CurrentLightingPlaces];

const char* Normals[] = { "Vertex Normal", "Normal Map" };
static int CurrentNormal = 0;
const char* NormalLabel = Normals[CurrentNormal];

const char* RenderTechs[] = { "Forward Rendering", "Deferred Rendering" };
static int CurrentTech = 0;
const char* TechLabel = RenderTechs[CurrentTech];

const char* ColorCorrections[] = { "Basic", "Reinhard", "Burgess Dowson" };
static int CurrentCorrection = 0;
const char* CorrectionLabel = ColorCorrections[CurrentCorrection];

std::string NewEffectName = "";
std::string NewPassName = "";

SetTechDesc TDesc;

#endif
#if defined(OGL)
bool OnW = false;
bool OnA = false;
bool OnS = false;
bool OnD = false;
bool OnQ = false;
bool OnE = false;
GLFWwindow* m_OGLWindow; // (En el c�digo que viene aqui, est� variable es global)
#endif
//-----------------------------------------------------------------------------------------

float GetCurrentTimeMillis()
{
	return GetTickCount64();
}

float GetRunningTime()
{
	float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)StartTime) / 1000.0f;
	return RunningTime;
}

#if defined(OGL)
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (LeftClick)
	{
		if (PreviousMouseRelativePosition[0] < xpos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(-0.01, 0.0f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (PreviousMouseRelativePosition[0] > xpos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(0.01, 0.0f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (PreviousMouseRelativePosition[1] < ypos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(0.0f, -0.01f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (PreviousMouseRelativePosition[1] > ypos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(0.0f, 0.01f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (!OverImGuiWindow)
		{
			LeftClick = true;
			glfwGetCursorPos(window, &PreviousMouseRelativePosition[0], &PreviousMouseRelativePosition[1]);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		LeftClick = false;
	}
}
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
			RM::GetRenderManager().m_Camera = &RM::GetRenderManager().m_OrtographicCamera;
			m_Obj.m_IsPerspectiveActive = false;
		}
		else
		{
			RM::GetRenderManager().m_Camera = &RM::GetRenderManager().m_PerspectiveCamera;
			m_Obj.m_IsPerspectiveActive = true;
		}
	}

	if (OnW)
	{
		CameraPosition.m_X = 0.0f;
		CameraPosition.m_Y = 0.1f;
		CameraPosition.m_Z = 0.0f;
		RM::GetRenderManager().m_Camera->MoveCamera(CameraPosition);
		RM::GetRenderManager().m_Camera->UpdateViewMatrix();
	}
	if (OnA)
	{
		CameraPosition.m_X = -0.1f;
		CameraPosition.m_Y = 0.0f;
		CameraPosition.m_Z = 0.0f;
		RM::GetRenderManager().m_Camera->MoveCamera(CameraPosition);
		RM::GetRenderManager().m_Camera->UpdateViewMatrix();
	}
	if (OnS)
	{
		CameraPosition.m_X = 0.0f;
		CameraPosition.m_Y = -0.1f;
		CameraPosition.m_Z = 0.0f;
		RM::GetRenderManager().m_Camera->MoveCamera(CameraPosition);
		RM::GetRenderManager().m_Camera->UpdateViewMatrix();
	}
	if (OnD)
	{
		CameraPosition.m_X = 0.1f;
		CameraPosition.m_Y = 0.0f;
		CameraPosition.m_Z = 0.0f;
		RM::GetRenderManager().m_Camera->MoveCamera(CameraPosition);
		RM::GetRenderManager().m_Camera->UpdateViewMatrix();
	}
	if (OnQ)
	{
		CameraPosition.m_X = 0.0f;
		CameraPosition.m_Y = 0.0f;
		CameraPosition.m_Z = 0.1f;
		RM::GetRenderManager().m_Camera->MoveCamera(CameraPosition);
		RM::GetRenderManager().m_Camera->UpdateViewMatrix();
	}
	if (OnE)
	{
		CameraPosition.m_X = 0.0f;
		CameraPosition.m_Y = 0.0f;
		CameraPosition.m_Z = -0.1f;
		RM::GetRenderManager().m_Camera->MoveCamera(CameraPosition);
		RM::GetRenderManager().m_Camera->UpdateViewMatrix();
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
	Vector3 CamPos;
	// Handle UI inputs
	if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wParam, _lParam))
		return 1;

	// Handle Window inputs
	switch (_msg)
	{

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
	case WM_SIZE:
	{
		if (GraphicsModule::GetManagerObj(_hwnd).GetSwapChain().GetDXSC())
		{
			//GraphicsModule::ClearDepthStencilViewStruct ClearDSVStruct;
			//ClearDSVStruct.pDepthStencilView = m_Obj.g_SimeDepthStencilView.GetDSV();
			//ClearDSVStruct.ClearFlags = GraphicsModule::SIME_CLEAR_DEPTH;
			//ClearDSVStruct.Depth = 1.0f;
			//ClearDSVStruct.Stencil = 0;
			//GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CClearDepthStencilView(ClearDSVStruct);
			HRESULT hr;

			//m_Obj.g_SimeRenderTargetView.GetRTV()->Release();

			hr = GraphicsModule::GetManagerObj(_hwnd).GetSwapChain().GetDXSC()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

			GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().COMSetRenderTargets(0, 0, 0);


			// Release all outstanding references to the swap chain's buffers.



			// Preserve the existing buffer count and format.
			// Automatically choose the width and height to match the client rect for HWNDs.


			// Perform error handling here!

			// Get buffer and create a render-target-view.
			ID3D11Texture2D* pBuffer;
			hr = GraphicsModule::GetManagerObj(_hwnd).GetSwapChain().CGetBuffer(0, __uuidof(ID3D11Texture2D),(void**)&pBuffer);
			// Perform error handling here!

			//hr = GraphicsModule::GetManagerObj(_hwnd).GetDevice().CCreateRenderTargetView(pBuffer, NULL, m_Obj.g_SimeRenderTargetView.GetRTVAdress());

			// Perform error handling here!
			pBuffer->Release();

			//GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().COMSetRenderTargets(1, m_Obj.g_SimeRenderTargetView.GetRTVAdress(), NULL);

			// Set up the viewport.
			RECT rc;
			UINT width;
			UINT height;
			GetClientRect(_hwnd, &rc);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;

			GraphicsModule::InitViewportStruct InitVPStruct;
			InitVPStruct.Width = width;
			InitVPStruct.Height = height;
			InitVPStruct.MinDepth = 0.0f;
			InitVPStruct.MaxDepth = 1.0f;
			InitVPStruct.TopLeftX = 0;
			InitVPStruct.TopLeftY = 0;

			RM::UpdateProjectionMatrixStruct PMStruct;
			PMStruct.AngleY = SIME_PIDIV4;
			PMStruct.Ratio = width / (FLOAT)height;
			PMStruct.NearPlane = 0.01f;
			PMStruct.FarPlane = 1000.0f;
			PMStruct.Width = width;
			PMStruct.Height = height;

			RM::GetRenderManager().m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(PMStruct);
			RM::GetRenderManager().m_OrtographicCamera.UpdateOrtographicProjectionMatrix(PMStruct);

			m_Obj.g_SimeViewport.InitViewport(InitVPStruct);
			GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CRSSetViewports(1, m_Obj.g_SimeViewport.GetViewportAddress());

			GraphicsModule::UpdateSubResourceStruct UpdateSBStruct;
			UpdateSBStruct.DstSubresource = 0;
			UpdateSBStruct.pDstBox = NULL;
			UpdateSBStruct.SrcRowPitch = 0;
			UpdateSBStruct.SrcDepthPitch = 0;
			
			GraphicsModule::CBChangeOnResize cbChangesOnResize;
			cbChangesOnResize.mProjection = XMMatrixTranspose(RM::GetRenderManager().m_Camera->m_ProjectionMatrix);
			UpdateSBStruct.pDstResource = m_Obj.g_SimeCBChangeOnResize.GetCBChangesOnResize();
			UpdateSBStruct.pSrcData = &cbChangesOnResize;
			GraphicsModule::GetManagerObj(_hwnd).GetDeviceContext().CUpdateSubresource(UpdateSBStruct);
		}
	break;
	}

	case WM_KEYDOWN:
	{
		if (LOWORD(_wParam) == 'W')
		{
			CamPos.m_X = 0.0f;
			CamPos.m_Y = 0.1f;
			CamPos.m_Z = 0.0f;
			//cam_pos.SetValues(0.0f, 0.1f, 0.0f);
			RM::GetRenderManager().m_Camera->MoveCamera(CamPos);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'A')
		{
			//cam_pos.SetValues(-0.1f, 0.0f, 0.0f);
			CamPos.m_X = -0.1f;
			CamPos.m_Y = 0.0f;
			CamPos.m_Z = 0.0f;

			RM::GetRenderManager().m_Camera->MoveCamera(CamPos);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'S')
		{
			//cam_pos.SetValues(0.0f, -0.1f, 0.0f);
			CamPos.m_X = 0.0f;
			CamPos.m_Y = -0.1f;
			CamPos.m_Z = 0.0f;
			RM::GetRenderManager().m_Camera->MoveCamera(CamPos);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'D')
		{
			//cam_pos.SetValues(0.1f, 0.0f, 0.0f);
			CamPos.m_X = 0.1f;
			CamPos.m_Y = 0.0f;
			CamPos.m_Z = 0.0f;
			RM::GetRenderManager().m_Camera->MoveCamera(CamPos);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'Q')
		{
			//cam_pos.SetValues(0.0f, 0.0f, 0.1f);
			CamPos.m_X = 0.0f;
			CamPos.m_Y = 0.0f;
			CamPos.m_Z = 0.1f;
			RM::GetRenderManager().m_Camera->MoveCamera(CamPos);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (LOWORD(_wParam) == 'E')
		{
			//cam_pos.SetValues(0.0f, 0.0f, -0.1f);
			CamPos.m_X = 0.0f;
			CamPos.m_Y = 0.0f;
			CamPos.m_Z = -0.1f;
			RM::GetRenderManager().m_Camera->MoveCamera(CamPos);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (_wParam == VK_TAB)
		{
			if (m_Obj.m_IsPerspectiveActive)
			{
				RM::GetRenderManager().m_Camera = &RM::GetRenderManager().m_OrtographicCamera;
				m_Obj.m_IsPerspectiveActive = false;
			}
			else
			{
				RM::GetRenderManager().m_Camera = &RM::GetRenderManager().m_PerspectiveCamera;
				m_Obj.m_IsPerspectiveActive = true;
			}
		}
		break;
	}

	case WM_LBUTTONDOWN:
	{
		if (!OverImGuiWindow)
		{
			LeftClick = true;
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		LeftClick = false;
		break;
	}

	}
	if (LeftClick)
	{ 
		float xPos = GET_X_LPARAM(_lParam);
		float yPos = GET_Y_LPARAM(_lParam);
		//std::cout << "Pos en window callback:" + to_string(xPos) + ", " + to_string(yPos)<< std::endl;
		if (PreviousMouseRelativePosition[0] < xPos)
		{
			//std::cout << "Esta avanzando" << std::endl;
			RM::GetRenderManager().m_Camera->RotateCamera(-0.01, 0.0f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (PreviousMouseRelativePosition[0] > xPos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(0.01, 0.0f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (PreviousMouseRelativePosition[1] < yPos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(0.0f, -0.01f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
		if (PreviousMouseRelativePosition[1] > yPos)
		{
			RM::GetRenderManager().m_Camera->RotateCamera(0.0f, 0.01f, 0.0f);
			RM::GetRenderManager().m_Camera->UpdateViewMatrix();
		}
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
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer back ends
	ImGui_ImplWin32_Init(g_hwnd);
	#if defined(DX11)
	ImGui_ImplDX11_Init(GraphicsModule::GetManagerObj(g_hwnd).GetDevice().GetDXDevice(), GraphicsModule::GetManagerObj(g_hwnd).GetDeviceContext().GetDXDC());
	#endif
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 7.0f;
	return S_OK;
}

#if defined(DX11) || defined(OGL)
void OpenMeshMenu(const int _Flags[], const int _PassID)
{
	string wideStringBuffer = "";
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
		cout << "Filename to load: " << wideStringBuffer << endl;
		
		GetRenderManager().LoadModel(wideStringBuffer);
	}
}

void ShowMenuOptions()
{
	if (ImGui::BeginMenu("Add Technique"))
	{
		//if (ImGui::BeginMenu("...as Triangles"))
		//{
		//	if (ImGui::MenuItem("...as RGBA"))
		//	{
		//		int Flags[2] = { 0, 2 };
		//		OpenMeshMenu(Flags);
		//	}
		//	if (ImGui::MenuItem("...as BGRA"))
		//	{
		//		int Flags[2] = { 0, 3 };
		//		OpenMeshMenu(Flags);
		//	}
		//	ImGui::EndMenu();
		//}
		//
		//if (ImGui::BeginMenu("...as Points"))
		//{
		//	if (ImGui::MenuItem("...as RGBA"))
		//	{
		//		int Flags[2] = { 1, 2 };
		//		OpenMeshMenu(Flags);
		//	}
		//	if (ImGui::MenuItem("...as BGRA"))
		//	{
		//		int Flags[2] = { 1, 3 };
		//		OpenMeshMenu(Flags);
		//	}
		//	ImGui::EndMenu();
		//}

		ImGui::EndMenu();
	}
}

void ShowMeshesMenu(const unsigned int _i)
{
	for (unsigned int j = 0; j < RM::GetRenderManager().m_vModels[_i]->GetMeshes().size(); ++j)
	{
		if (ImGui::CollapsingHeader(RM::GetRenderManager().m_vModels[_i]->GetMeshes()[j].GetName().c_str()))
		{
			if (ImGui::TreeNode("Textures"))
			{
				float TextureWidth = 100.0f;
				float TextureHeight = 100.0f;

				ImVec2 Position = ImGui::GetCursorScreenPos();
				ImVec2 UVMin = ImVec2(0.0f, 0.0f);
				ImVec2 UVMax = ImVec2(1.0f, 1.0f);
				ImVec4 Tint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 Border = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
				
				for (unsigned int i = 0; i < RM::GetRenderManager().m_vModels[_i]->GetMeshes()[j].GetMaterial()->GetTexNum(); ++i)
				{
					#if defined(DX11)
					ImTextureID TextureID = RM::GetRenderManager().m_vModels[_i]->GetMeshes()[j].GetMaterial()->GetSRVTexture(i)->GetDXSRV();
					#endif
					#if defined(OGL)
					ImTextureID TextureID = (void*)RM::GetRenderManager().m_vModels[_i]->GetMeshes()[j].GetTexID(i);
					#endif
					ImGui::Image(TextureID, ImVec2(TextureWidth, TextureHeight), UVMin, UVMax, Tint, Border);

				}
				ImGui::TreePop();
			}
		}
	}
}

void UpdateTransformMenu(const unsigned int _i)
{
	if (ImGui::DragFloat3("Position", RM::GetRenderManager().m_vModels[_i]->m_GuiPos, 0.01f))
	{
		RM::GetRenderManager().m_vModels[_i]->UpdateTranslationMatrix(RM::GetRenderManager().m_vModels[_i]->GetGuiPos()[0] * 10.0f,
																	  RM::GetRenderManager().m_vModels[_i]->GetGuiPos()[1] * 10.0f,
																	  RM::GetRenderManager().m_vModels[_i]->GetGuiPos()[2] * 10.0f);
	}
	if (ImGui::DragFloat3("Rotation", RM::GetRenderManager().m_vModels[_i]->GetGuiRot(), 1.0f))
	{
		RM::GetRenderManager().m_vModels[_i]->UpdateRotationMatrix(RM::GetRenderManager().m_vModels[_i]->GetGuiRot()[0],
																   RM::GetRenderManager().m_vModels[_i]->GetGuiRot()[1],
																   RM::GetRenderManager().m_vModels[_i]->GetGuiRot()[2]);
	}
	if (ImGui::DragFloat3("Scale", RM::GetRenderManager().m_vModels[_i]->GetGuiScale(), 0.001f))
	{
		RM::GetRenderManager().m_vModels[_i]->UpdateScaleMatrix(RM::GetRenderManager().m_vModels[_i]->GetGuiScale()[0],
			RM::GetRenderManager().m_vModels[_i]->GetGuiScale()[1],
			RM::GetRenderManager().m_vModels[_i]->GetGuiScale()[2]);
	}
}

//void DisplayModelsMenu(const unsigned int _i, const unsigned int _j, const unsigned int _k)
//{
//	if (ImGui::TreeNode(m_vModels[_k].GetName().c_str()))
//	{
//		ImGui::SameLine();
//		ImGui::TextDisabled("(?)");
//		if (ImGui::IsItemHovered())
//		{
//			ImGui::BeginTooltip();
//			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 8);
//			std::string MeshNumText = "Meshes:" + to_string(m_vModels[_k].GetMeshNum());
//			ImGui::TextUnformatted(&MeshNumText[0]);
//			ImGui::PopTextWrapPos();
//			ImGui::EndTooltip();
//		}
//		ImGui::Separator();
//		if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
//		{
//			if (ImGui::BeginTabItem("Transform"))
//			{
//				UpdateTransformMenu(_i, _j, _k);
//				ImGui::EndTabItem();
//			}
//			if (ImGui::BeginTabItem("Meshes"))
//			{
//				ShowMeshesMenu(_k);
//				ImGui::EndTabItem();
//			}
//			ImGui::EndTabBar();
//		}
//		ImGui::TreePop();
//	}
//
//	else
//	{
//		ImGui::SameLine();
//		ImGui::TextDisabled("(?)");
//		if (ImGui::IsItemHovered())
//		{
//			ImGui::BeginTooltip();
//			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20);
//			std::string MeshNumText = "Meshes:" + to_string(m_vModels[_k].GetMeshNum());
//			ImGui::TextUnformatted(&MeshNumText[0]);
//			ImGui::PopTextWrapPos();
//			ImGui::EndTooltip();
//		}
//	}
//
//	ImGui::Separator();
//}

void AnimationMenu(const unsigned int _i)
{
	if (ImGui::Checkbox("Show Skeleton", &ShowSkeleton))
	{
		GetRenderManager().m_vModels[_i]->ShowSkeleton(ShowSkeleton);
	}

	for (auto i = 0; i < GetRenderManager().m_vModels[_i]->GetAnimationNum(); ++i)
	{
		if (ImGui::Selectable(GetRenderManager().m_vModels[_i]->GetAnimationName(i).c_str(), SelectedAnimation == i))
		{
			GetRenderManager().m_vModels[_i]->SetAnimation(i - 1);
			SelectedAnimation = i;
		}
	}
}

void AccessModels()
{
	for (unsigned int i = 0; i < RM::GetRenderManager().m_vModels.size(); ++i)
	{
	
		if (ImGui::TreeNode(RM::GetRenderManager().m_vModels[i]->GetName().c_str()))
		{
			ImGui::SameLine();
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 8);
				std::string MeshNumText = "Meshes:" + to_string(RM::GetRenderManager().m_vModels[i]->GetMeshNum());
				ImGui::TextUnformatted(&MeshNumText[0]);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			ImGui::Separator();
			if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Transform"))
				{
					UpdateTransformMenu(i);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Meshes"))
				{
					ShowMeshesMenu(i);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Animations"))
				{
					AnimationMenu(i);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::TreePop();
		}
	
		else
		{
			ImGui::SameLine();
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 20);
				std::string MeshNumText = "Meshes:" + to_string(RM::GetRenderManager().m_vModels[i]->GetMeshNum());
				ImGui::TextUnformatted(&MeshNumText[0]);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		}
	
		ImGui::Separator();
	}
}

void PassesMenu(const int _i)
{
	//if (ImGui::Button("Add Pass"))
	//{
	//	g_NewPass = true;
	//}
	//
	//if (g_NewPass)
	//{
	//	ImGui::Begin("New Pass", NULL, 0);
	//
	//	size_t Size = sizeof(NewPassName);
	//
	//	ImGui::InputText("Pass Name", &NewPassName[0], Size);
	//
	//	ImGui::Text("Load Models");
	//	
	//	for (unsigned int i = 0; i < ModelNum; ++i)
	//	{
	//		ImGui::Selectable(m_vModels[i].GetName().c_str(), &m_vModels[i].m_ImGuiSelected);
	//	}
	//	
	//	if (ImGui::Button("Load"))
	//	{
	//		int Flags[2] = { 0, 3 };
	//		OpenMeshMenu(Flags);
	//	}
	//
	//	if (ImGui::Button("Ok"))
	//	{
	//		for (unsigned int i = 0; i < ModelNum; ++i)
	//		{
	//			if (!m_vModels[i].m_ImGuiSelected)
	//			{
	//				m_vModels.erase(m_vModels.begin() + i);
	//			}
	//		}
	//
	//		m_vEffects[_i].SetModels(&m_vModels);
	//	}
	//
	//	if (ImGui::Button("Cancel"))
	//	{
	//		g_NewPass = false;
	//	}
	//	ImGui::End();
	//}
	for (unsigned int j = 0; j < m_vEffects[_i].GetPassNum(); ++j)
	{
		if (ImGui::TreeNode(m_vEffects[_i].GetPassName(j).c_str()))
		{
			if (ImGui::Button("Load Model"))
			{
				int Flags[2] = { 0, 3 };
				OpenMeshMenu(Flags, m_vEffects[_i].GetPassID(j));

			}

			if (ImGui::TreeNode("Models"))
			{
				for (unsigned int i = 0; i < RM::GetRenderManager().m_vModels.size(); ++i)
				{
					if (RM::GetRenderManager().m_vModels[i]->GetPassID(m_vEffects[_i].GetPassID(j)))
					{
						ImGui::Text(RM::GetRenderManager().m_vModels[i]->GetName().c_str());
					}
				}

				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}


void ForwardMenu(const int _i)
{
	if ((m_vEffects[_i].GetActiveTech() > 29 && m_vEffects[_i].GetActiveTech() < 33) || (m_vEffects[_i].GetActiveTech() > 40 && m_vEffects[_i].GetActiveTech() < 44)
		|| (m_vEffects[_i].GetActiveTech() > 51 && m_vEffects[_i].GetActiveTech() < 55))
	{
		LightingPlacesLabel = LightingPlaces[0];
	}
	else
	{
		LightingPlacesLabel = LightingPlaces[1];
	}

	if (ImGui::BeginCombo("Light Calculation", LightingPlacesLabel, NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(LightingPlaces); n++)
		{
			const bool IsSelected = (m_vEffects[_i].GetActiveTech() == n);
			if (ImGui::Selectable(LightingPlaces[n], IsSelected))
			{
				CurrentLightingPlaces = n;
				m_vEffects[_i].DeactivateTech();

				TDesc.Features = 0;
				TDesc.LightingPlace = CurrentLightingPlaces;
				TDesc.LightingModel = CurrentLightingModels;

				m_vEffects[_i].ActivateTech(TDesc);
			}

			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	LightingModelsLabel = LightingModels[CurrentLightingModels];
	if (ImGui::BeginCombo("Lighting Models", LightingModelsLabel, NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(LightingModels); n++)
		{
			const bool IsSelected = (m_vEffects[_i].GetActiveTech() == n);
			if (ImGui::Selectable(LightingModels[n], IsSelected))
			{
				CurrentLightingModels = n;
				m_vEffects[_i].DeactivateTech();
				m_vEffects[_i].m_ImGuiNormalMap = false;
				m_vEffects[_i].m_ImGuiSpecularMap = false;

				TDesc.Features = 0;
				TDesc.LightingPlace = CurrentLightingPlaces;
				TDesc.LightingModel = CurrentLightingModels;

				m_vEffects[_i].ActivateTech(TDesc);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	CorrectionLabel = ColorCorrections[CurrentCorrection];
	if (ImGui::BeginCombo("Color Correction", CorrectionLabel, NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(ColorCorrections); n++)
		{
			const bool IsSelected = (CurrentCorrection == n);
			if (ImGui::Selectable(ColorCorrections[n], IsSelected))
			{
				CurrentCorrection = n;
				CorrectionLabel = ColorCorrections[n];

				TDesc.LightingModel = CurrentLightingModels;
				TDesc.NormalCalc = CurrentNormal;
				TDesc.LightingPlace = CurrentLightingPlaces;
				TDesc.ColorCorr = CurrentCorrection;

				m_vEffects[_i].DeactivateTech();
				m_vEffects[_i].ActivateTech(TDesc);
			}

			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (m_vEffects[_i].GetActiveTech() > kVertexBlinnPhong)
	{
		if (ImGui::Checkbox("Normal Map", &m_vEffects[_i].m_ImGuiNormalMap))
		{
			if (m_vEffects[_i].m_ImGuiNormalMap)
			{
				if (m_vEffects[_i].GetActiveTech() == kPixel)
				{
					m_vEffects[_i].DeactivateTech();
					g_LastFeature = kPixelNM;

					TDesc.Features = kPixelNM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelNM)
				{
					m_vEffects[_i].DeactivateTech();
					g_LastFeature = kPixel;

					TDesc.Features = kPixel;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelPhong)
				{
					m_vEffects[_i].DeactivateTech();
					g_LastFeature = kPixelPhongNM;

					TDesc.Features = kPixelPhongNM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelPhongNM)
				{
					m_vEffects[_i].DeactivateTech();
					g_LastFeature = kPixelPhong;

					TDesc.Features = kPixelPhong;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelBlinnPhong)
				{
					m_vEffects[_i].DeactivateTech();
					g_LastFeature = kPixelBlinnPhongNM;

					TDesc.Features = kPixelBlinnPhongNM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelBlinnPhongNM)
				{
					m_vEffects[_i].DeactivateTech();
					g_LastFeature = kPixelBlinnPhong;

					TDesc.Features = kPixelBlinnPhong;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
			}
			else
			{
				m_vEffects[_i].DeactivateTech();

				TDesc.Features = 0;
				TDesc.LightingPlace = CurrentLightingPlaces;
				TDesc.LightingModel = CurrentLightingModels;
				TDesc.ColorCorr = CurrentCorrection;

				m_vEffects[_i].ActivateTech(TDesc);
				m_vEffects[_i].m_ImGuiSpecularMap = false;
			}
		}
	}

	if (m_vEffects[_i].GetActiveTech() > kPixelBlinnPhong)
	{
		if (ImGui::Checkbox("Specular Map", &m_vEffects[_i].m_ImGuiSpecularMap))
		{
			if (m_vEffects[_i].m_ImGuiSpecularMap)
			{
				if (m_vEffects[_i].GetActiveTech() == kPixelPhongNM)
				{
					m_vEffects[_i].DeactivateTech();

					TDesc.Features = kPixelPhongNMSM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelPhongNMSM)
				{
					m_vEffects[_i].DeactivateTech();

					TDesc.Features = kPixelPhongNM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelBlinnPhongNM)
				{
					m_vEffects[_i].DeactivateTech();

					TDesc.Features = kPixelBlinnPhongNMSM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
				else if (m_vEffects[_i].GetActiveTech() == kPixelBlinnPhongNMSM)
				{
					m_vEffects[_i].DeactivateTech();

					TDesc.Features = kPixelBlinnPhongNM;
					TDesc.LightingPlace = CurrentLightingPlaces;
					TDesc.LightingModel = CurrentLightingModels;
					TDesc.ColorCorr = CurrentCorrection;

					m_vEffects[_i].ActivateTech(TDesc);
				}
			}
			else
			{
				m_vEffects[_i].DeactivateTech();

				TDesc.Features = g_LastFeature;
				TDesc.LightingPlace = CurrentLightingPlaces;
				TDesc.LightingModel = CurrentLightingModels;
				TDesc.ColorCorr = CurrentCorrection;

				m_vEffects[_i].ActivateTech(TDesc);
			}
		}
	}
}

void DeferredMenu(const int _i)
{
	LightingModelsLabel = LightingModels[CurrentLightingModels];
	if (ImGui::BeginCombo("Lighting Models", LightingModelsLabel, NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(LightingModels); n++)
		{
			const bool IsSelected = (m_vEffects[_i].GetActiveTech() == n);
			if (ImGui::Selectable(LightingModels[n], IsSelected))
			{
				CurrentLightingModels = n;
				m_vEffects[_i].DeactivateTech();
				m_vEffects[_i].m_ImGuiNormalMap = false;
				m_vEffects[_i].m_ImGuiSpecularMap = false;

				TDesc.Features = 0;
				TDesc.LightingModel = CurrentLightingModels;

				m_vEffects[_i].ActivateTech(TDesc);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	NormalLabel = Normals[CurrentNormal];
	if (ImGui::BeginCombo("Normals", NormalLabel, NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Normals); n++)
		{
			const bool IsSelected = (m_vEffects[_i].GetActiveTech() == n);
			if (ImGui::Selectable(Normals[n], IsSelected))
			{
				CurrentNormal = n;
				m_vEffects[_i].DeactivateTech();

				TDesc.LightingModel = CurrentLightingModels;
				TDesc.NormalCalc = CurrentNormal;

				m_vEffects[_i].ActivateTech(TDesc);
			}

			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	CorrectionLabel = ColorCorrections[CurrentCorrection];
	if (ImGui::BeginCombo("Color Correction", CorrectionLabel, NULL))
	{
		for (int n = 0; n < IM_ARRAYSIZE(ColorCorrections); n++)
		{
			const bool IsSelected = (CurrentCorrection == n);
			if (ImGui::Selectable(ColorCorrections[n], IsSelected))
			{
				CurrentCorrection = n;
				CorrectionLabel = ColorCorrections[n];

				TDesc.LightingModel = CurrentLightingModels;
				TDesc.NormalCalc = CurrentNormal;
				TDesc.ColorCorr = CurrentCorrection;

				m_vEffects[_i].DeactivateTech();
				m_vEffects[_i].ActivateTech(TDesc);
			}

			if (IsSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (CurrentLightingModels != 0)
	{
		if (ImGui::Checkbox("Specular Map", &m_vEffects[_i].ImGuiDefSM))
		{
			if (m_vEffects[_i].ImGuiDefSM)
			{
				m_vEffects[_i].DeactivateTech();

				TDesc.LightingModel = CurrentLightingModels;
				TDesc.Specular = true;
				m_vEffects[_i].ActivateTech(TDesc);
			}
			else
			{
				m_vEffects[_i].DeactivateTech();

				TDesc.LightingModel = CurrentLightingModels;
				TDesc.Specular = false;
				m_vEffects[_i].ActivateTech(TDesc);
			}
		}
	}
}

void EffectsMenu(const int _i)
{
	if (ImGui::TreeNode(m_vEffects[_i].GetName().c_str()))
	{
		TechLabel = RenderTechs[m_vEffects[_i].GetActiveRenderTech()];
		if (ImGui::BeginCombo("Render Techs", TechLabel, NULL))
		{
			for (int n = 0; n < IM_ARRAYSIZE(RenderTechs); n++)
			{
				const bool IsSelected = (CurrentTech == n);
				if (ImGui::Selectable(RenderTechs[n], IsSelected))
				{
					CurrentTech = n;
					TechLabel = RenderTechs[n];

					if (CurrentTech == 0)
					{
						m_vEffects[_i].DeactivateTech();
						m_vEffects[_i].SetRenderTech(0);

						TDesc.Features = 0;
						TDesc.LightingModel = 0;
						TDesc.LightingPlace = 0;

						LightingPlacesLabel = LightingPlaces[0];
						CurrentLightingPlaces = 0;

						LightingModelsLabel = LightingModels[0];
						CurrentLightingModels = 0;

						CorrectionLabel = ColorCorrections[0];
						CurrentCorrection = 0;

						m_vEffects[_i].ActivateTech(TDesc);
					}
					else if (CurrentTech == 1)
					{
						m_vEffects[_i].DeactivateTech();
						m_vEffects[_i].SetRenderTech(1);

						TDesc.Features = 0;
						TDesc.LightingModel = 0;
						TDesc.LightingPlace = 0;
						TDesc.NormalCalc = 0;
						TDesc.ColorCorr = 0;

						LightingPlacesLabel = LightingPlaces[0];
						CurrentLightingPlaces = 0;

						LightingModelsLabel = LightingModels[0];
						CurrentLightingModels = 0;

						CorrectionLabel = ColorCorrections[0];
						CurrentCorrection = 0;

						NormalLabel = Normals[0];
						CurrentNormal = 0;

						m_vEffects[_i].ActivateTech(TDesc);
					}
				}

				if (IsSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}


		if (m_vEffects[_i].GetActiveRenderTech() == 0)
		{
			ForwardMenu(_i);
		}
		else if (m_vEffects[_i].GetActiveRenderTech() == 1)
		{
			DeferredMenu(_i);
		}

		if (ImGui::TreeNode("Passes"))
		{
			PassesMenu(_i);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	ImGui::Separator();
}

void UIRender()
{
	// Start the Dear ImGui frame
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		OverImGuiWindow = true;
	}
	else
	{
		OverImGuiWindow = false;
	}
#if defined(DX11)
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
#endif
#if defined(OGL)
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
#endif
	//ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

	ImGui::NewFrame();

	ImGui::Begin("Configuration", NULL, ImGuiWindowFlags_MenuBar);

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("Menu"))
	//	{
	//		ShowMenuOptions();
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}

	if (ImGui::CollapsingHeader("Models"))
	{
		AccessModels();
	}
	
	if (m_vEffects.size() != 0)
	{
		if (ImGui::CollapsingHeader("Render Targets"))
		{
			if (m_vEffects[0].GetActiveRenderTech() == 1)
			{
				for (unsigned int i = 0; i < 4; ++i)
				{
					#if defined(DX11)
					ImGui::ImageButton((void*)RM::GetRenderManager().GBufferSRV[i].GetDXSRV(), ImVec2(1920 / 4, 1080 / 4));
					#endif
				}
				#if defined(DX11)
				ImGui::ImageButton((void*)RM::GetRenderManager().SkyboxSRV.GetDXSRV(), ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().DefSSAOSRV.GetDXSRV(), ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().DefCopySRV.GetDXSRV(), ImVec2(1920 / 4, 1080 / 4));
				#endif
				#if defined(OGL)
				ImGui::ImageButton((void*)RM::GetRenderManager().PositionTex, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().NormalTex, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().SpecularTex, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().AlbedoTex, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().DefSkyboxTex, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().SSAOTex, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().ToneTex, ImVec2(1920 / 4, 1080 / 4));
				#endif
			}
			else if (m_vEffects[0].GetActiveRenderTech() == 0)
			{
				for (unsigned int i = 0; i < 2; ++i)
				{
					#if defined(DX11)
					ImGui::ImageButton((void*)RM::GetRenderManager().ForwardSRV[i].GetDXSRV(), ImVec2(1920 / 4, 1080 / 4));
					#endif
				}
				#if defined(OGL)
				ImGui::ImageButton((void*)RM::GetRenderManager().SkyboxTexOGL, ImVec2(1920 / 4, 1080 / 4));
				ImGui::ImageButton((void*)RM::GetRenderManager().LightTexOGL, ImVec2(1920 / 4, 1080 / 4));
				#endif
			}
		}
	}

	float TextureWidth = 100.0f;
	float TextureHeight = 100.0f;
	ImVec2 UVMin = ImVec2(0.0f, 0.0f);
	ImVec2 UVMax = ImVec2(1.0f, 1.0f);
	ImVec4 Tint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 Border = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

	ImGui::End();

	ImGui::Begin("Enviroment", NULL, 0);

	if (ImGui::CollapsingHeader("Lights"))
	{
		if (ImGui::DragFloat("kDiffuse", &kDiffuse, 0.01f, 0.0f, 100.0f))
		{
			#if defined(DX11)
			RM::GetRenderManager().g_Diffuse.kDiffuse = XMFLOAT4(kDiffuse, 0.0f, 0.0f, 0.0f);
			#endif
			#if defined(OGL)
			RM::GetRenderManager().g_Diffuse.kDiffuse[0] = kDiffuse;
			RM::GetRenderManager().g_Diffuse.kDiffuse[1] = 0.0f;
			RM::GetRenderManager().g_Diffuse.kDiffuse[2] = 0.0f;
			RM::GetRenderManager().g_Diffuse.kDiffuse[3] = 0.0f;
			#endif
		}
		if (ImGui::DragFloat("kSpecular", &kSpecular, 0.01f, 0.0f, 100.0f))
		{
			#if defined(DX11)
			RM::GetRenderManager().g_Specular.kSpecular = XMFLOAT4(kSpecular, 0.0f, 0.0f, 0.0f);
			#endif
			#if defined(OGL)
			RM::GetRenderManager().g_Specular.kSpecular[0] = kSpecular;
			RM::GetRenderManager().g_Specular.kSpecular[1] = 0.0f;
			RM::GetRenderManager().g_Specular.kSpecular[2] = 0.0f;
			RM::GetRenderManager().g_Specular.kSpecular[3] = 0.0f;
			#endif
		}
		if (ImGui::DragFloat("Shininess", &Shininess, 0.01f, 0.01f, 100.0f))
		{
			#if defined(DX11)
			RM::GetRenderManager().g_Specular.Shininess = XMFLOAT4(Shininess, 0.0f, 0.0f, 0.0f);
			#endif
			#if defined(OGL)
			RM::GetRenderManager().g_Specular.Shininess[0] = Shininess;
			RM::GetRenderManager().g_Specular.Shininess[1] = 0.0f;
			RM::GetRenderManager().g_Specular.Shininess[2] = 0.0f;
			RM::GetRenderManager().g_Specular.Shininess[3] = 0.0f;
			#endif
		}
		if (ImGui::DragFloat("Expossure", &ExpossureGui, 0.01f, 0.01f, 100.0f))
		{
			#if defined(DX11)
			RM::GetRenderManager().g_Expossure.Expo = XMFLOAT4(ExpossureGui, 0.0f, 0.0f, 0.0f);
			#endif
			#if defined(OGL)
			RM::GetRenderManager().g_Expossure.Expo[0] = ExpossureGui;
			RM::GetRenderManager().g_Expossure.Expo[1] = 0.0f;
			RM::GetRenderManager().g_Expossure.Expo[2] = 0.0f;
			RM::GetRenderManager().g_Expossure.Expo[3] = 0.0f;
			#endif
		}
		if (m_vEffects.size() != 0)
		{
			if (m_vEffects[0].GetActiveRenderTech() == 1)
			{
				if (ImGui::TreeNode("SSAO"))
				{
					if (ImGui::DragFloat("Radius", &AORadius, 0.01f, 0.0f, 1.0f))
					{
						RM::GetRenderManager().g_AO.Radius = AORadius;
					}

					if (ImGui::DragFloat("Bias", &AOBias, 0.01f, 0.0f, 1.0f))
					{
						RM::GetRenderManager().g_AO.Bias = AOBias;
					}

					if (ImGui::DragFloat("Scale", &AOScale, 0.01f, 0.0f, 1.0f))
					{
						RM::GetRenderManager().g_AO.Scale = AOScale;
					}

					if (ImGui::DragFloat("Intensity", &AOIntensity, 0.01f, 0.0f, 10.0f))
					{
						RM::GetRenderManager().g_AO.Intensity = AOIntensity;
					}

					if (ImGui::DragInt("Iterations", &AOIterations, 1.0f, 1, 10))
					{
						RM::GetRenderManager().g_AO.Iterations = AOIterations;
					}
					ImGui::TreePop();
				}
			}
		}
		ImGui::Separator();

		if (ImGui::TreeNode("Ambient Light"))
		{
			if (ImGui::DragFloat("kAmbient", &kAmbient, 0.01f, 0.0f, 100.0f))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_Ambient.kAmbient = XMFLOAT4(kAmbient, 0.0f, 0.0f, 0.0f);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_Ambient.kAmbient[0] = kAmbient;
				RM::GetRenderManager().g_Ambient.kAmbient[1] = 0.0f;
				RM::GetRenderManager().g_Ambient.kAmbient[2] = 0.0f;
				RM::GetRenderManager().g_Ambient.kAmbient[3] = 0.0f;
				#endif
			}
			if (ImGui::ColorPicker4("Ambient Color##4", (float*)&AmbientLightColor, Flags))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_Ambient.AmbientColor = XMFLOAT4(AmbientLightColor.x, AmbientLightColor.y, AmbientLightColor.z, AmbientLightColor.w);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_Ambient.AmbientColor[0] = AmbientLightColor.x;
				RM::GetRenderManager().g_Ambient.AmbientColor[1] = AmbientLightColor.y;
				RM::GetRenderManager().g_Ambient.AmbientColor[2] = AmbientLightColor.z;
				RM::GetRenderManager().g_Ambient.AmbientColor[3] = AmbientLightColor.w;
				#endif
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Directional Light"))
		{
			if (ImGui::DragFloat3("Direction", DirectionLightDir, 0.001f, -1.0f, 1.0f))
			{
			#if defined(DX11)
				RM::GetRenderManager().g_DirLightBufferDesc.Dir = XMFLOAT4(DirectionLightDir[0], DirectionLightDir[1], DirectionLightDir[2], 0.0f);
			#endif
			#if defined(OGL)
				RM::GetRenderManager().g_DirLightBufferDesc.Dir[0] = DirectionLightDir[0];
				RM::GetRenderManager().g_DirLightBufferDesc.Dir[1] = DirectionLightDir[1];
				RM::GetRenderManager().g_DirLightBufferDesc.Dir[2] = DirectionLightDir[2];
				RM::GetRenderManager().g_DirLightBufferDesc.Dir[3] = 0.0f;

			#endif
			}
			
			if (ImGui::ColorPicker4("Color##4", (float*)&DirLightColor, Flags))
			{
			#if defined(DX11)
				RM::GetRenderManager().g_DirLightBufferDesc.Color = XMFLOAT4(DirLightColor.x, DirLightColor.y, DirLightColor.z, DirLightColor.w);
			#endif
			#if defined(OGL)
				RM::GetRenderManager().g_DirLightBufferDesc.Color[0] = DirLightColor.x;
				RM::GetRenderManager().g_DirLightBufferDesc.Color[1] = DirLightColor.y;
				RM::GetRenderManager().g_DirLightBufferDesc.Color[2] = DirLightColor.z;
				RM::GetRenderManager().g_DirLightBufferDesc.Color[3] = DirLightColor.w;
			#endif
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point Light"))
		{
			if (ImGui::DragFloat3("Position", PointLightPos, 0.1f))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_PointLightBufferDesc.Position = XMFLOAT4(PointLightPos[0], PointLightPos[1], PointLightPos[2], 1.0f);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_PointLightBufferDesc.Position[0] = PointLightPos[0];
				RM::GetRenderManager().g_PointLightBufferDesc.Position[1] = PointLightPos[1];
				RM::GetRenderManager().g_PointLightBufferDesc.Position[2] = PointLightPos[2];
				RM::GetRenderManager().g_PointLightBufferDesc.Position[3] = 1.0f;
				#endif
			}
			if (ImGui::DragFloat("Attenuation", &PointLightAttenuation, 0.1f, 0.0f, 100.0f))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_PointLightBufferDesc.Attenuation = XMFLOAT4(PointLightAttenuation, 0.0f, 0.0f, 0.0f);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_PointLightBufferDesc.Attenuation[0] = PointLightAttenuation;
				RM::GetRenderManager().g_PointLightBufferDesc.Attenuation[1] = 0.0f;
				RM::GetRenderManager().g_PointLightBufferDesc.Attenuation[2] = 0.0f;
				RM::GetRenderManager().g_PointLightBufferDesc.Attenuation[3] = 0.0f;
				#endif
			}

			if (ImGui::ColorPicker4("Color##4", (float*)&PointLightColor, Flags))
			{
			#if defined(DX11)
				RM::GetRenderManager().g_PointLightBufferDesc.Color = XMFLOAT4(PointLightColor.x, PointLightColor.y, PointLightColor.z, PointLightColor.w);
			#endif
			#if defined(OGL)
				RM::GetRenderManager().g_PointLightBufferDesc.Color[0] = PointLightColor.x;
				RM::GetRenderManager().g_PointLightBufferDesc.Color[1] = PointLightColor.y;
				RM::GetRenderManager().g_PointLightBufferDesc.Color[2] = PointLightColor.z;
				RM::GetRenderManager().g_PointLightBufferDesc.Color[3] = PointLightColor.w;
			#endif
			}
				ImGui::TreePop();
		}
		
		if (ImGui::TreeNode("SpotLight"))
		{
			if (ImGui::DragFloat3("Direction", SpotLightDir, 0.001f, -1.0f, 1.0f))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_SpotLightBufferDesc.Dir = XMFLOAT4(SpotLightDir[0], SpotLightDir[1], SpotLightDir[2], 0.0f);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_SpotLightBufferDesc.Dir[0] = SpotLightDir[0];
				RM::GetRenderManager().g_SpotLightBufferDesc.Dir[1] = SpotLightDir[1];
				RM::GetRenderManager().g_SpotLightBufferDesc.Dir[2] = SpotLightDir[2];
				RM::GetRenderManager().g_SpotLightBufferDesc.Dir[3] = 0.0f;
				#endif
			}

			if (ImGui::DragFloat3("Position", SpotLightPos, 0.1f))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_SpotLightBufferDesc.Pos = XMFLOAT4(SpotLightPos[0], SpotLightPos[1], SpotLightPos[2], 1.0f);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_SpotLightBufferDesc.Pos[0] = SpotLightPos[0];
				RM::GetRenderManager().g_SpotLightBufferDesc.Pos[1] = SpotLightPos[1];
				RM::GetRenderManager().g_SpotLightBufferDesc.Pos[2] = SpotLightPos[2];
				RM::GetRenderManager().g_SpotLightBufferDesc.Pos[3] = 1.0f;
				#endif
			}

			if (ImGui::DragFloat("Attenuation", &SpotLightAttenuation, 0.1f, 0.0f, 100.0f))
			{
				RM::GetRenderManager().g_SpotLightBufferDesc.Attenuation = SpotLightAttenuation;
			}

			if (ImGui::DragFloat("Inner Radius", &InnerRadius, 0.1f))
			{
				RM::GetRenderManager().g_SpotLightBufferDesc.InnerRadius = InnerRadius;
			}

			if (ImGui::DragFloat("Outer Radius", &OuterRadius, 0.1f))
			{
				RM::GetRenderManager().g_SpotLightBufferDesc.OuterRadius = OuterRadius;
			}

			if (ImGui::ColorPicker4("Color##4", (float*)&SpotLightColor, Flags))
			{
				#if defined(DX11)
				RM::GetRenderManager().g_SpotLightBufferDesc.Color = XMFLOAT4(SpotLightColor.x, SpotLightColor.y, SpotLightColor.z, SpotLightColor.w);
				#endif
				#if defined(OGL)
				RM::GetRenderManager().g_SpotLightBufferDesc.Color[0] = SpotLightColor.x;
				RM::GetRenderManager().g_SpotLightBufferDesc.Color[1] = SpotLightColor.y;
				RM::GetRenderManager().g_SpotLightBufferDesc.Color[2] = SpotLightColor.z;
				RM::GetRenderManager().g_SpotLightBufferDesc.Color[3] = SpotLightColor.w;
				#endif
			}
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("Effects"))
	{
		for (unsigned int i = 0; i < m_vEffects.size(); ++i)
		{
			EffectsMenu(i);
		}
		if (ImGui::Button("Add"))
		{
			g_NewEffect = true;
		}
	}

	if (g_NewEffect)
	{
		ImGui::Begin("New Effect", NULL, 0);
		size_t Size = sizeof(NewEffectName);

		ImGui::InputText("Effect Name", &NewEffectName[0], Size);

		ImGui::Separator();

		ImGui::Text("Render Technique");

		if (ImGui::Checkbox("Forward", &TDesc.ForwardRender))
		{
			if (TDesc.DeferredRender)
			{
				TDesc.DeferredRender = false;
			}
		}

		ImGui::SameLine();

		if (ImGui::Checkbox("Deferred", &TDesc.DeferredRender))
		{
			if (TDesc.ForwardRender)
			{
				TDesc.ForwardRender = false;
			}
		}



		if (TDesc.ForwardRender)
		{
			ImGui::Separator();

			ImGui::Text("Where the light will be calculated?");

			if (ImGui::BeginCombo("", LightingPlacesLabel, NULL))
			{
				for (int n = 0; n < IM_ARRAYSIZE(LightingPlaces); n++)
				{
					const bool IsSelected = (CurrentLightingPlaces == n);
					if (ImGui::Selectable(LightingPlaces[n], IsSelected))
					{
						CurrentLightingPlaces = n;
						LightingPlacesLabel = LightingPlaces[n];
					}
			
					if (IsSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}

		ImGui::Separator();
		ImGui::Text("Lighting Model");
		ImGui::ListBox("", &CurrentLightingModels, LightingModels, IM_ARRAYSIZE(LightingModels), 4);

		ImGui::Separator();

		if (ImGui::BeginCombo("Color Correction", CorrectionLabel, NULL))
		{
			for (int n = 0; n < IM_ARRAYSIZE(ColorCorrections); n++)
			{
				const bool IsSelected = (CurrentCorrection == n);
				if (ImGui::Selectable(ColorCorrections[n], IsSelected))
				{
					CurrentCorrection = n;
					CorrectionLabel = ColorCorrections[n];
				}

				if (IsSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Ok"))
		{
			TDesc.LightingPlace = CurrentLightingPlaces;
			TDesc.LightingModel = CurrentLightingModels;
			TDesc.ColorCorr = CurrentCorrection;

			TDesc.Features = 0;

			m_vEffects.push_back(Effect(&m_vTechs, NewEffectName, TDesc));

			m_vEffects.back().ActivateTech(TDesc);
			NewEffectName = "";
			g_NewEffect = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			g_NewEffect = false;
		}

		ImGui::End();
	}
	ImGui::End();
	// render UI
	//ImGui::ShowDemoWindow();
	ImGui::Render();
#if defined(DX11)
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

#if defined(OGL)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}
#endif
void Update()
{
	float RunningTime = GetRunningTime();

	if(LeftClick)
	{
	#if defined(OGL)
		glfwGetCursorPos(m_OGLWindow, &PreviousMouseRelativePosition[0], &PreviousMouseRelativePosition[1]);
	#endif
	#if defined(DX11)
		LPPOINT Direction = new POINT;
		GetCursorPos(Direction);
		ScreenToClient(g_hwnd, Direction);
		PreviousMouseRelativePosition[0] = Direction->x;
		PreviousMouseRelativePosition[1] = Direction->y;
	#endif
	}
	#if defined(DX11)
	RM::GetRenderManager().Update(RunningTime);
	m_Obj.Update();
	#endif
	#if defined(OGL)
	GetRenderManager().Update(RunningTime);

	m_Obj.UpdateOGL(m_OGLWindow);
	#endif
}

void Render()
{
#if defined(DX11)
	for (int i = 0; i < m_vEffects.size(); ++i)
	{
		m_vEffects[i].Render();
	}
	m_Obj.Render();

	UIRender();

	RM::GetRenderManager().SetBackBufferCleaned(false);
	GraphicsModule::GetManagerObj(g_hwnd).GetSwapChain().CPresent(0, 0);
#endif
#if defined(OGL)
	for (int i = 0; i < m_vEffects.size(); ++i)
	{
		m_vEffects[i].Render();
	}
	m_Obj.Render();

	UIRender();
	RM::GetRenderManager().SetBackBufferCleaned(false);
	glfwSwapBuffers(m_OGLWindow);
	glfwPollEvents();
#endif
}

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
	if (FAILED(InitWindow(1920, 1080)))
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

	int Flags[2] = { 0, 3 };

	GetRenderManager().LoadModel("C:\\Users\\simam\\source\\repos\\Graficas1\\Resources\\SAQ.obj");
	GetRenderManager().LoadModel("C:\\Users\\simam\\source\\repos\\Graficas1\\Resources\\Sphere.3ds");

	//Effects
	for (unsigned int k = 0; k < 2; k++)
	{ 
		TechDesc TDesc;
		TDesc.EffectNum = 2;
		TDesc.ActualEffect = k;
		TDesc.hwnd = g_hwnd;
		TDesc.ColorCorr = 0;

		int Counter = 0;
		int CounterLoop = 0;

		if (k == 0)
		{
			TDesc.PassNum = 4;

			for (unsigned int j = 0; j < 30; ++j)
			{
				if (j < 12)
				{
					TDesc.DefLightFlags = 0;
				}
				else if (j >= 12 && j < 24)
				{
					TDesc.DefLightFlags = 1;
				}
				else if (j >= 24)
				{
					TDesc.DefLightFlags = 2;
				}

				TDesc.DeferredFlags = j;

				m_vTechs.push_back(TDesc);

				if (j >= 24)
				{
					if (Counter == 1)
					{
						TDesc.ColorCorr++;
						if (TDesc.ColorCorr >= 3)
						{
							TDesc.ColorCorr = 2;
						}
						Counter = 0;
					}
					else
					{
						Counter++;
					}
				}

				else
				{
					if (Counter == 3)
					{
						TDesc.ColorCorr++;
						if (TDesc.ColorCorr >= 3)
						{
							TDesc.ColorCorr = 2;
						}
						Counter = 0;
					}
					else
					{ 
						Counter++;
					}
				}
				if (CounterLoop == 11 || CounterLoop == 23)
				{
					CounterLoop = 0;
					Counter = 0;
					TDesc.ColorCorr = 0;
				}
				else
				{ 
					CounterLoop++;
				}
			}
		}
		else if (k == 1)
		{
			TDesc.PassNum = 3;
			for (unsigned int i = 30; i < 63; ++i)
			{
				TDesc.TechTypesFlag = i;

				m_vTechs.push_back(TDesc);

				if (Counter == 10)
				{
					TDesc.ColorCorr++;
					Counter = 0;
				}
				else
				{ 
					Counter++;
				}
			}
		}
	}

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
	for (int i = 0; i < RM::GetRenderManager().m_vModels.size(); ++i)
	{
		GetRenderManager().m_vModels[i]->CleanUpDXResources();
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para hacer feliz a MacOS ; Aunque no deber�a ser necesaria
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //No queremos el viejo OpenGL 

	//Crear una ventana y su contexto OpenGL
	m_OGLWindow = glfwCreateWindow(1920, 1080, "GRAFICAS SIME1", NULL, NULL);
	if (m_OGLWindow == NULL) {
		fprintf(stderr, "Falla al abrir una ventana GLFW. Si usted tiene una GPU Intel, est� no es compatible con 3.3. Intente con la versi�n 2.1 de los tutoriales.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_OGLWindow); // Inicializar GLEW
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//glfwSetInputMode(m_OGLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetInputMode(m_OGLWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//glfwSetInputMode(m_OGLWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_OGLWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

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

	glViewport(0, 0, 1920, 1080);
	glfwSetFramebufferSizeCallback(m_OGLWindow, framebuffer_size_callback);
	glfwSetMouseButtonCallback(m_OGLWindow, mouse_button_callback);
	glfwSetCursorPosCallback(m_OGLWindow, cursor_position_callback);
	glfwSetKeyCallback(m_OGLWindow, KeyCallback);

	FreeImage_Initialise();
	std::cout << "FreeImage" << FreeImage_GetVersion() << "\n";
	std::cout << FreeImage_GetCopyrightMessage() << "\n\n";

	GetRenderManager().LoadModel("C:\\Users\\simam\\source\\repos\\Graficas1\\Resources\\SAQ.obj");
	GetRenderManager().LoadModel("C:\\Users\\simam\\source\\repos\\Graficas1\\Resources\\Sphere.3ds");

	//Effects
	for (unsigned int k = 0; k < 2; k++)
	{
		TechDesc TDesc;
		TDesc.EffectNum = 2;
		TDesc.ActualEffect = k;
		TDesc.hwnd = g_hwnd;
		TDesc.ColorCorr = 0;

		int Counter = 0;
		int CounterLoop = 0;

		if (k == 0)
		{
			TDesc.PassNum = 4;

			for (unsigned int j = 0; j < 30; ++j)
			{
				if (j < 12)
				{
					TDesc.DefLightFlags = 0;
				}
				else if (j >= 12 && j < 24)
				{
					TDesc.DefLightFlags = 1;
				}
				else if (j >= 24)
				{
					TDesc.DefLightFlags = 2;
				}

				TDesc.DeferredFlags = j;

				m_vTechs.push_back(TDesc);

				if (j >= 24)
				{
					if (Counter == 1)
					{
						TDesc.ColorCorr++;
						if (TDesc.ColorCorr >= 3)
						{
							TDesc.ColorCorr = 2;
						}
						Counter = 0;
					}
					else
					{
						Counter++;
					}
				}

				else
				{
					if (Counter == 3)
					{
						TDesc.ColorCorr++;
						if (TDesc.ColorCorr >= 3)
						{
							TDesc.ColorCorr = 2;
						}
						Counter = 0;
					}
					else
					{
						Counter++;
					}
				}
				if (CounterLoop == 11 || CounterLoop == 23)
				{
					CounterLoop = 0;
					Counter = 0;
					TDesc.ColorCorr = 0;
				}
				else
				{
					CounterLoop++;
				}
			}
		}
		else if (k == 1)
		{
			TDesc.PassNum = 3;
			for (unsigned int i = 30; i < 63; ++i)
			{
				TDesc.TechTypesFlag = i;

				m_vTechs.push_back(TDesc);

				if (Counter == 10)
				{
					TDesc.ColorCorr++;
					Counter = 0;
				}
				else
				{
					Counter++;
				}
			}
		}
	}

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
