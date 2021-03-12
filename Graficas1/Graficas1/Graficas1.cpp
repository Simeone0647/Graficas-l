#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "GraphicModule.h"

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HWND g_hwnd;
GraphicsModule::test m_Obj;

//-----------------------------------------------------------------------------------------

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


void UIRender()
{
	// Start the Dear ImGui frame
#if defined(DX11)
	ImGui_ImplDX11_NewFrame();
#endif
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// example window
	if (ImGui::Begin("Another Window", nullptr))
	{
		static float dir[3]{};
		if (ImGui::DragFloat3("Direccion de la luz", dir, 0.001f, -1.0f, 1.0f))
		{
			#if defined(DX11)
			m_Obj.g_DirLightBufferDesc.Dir = XMFLOAT4(dir[0], dir[1], dir[2], 0.0f);
			#endif
		}
	}
	ImGui::End();

	// render UI
	ImGui::Render();
#if defined(DX11)
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}

void Update()
{
	m_Obj.Update();
}

void Render()
{
	m_Obj.Render();
#if defined(DX11)
	UIRender();
	GraphicsModule::GetManagerObj(g_hwnd).GetSwapChain().CPresent(0, 0);
#endif
}

int main()
{
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
}
