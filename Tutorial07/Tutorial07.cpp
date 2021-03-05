//--------------------------------------------------------------------------------------
// File: Tutorial07.cpp
//
// This application demonstrates texturing
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "CCamera.h"
#include "CVector3.h"
#include "CMesh.h"
#include "CVertex.h"
#include "CDevice.h"
#include "CDeviceContext.h"
#include "CSwapChain.h"
#include "resource.h"

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT2 Tex;
};

struct CBNeverChanges
{
    XMMATRIX mView;
};

struct CBChangeOnResize
{
    XMMATRIX mProjection;
};

struct CBChangesEveryFrame
{
    XMMATRIX mWorld;
    XMFLOAT4 vMeshColor;
};

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE                           g_hInst = NULL;
HWND                                g_hWnd = NULL;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*                       g_pd3dDevice = NULL;
CDevice*                            g_SimeDevice = new CDevice;
CDeviceContext*                     g_SimeDeviceContext = new CDeviceContext;
//ID3D11DeviceContext*                g_pImmediateContext = NULL;
//IDXGISwapChain*                     g_pSwapChain = NULL;
CSwapChain*                         g_SimeSwapChain = new CSwapChain;
ID3D11RenderTargetView*             g_pRenderTargetView = NULL;
ID3D11Texture2D*                    g_pDepthStencil = NULL;
ID3D11DepthStencilView*             g_pDepthStencilView = NULL;
ID3D11VertexShader*                 g_pVertexShader = NULL;
ID3D11PixelShader*                  g_pPixelShader = NULL;
ID3D11InputLayout*                  g_pVertexLayout = NULL;
ID3D11Buffer*                       g_pVertexBuffer = NULL;
ID3D11Buffer*                       g_pIndexBuffer = NULL;
ID3D11Buffer*                       g_pCBNeverChanges = NULL;
ID3D11Buffer*                       g_pCBChangeOnResize = NULL;
ID3D11Buffer*                       g_pCBChangesEveryFrame = NULL;
ID3D11ShaderResourceView*           g_pTextureRV = NULL;
ID3D11SamplerState*                 g_pSamplerLinear = NULL;
XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX                            g_Projection;
XMMATRIX                            g_Translation;
XMFLOAT4                            g_vMeshColor( 0.7f, 0.7f, 0.7f, 1.0f );
CCamera                             m_PerspectiveCamera;
CCamera                             m_OrtographicCamera;
CCamera*                            m_Camera;
CVertex                             CubeVertex[24];
CMesh                               FirstCube;
CMesh                               SecondCube;
CMesh                               ThirdCube;
CMesh                               FourthCube;
LPPOINT                             MouseInitPos = new POINT;
LPPOINT                             MouseFinalPos = new POINT;
LPPOINT                             MouseDirection = new POINT;
unsigned short*                     CubeVertexIndex = new unsigned short[36];
bool                                m_IsPerspectiveActive = true;
bool                                m_IsFirstFrame = false;
bool                                m_IsFirstPosStored = false;
D3D11_VIEWPORT                      g_Viewport;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();
void Update();

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Update();
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile( WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
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
    hr = D3DX11CompileFromFile( szFileName, NULL, NULL, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
            OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
        if( pErrorBlob ) pErrorBlob->Release();
        return hr;
    }
    if( pErrorBlob ) pErrorBlob->Release();

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    //SIME1_DRIVER_TYPE driverTypes[] =
    //{
    //    SIME1_DRIVER_TYPE::SIME1_HARDWARE;
    //    SIME1_DRIVER_TYPE::SIME1_WRAP;
    //    SIME1_DRIVER_TYPE::SIME1_REFERENCE;
    //};

    UINT numDriverTypes = ARRAYSIZE( driverTypes ); 

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    //SIME1_FEATURE_LEVEL featureLevels[] =
    //{
    //    SIME1_FEATURE_LEVEL::SIME1_FEATURE_LEVEL_11_0;
    //    SIME1_FEATURE_LEVEL::SIME1_FEATURE_LEVEL_10_1,
    //    SIME1_FEATURE_LEVEL::SIME1_FEATURE_LEVEL_10_0;
    //};

    UINT numFeatureLevels = ARRAYSIZE( featureLevels );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                                            D3D11_SDK_VERSION, &sd, g_SimeSwapChain->GetSwapChainAdress(), g_SimeDevice->GetDeviceAdress(),
                                            &g_featureLevel, g_SimeDeviceContext->GetDeviceContextAdress());
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_SimeSwapChain->CGetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
    if( FAILED( hr ) )
        return hr;

    hr = g_SimeDevice->CCreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = g_SimeDevice->CCreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
    if( FAILED( hr ) )
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = g_SimeDevice->CCreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
    if( FAILED( hr ) )
        return hr;



    // Setup the viewport

    g_Viewport.Width = (FLOAT)width;
    g_Viewport.Height = (FLOAT)height;
    g_Viewport.MinDepth = 0.0f;
    g_Viewport.MaxDepth = 1.0f;
    g_Viewport.TopLeftX = 0;
    g_Viewport.TopLeftY = 0;

  
    // Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial07.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the vertex shader
    hr = g_SimeDevice->CCreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader );
    if( FAILED( hr ) )
    {    
        pVSBlob->Release();
        return hr;
    }

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE( layout );

    // Create the input layout
    hr = g_SimeDevice->CCreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &g_pVertexLayout );
    pVSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Compile the pixel shader
    ID3DBlob* pPSBlob = NULL;
    hr = CompileShaderFromFile( L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

    // Create the pixel shader
    hr = g_SimeDevice->CCreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader );
    pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

    // Create vertex buffer
    // SET ALL VERTEX PARAMS
    CubeVertex[0].SetPosition(-1.0f, 1.0f, -1.0f);
    CubeVertex[0].SetTexture(0.0f, 0.0f);
	CubeVertex[1].SetPosition(1.0f, 1.0f, -1.0f);
	CubeVertex[1].SetTexture(1.0f, 0.0f);
	CubeVertex[2].SetPosition(1.0f, 1.0f, 1.0f);
	CubeVertex[2].SetTexture(1.0f, 1.0f);
	CubeVertex[3].SetPosition(-1.0f, 1.0f, 1.0f);
	CubeVertex[3].SetTexture(0.0f, 1.0f);
   
	CubeVertex[4].SetPosition(-1.0f, -1.0f, -1.0f);
	CubeVertex[4].SetTexture(0.0f, 0.0f);
	CubeVertex[5].SetPosition(1.0f, -1.0f, -1.0f);
	CubeVertex[5].SetTexture(1.0f, 0.0f);
	CubeVertex[6].SetPosition(1.0f, -1.0f, 1.0f);
	CubeVertex[6].SetTexture(1.0f, 1.0f);
	CubeVertex[7].SetPosition(-1.0f, -1.0f, 1.0f);
	CubeVertex[7].SetTexture(0.0f, 1.0f);

	CubeVertex[8].SetPosition(-1.0f, -1.0f, 1.0f);
	CubeVertex[8].SetTexture(0.0f, 0.0f);
	CubeVertex[9].SetPosition(-1.0f, -1.0f, -1.0f);
	CubeVertex[9].SetTexture(1.0f, 0.0f);
	CubeVertex[10].SetPosition(-1.0f, 1.0f, -1.0f);
	CubeVertex[10].SetTexture(1.0f, 1.0f);
	CubeVertex[11].SetPosition(-1.0f, 1.0f, 1.0f);
	CubeVertex[11].SetTexture(0.0f, 1.0f);

	CubeVertex[12].SetPosition(1.0f, -1.0f, 1.0f);
	CubeVertex[12].SetTexture(0.0f, 0.0f);
	CubeVertex[13].SetPosition(1.0f, -1.0f, -1.0f);
	CubeVertex[13].SetTexture(1.0f, 0.0f);
	CubeVertex[14].SetPosition(1.0f, 1.0f, -1.0f);
	CubeVertex[14].SetTexture(1.0f, 1.0f);
	CubeVertex[15].SetPosition(1.0f, 1.0f, 1.0f);
	CubeVertex[15].SetTexture(0.0f, 1.0f);

	CubeVertex[16].SetPosition(-1.0f, -1.0f, -1.0f);
	CubeVertex[16].SetTexture(0.0f, 0.0f);
	CubeVertex[17].SetPosition(1.0f, -1.0f, -1.0f);
	CubeVertex[17].SetTexture(1.0f, 0.0f);
	CubeVertex[18].SetPosition(1.0f, 1.0f, -1.0f);
	CubeVertex[18].SetTexture(1.0f, 1.0f);
	CubeVertex[19].SetPosition(-1.0f, 1.0f, -1.0f);
	CubeVertex[19].SetTexture(0.0f, 1.0f);

	CubeVertex[20].SetPosition(-1.0f, -1.0f, 1.0f);
	CubeVertex[20].SetTexture(0.0f, 0.0f);
	CubeVertex[21].SetPosition(1.0f, -1.0f, 1.0f);
	CubeVertex[21].SetTexture(1.0f, 0.0f);
	CubeVertex[22].SetPosition(1.0f, 1.0f, 1.0f);
	CubeVertex[22].SetTexture(1.0f, 1.0f);
	CubeVertex[23].SetPosition(-1.0f, 1.0f, 1.0f);
	CubeVertex[23].SetTexture(0.0f, 1.0f);

    //SETTING THE CUBE VERTEX TO THE MESHES
    FirstCube.SetVertex(CubeVertex, 24);
    FirstCube.SetNumOfVertex(24);

    SecondCube.SetVertex(CubeVertex, 24);
    SecondCube.SetNumOfVertex(24);

    ThirdCube.SetVertex(CubeVertex, 24);
    ThirdCube.SetNumOfVertex(24);

    FourthCube.SetVertex(CubeVertex, 24);
    FourthCube.SetNumOfVertex(24);

    D3D11_BUFFER_DESC bd;
    ZeroMemory( &bd, sizeof(bd) );
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = CubeVertex->GetSize() * FirstCube.GetNumOfVertex();
    //bd.ByteWidth = sizeof( SimpleVertex ) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory( &InitData, sizeof(InitData) );
    InitData.pSysMem = FirstCube.GetVertex();
    //InitData.pSysMem = vertices;
    hr = g_SimeDevice->CCreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    //SET VERTEX INDEX ARRAY
    CubeVertexIndex[0] = 3;
    CubeVertexIndex[1] = 1;
    CubeVertexIndex[2] = 0;
    CubeVertexIndex[3] = 2;
    CubeVertexIndex[4] = 1;
    CubeVertexIndex[5] = 3;
    CubeVertexIndex[6] = 6;
    CubeVertexIndex[7] = 4;
    CubeVertexIndex[8] = 5;
    CubeVertexIndex[9] = 7;
    CubeVertexIndex[10] = 4;
    CubeVertexIndex[11] = 6;
    CubeVertexIndex[12] = 11;
    CubeVertexIndex[13] = 9;
    CubeVertexIndex[14] = 8;
    CubeVertexIndex[15] = 10;
    CubeVertexIndex[16] = 9;
    CubeVertexIndex[17] = 11;
    CubeVertexIndex[18] = 14;
    CubeVertexIndex[19] = 12;
    CubeVertexIndex[20] = 13;
    CubeVertexIndex[21] = 15;
    CubeVertexIndex[22] = 12;
    CubeVertexIndex[23] = 14;
    CubeVertexIndex[24] = 19;
    CubeVertexIndex[25] = 17;
    CubeVertexIndex[26] = 16;
    CubeVertexIndex[27] = 18;
    CubeVertexIndex[28] = 17;
    CubeVertexIndex[29] = 19;
    CubeVertexIndex[30] = 22;
    CubeVertexIndex[31] = 20;
    CubeVertexIndex[32] = 21;
    CubeVertexIndex[33] = 23;
    CubeVertexIndex[34] = 20;
    CubeVertexIndex[35] = 22;

    //SET ALL FOUR MESHES VERTEX INDEX
    FirstCube.SetVertexIndex(CubeVertexIndex, 36);
	SecondCube.SetVertexIndex(CubeVertexIndex, 36);
    ThirdCube.SetVertexIndex(CubeVertexIndex, 36);
    FourthCube.SetVertexIndex(CubeVertexIndex, 36);

	//SET NUM OF VERTEX INDEX
	FirstCube.SetNumOfVertexIndex(36);
	SecondCube.SetNumOfVertexIndex(36);
	ThirdCube.SetNumOfVertexIndex(36);
	FourthCube.SetNumOfVertexIndex(36);

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = FirstCube.GetVertexIndexSize();
  //bd.ByteWidth = sizeof( WORD ) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    InitData.pSysMem = FirstCube.GetVertexIndex();
    //InitData.pSysMem = indices;
    hr = g_SimeDevice->CCreateBuffer( &bd, &InitData, &g_pIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Create the constant buffers
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CBNeverChanges);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_SimeDevice->CCreateBuffer( &bd, NULL, &g_pCBNeverChanges );
    if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_SimeDevice->CCreateBuffer( &bd, NULL, &g_pCBChangeOnResize );
    if( FAILED( hr ) )
        return hr;
    
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_SimeDevice->CCreateBuffer( &bd, NULL, &g_pCBChangesEveryFrame );
    if( FAILED( hr ) )
        return hr;

    // Load the Texture
    hr = D3DX11CreateShaderResourceViewFromFile( g_SimeDevice->GetDevice(), L"seafloor.dds", NULL, NULL, &g_pTextureRV, NULL );
    if( FAILED( hr ) )
        return hr;

    // Create the sample state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof(sampDesc) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = g_SimeDevice->CCreateSamplerState( &sampDesc, &g_pSamplerLinear );
    if( FAILED( hr ) )
        return hr;

    // Initialize the world matrices
    g_World = XMMatrixIdentity();


    m_PerspectiveCamera.SetAt(0.0f, 0.0f, 0.0f);
    m_PerspectiveCamera.SetEye(0.0f, 3.0f, -6.0f);
    m_PerspectiveCamera.SetUp(0.0f, 1.0f, 0.0f);
    m_PerspectiveCamera.UpdateViewMatrix();
    m_PerspectiveCamera.UpdatePerspectiveProjectionMatrix(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	m_OrtographicCamera.SetAt(0.0f, 0.0f, 0.0f);
	m_OrtographicCamera.SetEye(0.0f, 3.0f, -6.0f);
	m_OrtographicCamera.SetUp(0.0f, 1.0f, 0.0f);
    m_OrtographicCamera.UpdateViewMatrix();
    m_OrtographicCamera.UpdateOrtographicProjectionMatrix(width, height, 0.01f, 100.0f);

    m_Camera = &m_PerspectiveCamera;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_SimeDeviceContext ) g_SimeDeviceContext->GetDeviceContext()->ClearState();

    if( g_pSamplerLinear ) g_pSamplerLinear->Release();
    if( g_pTextureRV ) g_pTextureRV->Release();
    if( g_pCBNeverChanges ) g_pCBNeverChanges->Release();
    if( g_pCBChangeOnResize ) g_pCBChangeOnResize->Release();
    if( g_pCBChangesEveryFrame ) g_pCBChangesEveryFrame->Release();
    if( g_pVertexBuffer ) g_pVertexBuffer->Release();
    if( g_pIndexBuffer ) g_pIndexBuffer->Release();
    if( g_pVertexLayout ) g_pVertexLayout->Release();
    if( g_pVertexShader ) g_pVertexShader->Release();
    if( g_pPixelShader ) g_pPixelShader->Release();
    if( g_pDepthStencil ) g_pDepthStencil->Release();
    if( g_pDepthStencilView ) g_pDepthStencilView->Release();
    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_SimeSwapChain ) g_SimeSwapChain->GetSwapChain()->Release();
    if( g_SimeDeviceContext ) g_SimeDeviceContext->GetDeviceContext()->Release();
    if( g_SimeDevice->GetDevice() ) g_SimeDevice->GetDevice()->Release();
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;
    CVector3 cam_pos;

    
    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        case WM_SIZE:
            if (m_Camera != NULL)
            {
            /* WORKING
                if (m_IsPerspectiveActive)
                { 
                    float width = LOWORD(lParam);
                    float height = HIWORD(lParam);
                    g_SimeSwapChain->ResizeWindowPerspective(m_Camera, XM_PIDIV4, width/height, 0.01f, 100.0f );
                }
                else
                {
					float width = LOWORD(lParam);
					float height = HIWORD(lParam);
					g_SimeSwapChain->ResizeWindowOrtographic(m_Camera, width, height, 0.01f, 100.0f);
                }
            */
            
            }
            break;
        case WM_KEYDOWN: 
        {
            if (LOWORD(wParam) == 'W')
            {
                cam_pos.SetValues(0.0f, 0.1f, 0.0f);
                m_Camera->MoveCamera(cam_pos);
                m_Camera->UpdateViewMatrix();
            }
            if (LOWORD(wParam) == 'A')
            {
				cam_pos.SetValues(-0.1f, 0.0f, 0.0f);
				m_Camera->MoveCamera(cam_pos);
                m_Camera->UpdateViewMatrix();
            }
			if (LOWORD(wParam) == 'S')
			{
				cam_pos.SetValues(0.0f, -0.1f, 0.0f);
				m_Camera->MoveCamera(cam_pos);
                m_Camera->UpdateViewMatrix();
			}
			if (LOWORD(wParam) == 'D')
			{
				cam_pos.SetValues(0.1f, 0.0f, 0.0f);
				m_Camera->MoveCamera(cam_pos);
                m_Camera->UpdateViewMatrix();
			}
			if (LOWORD(wParam) == 'Q')
			{
				cam_pos.SetValues(0.0f, 0.0f, 0.1f);
				m_Camera->MoveCamera(cam_pos);
                m_Camera->UpdateViewMatrix();
			}
			if (LOWORD(wParam) == 'E')
			{
				cam_pos.SetValues(0.0f, 0.0f, -0.1f);
				m_Camera->MoveCamera(cam_pos);
                m_Camera->UpdateViewMatrix();
			}
            if (wParam == VK_TAB)
            {
                if (m_IsPerspectiveActive)
                {
                    m_Camera = &m_OrtographicCamera;
                    m_IsPerspectiveActive = false;
                }
                else
                {
                    m_Camera = &m_PerspectiveCamera;
                    m_IsPerspectiveActive = true;
                }
            }
            break;
        }
        
        case WM_LBUTTONDOWN:
        {
            m_IsFirstFrame = true;
            break;
        }

        case WM_LBUTTONUP:
        m_IsFirstFrame = false;
        break;
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}

void Update()
{
	// Update our time
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
			m_Camera->RotateCamera(CVector3(Direction->x, Direction->y, 0.0f));
			m_Camera->UpdateViewMatrix();
            m_IsFirstPosStored = false;
            if (Direction != nullptr)
            {
                delete Direction;
            }
        }
    }

	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
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
	g_Translation.m[0][3] = 0;
	g_Translation.m[1][3] = 0;
	g_Translation.m[2][3] = 0;

    //FIRST CUBE
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            FirstCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
            counter++;
        }
    }
    counter = 0;

	g_World = g_Translation;
	g_World = XMMatrixRotationY(t);
    g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FirstCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

    //SECOND CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SecondCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;

    g_World = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
    g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
    g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			SecondCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

    //THIRD CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ThirdCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	g_World = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ThirdCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

    //FOURTH CUBE
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FourthCube.SetWorldMatrixValue(counter, g_Translation.m[i][j]);
			counter++;
		}
	}
	counter = 0;
	g_World = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
	g_World = XMMatrixMultiply(XMMatrixRotationY(t), g_World);
	g_World = XMMatrixMultiply(XMMatrixScaling(0.5f, 0.5f, 0.5f), g_World);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			FourthCube.SetWorldMatrixValue(counter, g_World.m[i][j]);
			counter++;
		}
	}
	counter = 0;

	// Modify the color
	g_vMeshColor.x = (sinf(t * 1.0f) + 1.0f) * 0.5f;
	g_vMeshColor.y = (cosf(t * 3.0f) + 1.0f) * 0.5f;
	g_vMeshColor.z = (sinf(t * 5.0f) + 1.0f) * 0.5f;

	//
	// Clear the back buffer
	//
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	g_SimeDeviceContext->CClearRenderTargetView(g_pRenderTargetView, ClearColor);

	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	g_SimeDeviceContext->CClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	CBNeverChanges cbNeverChanges;
	cbNeverChanges.mView = XMMatrixTranspose(m_Camera->GetViewMatrix());
	g_SimeDeviceContext->CUpdateSubresource(g_pCBNeverChanges, 0, NULL, &cbNeverChanges, 0, 0);

	CBChangeOnResize cbChangesOnResize;
	cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
	g_SimeDeviceContext->CUpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);

	if (m_IsPerspectiveActive)
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetPerspectiveProjectionMatrix());
		g_SimeDeviceContext->CUpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	}
	else
	{
		CBChangeOnResize cbChangesOnResize;
		cbChangesOnResize.mProjection = XMMatrixTranspose(m_Camera->GetOrtographicProjectionMatrix());
		g_SimeDeviceContext->CUpdateSubresource(g_pCBChangeOnResize, 0, NULL, &cbChangesOnResize, 0, 0);
	}
}
//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	UINT stride = sizeof(CVertex);
	UINT offset = 0;

	g_SimeDeviceContext->COMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_SimeDeviceContext->CRSSetViewports(1, &g_Viewport);
    g_SimeDeviceContext->CIASetInputLayout(g_pVertexLayout);

	g_SimeDeviceContext->CIASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_SimeDeviceContext->CIASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_SimeDeviceContext->CIASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //
    // Render the cube
    //
    g_SimeDeviceContext->CVSSetShader( g_pVertexShader, NULL, 0 );
    g_SimeDeviceContext->CVSSetConstantBuffers( 0, 1, &g_pCBNeverChanges );
    g_SimeDeviceContext->CVSSetConstantBuffers( 1, 1, &g_pCBChangeOnResize );
    g_SimeDeviceContext->CVSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_SimeDeviceContext->CPSSetShader( g_pPixelShader, NULL, 0 );
    g_SimeDeviceContext->CPSSetConstantBuffers( 2, 1, &g_pCBChangesEveryFrame );
    g_SimeDeviceContext->CPSSetShaderResources( 0, 1, &g_pTextureRV );
    g_SimeDeviceContext->CPSSetSamplers( 0, 1, &g_pSamplerLinear );


	CBChangesEveryFrame cb;
	cb.mWorld = XMMatrixTranspose(FirstCube.GetWorldMatrix());
	cb.vMeshColor = g_vMeshColor;
	g_SimeDeviceContext->CUpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
    g_SimeDeviceContext->CDrawIndexed(36, 0, 0);

	cb.mWorld = XMMatrixTranspose(SecondCube.GetWorldMatrix());
	cb.vMeshColor = g_vMeshColor;
	g_SimeDeviceContext->CUpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	g_SimeDeviceContext->CDrawIndexed(36, 0, 0);

	cb.mWorld = XMMatrixTranspose(ThirdCube.GetWorldMatrix());
	cb.vMeshColor = g_vMeshColor;
	g_SimeDeviceContext->CUpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	g_SimeDeviceContext->CDrawIndexed(36, 0, 0);

	cb.mWorld = XMMatrixTranspose(FourthCube.GetWorldMatrix());
	cb.vMeshColor = g_vMeshColor;
	g_SimeDeviceContext->CUpdateSubresource(g_pCBChangesEveryFrame, 0, NULL, &cb, 0, 0);
	g_SimeDeviceContext->CDrawIndexed(36, 0, 0);
    //
    // Present our back buffer to our front buffer
    //
    g_SimeSwapChain->CPresent( 0, 0 );
}
