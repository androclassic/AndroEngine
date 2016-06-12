
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

#include "FrameBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define _USE_MATH_DEFINES
#include <math.h>
#include "Material.h"
#include "AndroUtils/Utils/Octree.h"
#include "AndroUtils/Utils/Ray.h"
#include "AndroUtils/Utils/Shapes.h"
#include "AndroUtils/Introspection/LuaState.h"


std::vector<Object*> m_objects;
andro::OctreeNode<Object*>* m_octree;
andro::BoundingBox m_scene_bbx;
int m_nFrame = 0;
HDC hdcMem = 0;
HBITMAP hbmMem = 0;
bool m_init = false;
CFrameBuffer g_Framebuffer(300, 300);


//#define ONE_FRAME 1

//////////////////////////////////////////////////////////////////////////////////
	void DestroyObjects()
	{
		for (auto obj : m_objects)
		{
			delete obj->m_material;
			delete obj;
		}
		delete m_octree;
	}

	//////////////////////////////////////////////////////////////////////////////////
	void InitScene()
	{
		m_init = true;
		int w, h, n = 0;
		unsigned char* data = stbi_load("data/doza.jpg", &w, &h, &n, 0);
		ASSERT(data != nullptr);
		if (data == nullptr) TRACE(L"ERROR LOADING FILE");

		float light_intensity = 5;
		m_objects.push_back(new RectObject(new diffuse_light(new constant_texture(andro::Vector3(1, 1, 1)*light_intensity)), andro::Vector3(-2, 4, 2), andro::Vector2(0.7, 0.7), RectObjectType::XZ));
		m_objects.push_back(new RectObject(new diffuse_light(new constant_texture(andro::Vector3(1, 1, 1)*light_intensity)), andro::Vector3(2, 4, -2), andro::Vector2(0.7, 0.7), RectObjectType::XZ));
		m_objects.push_back(new RectObject(new diffuse_light(new constant_texture(andro::Vector3(1, 1, 0)*light_intensity)), andro::Vector3(-2, 4, -2), andro::Vector2(0.7, 0.7), RectObjectType::XZ));
		m_objects.push_back(new RectObject(new diffuse_light(new constant_texture(andro::Vector3(1, 1, 0)*light_intensity)), andro::Vector3(2, 4, 2), andro::Vector2(0.7, 0.7), RectObjectType::XZ));
		m_objects.push_back(new SphereObject(new Lambertian(new noise_texture(andro::Vector3(0.9, 0.9, 0.0))), andro::Vector3(0, -1000, -1), 1000));
		int i = 1;
		for (int a = -11; a < 11; a++)
		{
			for (int b = -11; b < 11; b++)
			{
				float choose_mat = random_float(1);
				Vector3 center(a + 0.9* random_float(1), 0.2, b * random_float(1));
				if ((center - Vector3(4, 0.2, 0)).Lenght() > 0.9)
				{
					if (choose_mat < 0.8) //difuse
						m_objects.push_back(new SphereObject(new Lambertian(new constant_texture(Vector3(random_float(1)* random_float(1), random_float(1)* random_float(1), random_float(1)* random_float(1)))), center, 0.2));
					else if (choose_mat < 0.95) //metal
						m_objects.push_back(new SphereObject(new Metal(Vector3(0.5*(1 + random_float(1)), 0.5*(1 + random_float(1)), 0.5*(1 + random_float(1))), 0.5*random_float(1)), center, 0.2));
					else
						m_objects.push_back(new SphereObject(new Dielectric(1.5), center, 0.2));

				}
			}


		}
		m_objects.push_back(new SphereObject(new Dielectric(1.5), Vector3(0, 1, 0.5), 1.0));
		m_objects.push_back(new SphereObject(new Lambertian(new  constant_texture(Vector3(0.4, 0.2, 0.1))), Vector3(-4, 1.0, 1.0), 1));
//		m_objects.push_back(new BoxObject(new Metal(Vector3(0.7, 0.6, 0.2), 0.05), Vector3(4, 1.0, 0.0), Vector3(1)));

		// buid scene bbx
		float min_radius = 10000;


		for (auto object : m_objects)
		{
#ifdef OBJECT_LIST_DEBUG_TEST
			g_Framebuffer.debug_objects.push_back(&sphere);
#endif

			andro::Sphere bsphere = object->GetBoundingSphere();

			min_radius = fminf(min_radius, bsphere.radius);
			for (int a = 0; a < 3; a++)
			{

				m_scene_bbx.min[a] = fminf(m_scene_bbx.min[a], bsphere.center[a] - bsphere.radius);
				m_scene_bbx.max[a] = fmaxf(m_scene_bbx.max[a], bsphere.center[a] + bsphere.radius);
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////////////

	void BuildSceneOctree()
	{ 
		// build octree of the scene
		m_octree = andro::BuildOctree<Object*>(m_objects, m_scene_bbx, 21, [](const andro::BoundingBox& box, Object* t)
		{
			// check in sphere is enclosed
			andro::Sphere bsphere = t->GetBoundingSphere();

			andro::Vector3 v = (bsphere.center - box.GetCenter());
			andro::Vector3 halfsize = box.GetHalfSize();

			if ((halfsize.x > std::fabs(v.x) + bsphere.radius) &&
				(halfsize.y > std::fabs(v.y) + bsphere.radius) &&
				(halfsize.z > std::fabs(v.z) + bsphere.radius))
				return true;

			return false;

		}, true);

	}
	//////////////////////////////////////////////////////////////////////////////////
	void PaintFrameBuffer(HDC hdc)
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
		HANDLE hOld = SelectObject(hdcMem, hbmMem);

		SetBitmapBits(hbmMem, 4 * iWidth*iHeight, p);

		BitBlt(hdc, 0, 0, iWidth, iHeight, hdcMem, 0, 0, SRCCOPY);

		// Free-up the off-screen DC
		SelectObject(hdcMem, hOld);
	}
	//////////////////////////////////////////////////////////////////////////////////

	void RenderFrame( HDC hdc )
	{

#if defined ONE_FRAME
		static int s = 0;
		s++;
		if (s > 1)
		{
			PaintFrameBuffer(hdc);
			return;
		}
#endif

		if (m_init)
			g_Framebuffer.Update(m_octree);

		PaintFrameBuffer(hdc);
	}

//////////////////////////////////////////////////////////////////////////////////


	

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


ObjectRef<Object> CreateObject()
{
	ObjectRef<Object> ref;

	ref.object = new BoxObject(new Metal(Vector3(0.7, 0.6, 0.2), 0.05), Vector3(4, 1.0, 0.0), Vector3(1));
	m_objects.push_back(ref.object);

	return ref;

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

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}


	HDC hdc = GetDC(g_hWnd);

	//-----------------------------------------------------------
	//------ game types
	//-----------------------------------------------------------


	REGISTER_USER_TYPE(Object);
	REGISTER_USER_TYPE_REF(Object);
	REGISTER_USER_TYPE(BoxObject);
	REGISTER_USER_TYPE_REF(BoxObject);
	//--------------------------------------
	
	
	//initialise Lua
	Lua_State::GetInstance()->Init();
	lua_State* L = *Lua_State::GetInstance();
	lua_bind(L, CreateObject);

	Lua_State::GetInstance()->execute_program("data/raytracer/raytracer.lua");

	InitScene();
	BuildSceneOctree();

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

		andro::EventManager::GetInstance()->Update(deltaTime);



		InvalidateRect(g_hWnd,0,FALSE);

#if defined ONE_FRAME
		static int st = 0;
		st++;
		if (st == 1)
#else
		if (elapsedTime >= 100)
#endif
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
