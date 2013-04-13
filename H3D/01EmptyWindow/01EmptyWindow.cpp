/********************************************************************
	created:	2011/05/12
	created:	12:5:2011   0:32
	filename: 	e:\Work\Heaven3D\01EmptyWindow\01EmptyWindow.cpp
	file path:	e:\Work\Heaven3D\01EmptyWindow
	file base:	01EmptyWindow
	file ext:	cpp
	author:		Xiong Xinke
	
	purpose:	
*********************************************************************/

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include <TCHAR.h>
#include "h3d_pch.h"
#include "h3d_renderer.h"

using namespace Heaven3D;

HWND g_hWnd          = NULL;

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
		_T("Heaven 3D Framework based on DX11 - 01EmptyWindow"),
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

LRESULT CALLBACK WindowProc( HWND   hWnd, 
							UINT   msg, 
							WPARAM wParam, 
							LPARAM lParam )
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

	case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

	default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

void init( void )
{
	H3DRenderer::GetSingletonPtr()->Initialize( 640 , 480 , true , g_hWnd , false , 1000.f,-10.f );
}

void shutDown( void )
{
	H3DRenderer::GetSingletonPtr()->Shutdown();
}

void render( void )
{
	H3DRenderer::GetSingletonPtr()->BeginScene(0.0f, 0.125f, 0.3f, 1.0f);
	H3DRenderer::GetSingletonPtr()->EndScene();
}
