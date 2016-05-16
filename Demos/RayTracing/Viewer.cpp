
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "FrameBuffer.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "AndroUtils\Utils\Ray.h"
#include "Material.h"

CFrameBuffer g_Framebuffer(384, 256);
Lambertian mat_Red(andro::Vector3(0.5, 0.0, 0.0));
Lambertian mat_blue(andro::Vector3(0.0, 0.0, 0.5));
Metal metal_green(andro::Vector3(0.3, 1.0f, 0.3));
Metal metal_blue(andro::Vector3(0.3f, 0.3f, 1.0));

//////////////////////////////////////////////////////////////////////////////////
class CViewer
{
public:
	CViewer()
	{
		hbmMem = 0;
		hdcMem = 0;
		m_nFrame = 0;

		spheres.push_back(Object(&metal_green, andro::Vector3(-1.0f, 0, -2), 0.5));
		spheres.push_back(Object(&mat_Red,     andro::Vector3( 0  , 0, -2), 0.5));
		spheres.push_back(Object(&metal_blue,  andro::Vector3( 1.0f, 0, -2), 0.5));
		spheres.push_back(Object(&mat_blue,    andro::Vector3(0, -100.5, -1), 100));

	}

	void RenderFrame( HDC hdc )
	{
		g_Framebuffer.Update(spheres);
		PaintFrameBuffer(hdc);
	}

private:
	void PaintFrameBuffer( HDC hdc )
	{
		m_nFrame++;

		int iWidth = g_Framebuffer.GetWidth();
		int iHeight = g_Framebuffer.GetHeight();

		// Create an off-screen DC for double-buffering
		if (!hbmMem)
		{
			hdcMem = CreateCompatibleDC(hdc);
			hbmMem = CreateCompatibleBitmap(hdc, iWidth, iHeight);
		}

		const unsigned int *p = g_Framebuffer.GetFrameBuffer();

		// Draw back buffer
		HANDLE hOld   = SelectObject(hdcMem, hbmMem);

		SetBitmapBits(hbmMem,4*iWidth*iHeight,p);

		//////////////////////////////////////////////////////////////////////////////////
		// Display FPS
		//////////////////////////////////////////////////////////////////////////////////
		//float fps = (float)(1000.f/m_averageFrameTime);

		//char str[1024];
		//sprintf_s( str,"Frame:%d:  fps:%.1f,  %.2fms",m_nFrame,fps,m_averageFrameTime );
		//TextOut(hdcMem,0,0,str,(int)strlen(str) );

		//sprintf_s( str,"Turn Time: %.2f sec",(float)(m_lastFullRotationTime/1000.0) );
		//TextOut(hdcMem,0,16,str,(int)strlen(str) );
		//////////////////////////////////////////////////////////////////////////////////

		// Transfer the off-screen DC to the screen
		BitBlt(hdc, 0, 0, iWidth, iHeight, hdcMem, 0, 0, SRCCOPY);

		// Free-up the off-screen DC
		SelectObject(hdcMem, hOld);
	}
	
private:
	int m_nFrame;
	HDC hdcMem;
	HBITMAP hbmMem;
	std::vector<Object> spheres;

};

CViewer g_viewer;

//////////////////////////////////////////////////////////////////////////////////
#define MAX_LOADSTRING 100
// Global Variables:
HWND g_hWnd = 0;
HINSTANCE hInst;								// current instance
TCHAR   szTitle[256] = "RAYTRACER";				// The title bar text
TCHAR* szWindowClass = "RAYTRACER";				// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;

	DWORD FPS = 0;
	TCHAR szFPS[32];
	szFPS[0] = '\0';

	double currentTime = 0;
	double lastTime = 0;
	double lastframeTime = GetTickCount();
	double elapsedTime = 0;
	double deltaTime = 0;

	// Initialize global strings
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}


	HDC hdc = GetDC(g_hWnd);

	while (true)
	{
		FPS++;
		currentTime = GetTickCount();
		elapsedTime = currentTime - lastTime;
		deltaTime = (currentTime - lastframeTime) / 1000;




		// Main message loop:
		while (PeekMessage(&msg, NULL, 0, 0,TRUE))
		{
			if (msg.message == WM_QUIT)
				return 0;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		InvalidateRect(g_hWnd,0,FALSE);
		if (elapsedTime >= 100)
		{
			
			sprintf(szTitle, (LPCSTR)"FPS = %u MS = %.2f", (UINT)(FPS * 1000.0 / elapsedTime), (float)(currentTime - lastframeTime));
			SetWindowText(g_hWnd, szTitle);
			FPS = 0;
			lastTime = currentTime;

		}

		lastframeTime = currentTime;

	}

	ReleaseDC(g_hWnd,hdc);

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable


	 int width = g_Framebuffer.GetWidth() + 20;
	 int height = g_Framebuffer.GetHeight() + 50;
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, width,height, 0, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

	 g_hWnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			g_viewer.RenderFrame( hdc );
			EndPaint(hWnd, &ps);
		}
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
