
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <fstream>
#include <sstream>

#include "FrameBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define _USE_MATH_DEFINES
#include <math.h>
#include "ObjectsLuaDesc.h"
#include "Scene.h"

#ifdef _USE_CUDA
extern void cudaFreeOjects();
#endif


int m_nFrame = 0;
HDC hdcMem = 0;
HBITMAP hbmMem = 0;
unsigned int s_numberOfSamples;

//#define ONE_FRAME 1

///////////////////////////////////////////////////////////////////////////////////

	void InitFrame(andro::Vector3 frameWHZ, andro::Vector3 bgColour, andro::Vector3 cameraPos, andro::Vector3 cameraLook) //TODO CHECK FOR MEM LEAKS
	{
		CFrameBuffer::GetInstance()->Init(frameWHZ.x, frameWHZ.y, bgColour, cameraPos, cameraLook);
		s_numberOfSamples = frameWHZ.z;
	}


	//////////////////////////////////////////////////////////////////////////////////
	
	ObjectRef<Object> CreateObject(ObjectDesc desc)
	{

		ObjectRef<Object> ref;
		ref.object = CreateFromObjectDesc(desc);

		CScene::GetInstance()->AddObject(ref.object, desc);
		return ref;
	}


	//////////////////////////////////////////////////////////////////////////////////
	void PaintFrameBuffer(HDC hdc)
	{
		m_nFrame++;

		int iWidth = CFrameBuffer::GetInstance()->GetWidth();
		int iHeight = CFrameBuffer::GetInstance()->GetHeight();

		// Create an off-screen DC for double-buffering
		if (!hbmMem)
		{
			hdcMem = CreateCompatibleDC(hdc);
			hbmMem = CreateCompatibleBitmap(hdc, iWidth, iHeight);
		}

		const unsigned int *p = CFrameBuffer::GetInstance()->GetFrameBuffer();

		// Draw back buffer
		HANDLE hOld = SelectObject(hdcMem, hbmMem);

		SetBitmapBits(hbmMem, 4 * iWidth*iHeight, p);

		BitBlt(hdc, 0, 0, iWidth, iHeight, hdcMem, 0, 0, SRCCOPY);

		// Free-up the off-screen DC
		SelectObject(hdcMem, hOld);
	}
	//////////////////////////////////////////////////////////////////////////////////

	void RenderFrame( HDC hdc )
	{

		PaintFrameBuffer(hdc);
	}


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

using namespace andro;

ObjectRef<Vector3> CreateVector3(float x, float y, float z)
{
	ObjectRef<Vector3> obj;
	obj.object = new Vector3(x,y,z);
	return obj;
}

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



	//-----------------------------------------------------------
	//------ game types
	//-----------------------------------------------------------


	REGISTER_USER_TYPE(Object);
	REGISTER_USER_TYPE_REF(Object);
	REGISTER_USER_TYPE(BoxObject);
	REGISTER_USER_TYPE_REF(BoxObject);

	REGISTER_USER_TYPE(Vector3);
	REGISTER_USER_TYPE_REF(Vector3);

	REGISTER_TYPE_EXPLCIT(ObjectDesc, ObjectDesc, ObjectDesc::ToLua, ObjectDesc::FromLua);

	//--------------------------------------
	
	
	//initialise Lua
	Lua_State::GetInstance()->Init();
	lua_State* L = *Lua_State::GetInstance();

	lua_bind_explicit(L, CreateVector3, vec3);

	lua_bind(L, CreateObject);
	lua_bind(L, InitFrame);


	Lua_State::GetInstance()->execute_program("data/raytracer/cornell_box.lua");


	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	HDC hdc = GetDC(g_hWnd);

/*	Object* obj = new MeshObject(new Metal(andro::Vector3(184.0 / 255, 115.0 / 255, 51.0 / 255), 0.3), "data/catmark_torus_creases0.obj", andro::Vector3(0, 0.0, -1));
	m_objects.push_back(obj);*/

	CScene::GetInstance()->InitScene();


	bool should_close = false;

	lastTime = GetTickCount();
	while (!should_close)
	{
		// Main message loop:
		while (PeekMessage(&msg, NULL, 0, 0,TRUE))
		{
			if (msg.message == WM_QUIT)
				return 0;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		andro::EventManager::GetInstance()->Update(deltaTime);



		InvalidateRect(g_hWnd,0,FALSE);

		CFrameBuffer* frameBuffer = CFrameBuffer::GetInstance();
		if (frameBuffer->m_frameCount++ > s_numberOfSamples)
		{

			// write image to log
			const unsigned int* f = frameBuffer->GetFrameBuffer();
			std::ofstream out("ray_log.ppm");

			out << "P3" << std::endl;
			out << frameBuffer->GetWidth() << " " << frameBuffer->GetHeight() << std::endl;
			out << "255 " << std::endl;

			for (int j = 0; j < frameBuffer->GetHeight(); j++)
				for (int i = 0; i < frameBuffer->GetWidth(); i++)
				{
					int r = int(f[j* frameBuffer->GetWidth() + i]) >> 16;
					int g = (int(f[j* frameBuffer->GetWidth() + i]) & 0xff00) >> 8;
					int b = int(f[j* frameBuffer->GetWidth() + i]) & 0xff;
					out << r << " " << g << " " << b << std::endl;
				}

			out.close();
			Sleep(1000);
			should_close = true;
			continue;
		}


		frameBuffer->Update(CScene::GetInstance(), s_numberOfSamples);


		FPS++;
		currentTime = GetTickCount();
		elapsedTime = currentTime - lastTime;
		deltaTime = (currentTime - lastframeTime) / 1000;

		if (elapsedTime >= 100)		
		{
			sprintf(szTitle, (LPCSTR)"FPS = %u MS = %.2f frame : %d", (UINT)(FPS * 1000.0 / elapsedTime), elapsedTime, frameBuffer->m_frameCount);
			SetWindowText(g_hWnd, szTitle);
			FPS = 0;
			lastTime = currentTime;
		}

		lastframeTime = currentTime;

	}

#ifdef _USE_CUDA
	cudaFreeOjects();
#endif

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


	 int width = CFrameBuffer::GetInstance()->GetWidth() + 20;
	 int height = CFrameBuffer::GetInstance()->GetHeight() + 50;
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
	int xPos, yPos;

	switch (message)
	{
	case WM_KEYDOWN:
		andro::EventManager::GetInstance()->AddEvent(new KeyPressedEvent(wParam));
		break;
	case WM_MOUSEMOVE:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		andro::EventManager::GetInstance()->AddEvent(new MouseMove(xPos,yPos));
		break;
	case WM_LBUTTONDOWN:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		andro::EventManager::GetInstance()->AddEvent(new MouseLButtonPressed(xPos, yPos));
		break;
	case WM_LBUTTONUP:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		andro::EventManager::GetInstance()->AddEvent(new MouseLButtonUp(xPos, yPos));
		break;
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
			RenderFrame( hdc );
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
