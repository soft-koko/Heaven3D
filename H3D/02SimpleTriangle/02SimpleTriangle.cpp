///********************************************************************
//	created:	2012/03/05
//	created:	5:3:2012   0:19
//	filename: 	e:\Work\Heaven3D\02SimpleTriangle\02SimpleTriangle.cpp
//	author:		Xiong Xinke
//	
//	purpose:	
//*********************************************************************/
#define STRICT
#define WIN32_LEAN_AND_MEAN

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
#include "h3d_camera.h"
#include "h3d_vector3.h"
#include "h3d_vector4.h"

using namespace Heaven3D;

struct VertexType
{
	XMFLOAT3 position;
	XMFLOAT4 color;
};

HWND				g_hWnd				= NULL;
H3DVertexBuffer*	g_vertex_buffer		= NULL;
H3DIndexBuffer*		g_index_buffer		= NULL;
H3DCamera*			g_camera			= NULL;
H3DVertexShader*	g_vertex_shader		= NULL;
H3DPixelShader*		g_pixel_shader		= NULL;
H3DConstantBuffer*	g_const_buffer		= NULL;
H3DMatrix			g_world_mat;
H3DMatrix			g_view_mat;
H3DMatrix			g_proj_mat;
H3DMatrix			wvp_matrix[3];


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void loadTexture();
void init();
void shutDown();
void render();

int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow )
{
	WNDCLASSEX winClass;
	MSG        uMsg;

	memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = _T("MY_WINDOWS_CLASS");
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
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
		_T("Heaven 3D Framework based on DX11 - 02SimpleTriangle"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 640, 480, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
		{
			render();
		}
	}

	shutDown();

	UnregisterClass( _T("MY_WINDOWS_CLASS"), winClass.hInstance );

	return uMsg.wParam;
}

LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{	
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}
		}
		break;

	case WM_CLOSE:PostQuitMessage(0);	
	case WM_DESTROY:PostQuitMessage(0);
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
		break;
	}

	return 0;
}

void init( void )
{
	H3DRenderer::GetSingletonPtr()->Initialize( 640 , 480 , true , g_hWnd , false , 1000.f,-10.f );

	g_vertex_buffer = new H3DVertexBuffer;
	g_index_buffer = new H3DIndexBuffer;
	
	/*
	//== 创建顶点缓冲
	VertexType vertices[3];
	vertices[0].position = XMFLOAT3(-1.0f, 0.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(1.0f, 0.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(0.0f, 2.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	*/

	VertexType vertices[] =
	{
		{ XMFLOAT3( -1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, 1.0f, 1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, 1.0f, 1.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f ), XMFLOAT4( 1.0f, 1.0f, 0.0f, 1.0f ) },
		{ XMFLOAT3( 1.0f, -1.0f, 1.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
		{ XMFLOAT3( -1.0f, -1.0f, 1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) },
	};

	g_vertex_buffer->Create( 8 , sizeof(VertexType) , 0 , D3D11_USAGE_DEFAULT 
		, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST , vertices );
	
	// == 创建索引缓冲
	
	uint32_t indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	g_index_buffer->Create( 36 , indices );

	// == 创建摄像机
	g_camera = new H3DCamera( H3DVector3(0.0f, 0.0f, -10.0f) ,  H3DVector3(0.0f, 0.0f, 0.f) );

	g_vertex_shader = new H3DVertexShader;
	g_pixel_shader = new H3DPixelShader;

	//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	D3D11_INPUT_ELEMENT_DESC input_elem_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	uint32_t input_elem_num = sizeof(input_elem_desc) / sizeof(input_elem_desc[0]);
	g_vertex_shader->Create( _T("tutorial04.fx") , _T("VS"),_T("vs_4_0"),input_elem_desc , input_elem_num);
	g_pixel_shader->Create(_T("tutorial04.fx"),_T("PS"),_T("ps_4_0"));

	g_world_mat.Identity();

	H3DVector3 eye = H3DVector3( 0.0f, 1.0f, -5.0f );
	H3DVector3 at = H3DVector3( 0.0f, 1.0f, 0.0f);
	H3DVector3 up = H3DVector3( 0.0f, 1.0f, 0.0f);
	//g_view_mat.LookAtLH( eye , at , up );

	g_camera->SetLookAt(eye,at,up);
	g_camera->Update();


	g_proj_mat.PerspectiveFovLH( XM_PIDIV2, 640.f / 480.f, 0.1f, 100.0f);

	wvp_matrix[0] = g_world_mat;
	wvp_matrix[1] = g_camera->GetViewMatrix();
	wvp_matrix[2] = g_proj_mat;

	wvp_matrix[0].Transpose();
	wvp_matrix[1].Transpose();
	wvp_matrix[2].Transpose();

	g_const_buffer = new H3DConstantBuffer;
	g_const_buffer->Create( wvp_matrix , sizeof(H3DMatrix)*3 );
}

void shutDown( void )
{
	H3DRenderer::GetSingletonPtr()->Shutdown();
	H3DRenderer::DestroySingleton();
}

void render( void )
{
	H3DRenderer::GetSingletonPtr()->BeginScene(0.0f, 0.125f, 0.3f, 1.0f);

	ID3D11Device* device = H3DRenderer::GetSingletonPtr()->GetDevice();
	ID3D11DeviceContext* device_context = H3DRenderer::GetSingletonPtr()->GetDeviceContext();

	g_camera->Update();

	g_vertex_buffer->Set();
	g_index_buffer->Set();
	g_vertex_shader->Set();
	g_pixel_shader->Set();
	
	g_const_buffer->SetForVertexShader();
	device_context->DrawIndexed( g_index_buffer->GetIndexCount(), 0, 0 );

	H3DRenderer::GetSingletonPtr()->EndScene();
}