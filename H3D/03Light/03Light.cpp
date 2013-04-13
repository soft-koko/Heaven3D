#include "stdafx.h"
#include "03Light.h"
#include <windows.h>
#include <mmsystem.h>
#include <TCHAR.h>
#include "h3d_pch.h"
#include "h3d_renderer.h"
#include "h3d_vertex_shader.h"
#include "h3d_pixel_shader.h"
#include "h3d_vertex_buffer.h"
#include "h3d_index_buffer.h"
#include "h3d_constant_buffer.h"
#include "h3d_source_texture.h"
#include "h3d_camera.h"
#include "h3d_vector3.h"
#include "h3d_vector4.h"
#include "h3d_light.h"
#include "h3d_sampler_state.h"
// 类
using namespace Heaven3D;

#define MAX_LOADSTRING 100

struct VertexType
{
	H3DVector3 position;
	H3DVector2 texture;
	H3DVector3 normal;
};

struct MatrixBufferType
{
	H3DMatrix transpose_world_matrix;
	H3DMatrix transpose_view_matrix;
	H3DMatrix transpose_proj_matrix;
};

struct LightBufferType
{
	H3DVector4 diffuse_color;
	H3DVector3 light_direction;
	float padding; 
};

HWND g_hWnd;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

H3DVertexBuffer*	g_vb;
H3DIndexBuffer*		g_ib;
H3DVertexShader*	g_vs;
H3DPixelShader*		g_ps;
H3DSourceTexture*	g_texture;
H3DCamera*			g_camera;
H3DLight*			g_light;
H3DConstantBuffer*  g_matrix_buf;
H3DConstantBuffer*  g_light_buf;
H3DSamplerState*	g_sampler;
LightBufferType		g_light_buf_type;
MatrixBufferType	g_matrix_buf_type;
H3DMatrix			g_world_matrix;
float				g_rotation_angle = 0.f;

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void InitGeometry();
void Shutdown();
void render();
void Shutdown();


void InitGeometry()
{
	H3DRenderer::GetSingletonPtr()->Initialize( 640 , 480 , true , g_hWnd , false , 1000.f,-10.f );

	g_vb = new H3DVertexBuffer;

	VertexType vertices[3];
	vertices[0].position = H3DVector3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].texture = H3DVector2(0.0f, 1.0f);
	vertices[0].normal = H3DVector3(0.0f, 0.0f, -1.0f);

	vertices[1].position = H3DVector3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].texture = H3DVector2(0.5f, 0.0f);
	vertices[1].normal = H3DVector3(0.0f, 0.0f, -1.0f);

	vertices[2].position = H3DVector3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].texture = H3DVector2(1.0f, 1.0f);
	vertices[2].normal = H3DVector3(0.0f, 0.0f, -1.0f);

	g_vb->Create(3, sizeof(VertexType) , 0 , 
		D3D11_USAGE_DEFAULT ,  D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST  , vertices);

//==========================================

	g_ib = new H3DIndexBuffer;

	uint32_t indices[3];
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.
	g_ib->Create( 3 , indices );

//==========================================

	g_light = new H3DLight;
	g_light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	g_light->SetDirection(0.0f, 0.0f, 1.0f);

//==========================================

	g_camera = new H3DCamera;
	g_camera->SetLookAt( H3DVector3(0.f,0.f,-2.f) , 
		H3DVector3(0.f,0.f,0.f) , H3DVector3(0.f,1.f,0.f) );

//=========================================
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	uint32_t numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	g_vs = new H3DVertexShader;
	g_vs->Create(_T("lighting.fx"),_T("LightVertexShader"), _T("vs_5_0"),polygonLayout , numElements );

//=========================================

	g_ps = new H3DPixelShader;
	g_ps->Create(_T("lighting.fx"),_T("LightPixelShader"), _T("ps_5_0"));

//=========================================
	 
	g_light_buf_type.diffuse_color = g_light->GetDiffuseColor();
	g_light_buf_type.light_direction = g_light->GetDirection();
	g_light_buf_type.padding = 0.f;
	g_light_buf = new H3DConstantBuffer;
	g_light_buf->Create( &g_light_buf_type , sizeof(LightBufferType) , D3D11_USAGE_DYNAMIC );

//=========================================
	g_world_matrix.Identity();
	g_matrix_buf_type.transpose_world_matrix = g_world_matrix;
	g_matrix_buf_type.transpose_world_matrix.Transpose();

	g_matrix_buf_type.transpose_view_matrix = g_camera->GetViewMatrix();
	g_matrix_buf_type.transpose_view_matrix.Transpose();

	g_matrix_buf_type.transpose_proj_matrix.PerspectiveFovLH( XM_PIDIV2, 640.f / 480.f, 0.1f, 100.0f );
	g_matrix_buf_type.transpose_proj_matrix.Transpose();

	g_matrix_buf = new H3DConstantBuffer;
	g_matrix_buf->Create( &g_matrix_buf_type , sizeof(MatrixBufferType) , D3D11_USAGE_DYNAMIC );

//=========================================
	g_texture = new H3DSourceTexture;
	g_texture->CreateFromFile(_T("seafloor.dds"));
//=========================================
	D3D11_SAMPLER_DESC sampler_desc;
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = 0;
	sampler_desc.BorderColor[1] = 0;
	sampler_desc.BorderColor[2] = 0;
	sampler_desc.BorderColor[3] = 0;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	g_sampler = new H3DSamplerState;
	g_sampler->Create( &sampler_desc );
}

void Shutdown()
{
	H3D_SAFE_DELETE(g_vb);
	H3D_SAFE_DELETE(g_ib);
	H3D_SAFE_DELETE(g_texture);
	H3D_SAFE_DELETE(g_camera);
	H3D_SAFE_DELETE(g_light);
	H3D_SAFE_DELETE(g_matrix_buf);
	H3D_SAFE_DELETE(g_light_buf);
	H3D_SAFE_DELETE(g_ps);
	H3D_SAFE_DELETE(g_vs);
	H3D_SAFE_DELETE(g_sampler);
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASSEX winClass;
	MSG        uMsg;

	memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = _T("MY_WINDOWS_CLASS");
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WndProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, _T("MY_WINDOWS_CLASS"), 
		_T("Heaven 3D Framework based on DX11 - 03Lighting"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 640, 480, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

	InitGeometry();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
		{
			g_rotation_angle += XM_PI / 360.f;
			
			if( g_rotation_angle > XM_2PI )
			{
				g_rotation_angle -= XM_2PI;
			}

			g_world_matrix.RotationY(g_rotation_angle);
			render();
		}
	}

	Shutdown();

	UnregisterClass( _T("MY_WINDOWS_CLASS"), winClass.hInstance );

	return uMsg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY03LIGHT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY03LIGHT);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   InitGeometry();

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void render()
{
	g_matrix_buf_type.transpose_world_matrix = g_world_matrix;
	g_matrix_buf_type.transpose_world_matrix.Transpose();
	
	H3DRenderer::GetSingletonPtr()->BeginScene(0.0f, 0.125f, 0.3f, 1.0f);

	ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
	ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();

	g_camera->Update();

	g_vb->Set();
	g_ib->Set();
	g_vs->Set();
	g_ps->Set();

	g_matrix_buf->UpdateBufferData(&g_matrix_buf_type, sizeof(MatrixBufferType));

	g_matrix_buf->SetForVertexShader();
	g_light_buf->SetForPixelShader();
	g_sampler->Set();

	ID3D11ShaderResourceView* tex = g_texture->GetTexture();
	device_context->PSSetShaderResources(0, 1, &tex);
	device_context->DrawIndexed( g_ib->GetIndexCount(), 0, 0 );

	H3DRenderer::GetSingletonPtr()->EndScene();
}