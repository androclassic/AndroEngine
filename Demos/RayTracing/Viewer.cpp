
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


std::vector<Object*> m_objects;
andro::OctreeNode<Object*>* m_octree;
andro::BoundingBox m_scene_bbx;
int m_nFrame = 0;
HDC hdcMem = 0;
HBITMAP hbmMem = 0;
unsigned int s_numberOfSamples;

CFrameBuffer* g_Framebuffer = NULL;

//#define ONE_FRAME 1
//////////////////////////////////////////////////////////////////////////////////
	void DestroyObjects()
	{
		delete g_Framebuffer;

		for (auto obj : m_objects)
		{
			obj->m_material->Destroy();
			delete obj->m_material;
			delete obj;
		}
		delete m_octree;
	}
///////////////////////////////////////////////////////////////////////////////////

	void InitFrame(andro::Vector3 frameWHZ, andro::Vector3 bgColour, andro::Vector3 cameraPos, andro::Vector3 cameraLook) //TODO CHECK FOR MEM LEAKS
	{
		g_Framebuffer = new CFrameBuffer(frameWHZ.x, frameWHZ.y, bgColour, cameraPos, cameraLook);
		s_numberOfSamples = frameWHZ.z;
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
	void InitScene()
	{

		// buid scene bbx
		afloat min_radius = 10000;


		for (auto object : m_objects)
		{
#ifdef OBJECT_LIST_DEBUG_TEST
			g_Framebuffer->debug_objects.push_back(object);
#endif

			andro::Sphere bsphere = object->GetBoundingSphere();

			min_radius = fminf(min_radius, bsphere.radius);
			for (int a = 0; a < 3; a++)
			{

				m_scene_bbx.min[a] = fminf(m_scene_bbx.min[a], bsphere.center[a] - bsphere.radius);
				m_scene_bbx.max[a] = fmaxf(m_scene_bbx.max[a], bsphere.center[a] + bsphere.radius);
			}
		}

		BuildSceneOctree();
	}

	//////////////////////////////////////////////////////////////////////////////////
	
	ObjectRef<Object> CreateObject(ObjectDesc desc)
	{
		ObjectRef<Object> ref;
		Object* object = nullptr;
		material* mat = nullptr;
		texture* tex = nullptr;

		if (desc.m_material == MaterialType::M_Lambertian)
		{

			if (desc.m_texture == TextureType::Tex_Constant)
				tex = new constant_texture(desc.m_colour);
			else
				tex = new noise_texture(desc.m_colour);

			mat = new Lambertian(tex);
		}
		else if (desc.m_material == MaterialType::M_LIGHT)
		{
			if (desc.m_texture == TextureType::Tex_Constant)
				tex = new constant_texture(desc.m_colour);
			else
				tex = new noise_texture(desc.m_colour);

			mat = new diffuse_light(tex);
		}
		else if (desc.m_material == MaterialType::M_Metal)
		{
			mat = new Metal(desc.m_colour, desc.m_roughness);
		}
		else if (desc.m_material == MaterialType::M_Dielectric)
		{
			mat = new Dielectric(desc.m_roughness); //refractive index
		}
		else
			ASSERT(false);

		//------------------------------------------------------------
		if (desc.m_type == ObjectType::OBJ_Sphere)
		{
			object = new SphereObject(mat, desc.m_position, desc.m_size.x);
		}
		else if (desc.m_type == ObjectType::OBJ_Box)
		{
			object = new BoxObject(mat, desc.m_position, desc.m_size);
		}
		else if (desc.m_type == ObjectType::OBJ_RectObjectXY)
		{
			Vector2 size(desc.m_size.x, desc.m_size.y);
			object = new RectObject(mat, desc.m_position, size, RectObjectType::XY);
		}
		else if (desc.m_type == ObjectType::OBJ_RectObjectYZ)
		{
			Vector2 size(desc.m_size.x, desc.m_size.y);
			object = new RectObject(mat, desc.m_position, size, RectObjectType::YZ);
		}
		else if (desc.m_type == ObjectType::OBJ_RectObjectXZ)
		{
			Vector2 size(desc.m_size.x, desc.m_size.y);
			object = new RectObject(mat, desc.m_position, size, RectObjectType::XZ);
		}
		else
			ASSERT(false);


		ref.object = object;
		m_objects.push_back(ref.object);

		return ref;

	}

	//////////////////////////////////////////////////////////////////////////////////
	void PaintFrameBuffer(HDC hdc)
	{
		m_nFrame++;

		int iWidth = g_Framebuffer->GetWidth();
		int iHeight = g_Framebuffer->GetHeight();

		// Create an off-screen DC for double-buffering
		if (!hbmMem)
		{
			hdcMem = CreateCompatibleDC(hdc);
			hbmMem = CreateCompatibleBitmap(hdc, iWidth, iHeight);
		}

		const unsigned int *p = g_Framebuffer->GetFrameBuffer();

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

	Lua_State::GetInstance()->execute_program("data/raytracer/raytracer.lua");



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		DestroyObjects();
		return FALSE;
	}
	HDC hdc = GetDC(g_hWnd);

/*	Object* obj = new MeshObject(new Metal(andro::Vector3(184.0 / 255, 115.0 / 255, 51.0 / 255), 0.3), "data/catmark_torus_creases0.obj", andro::Vector3(0, 0.0, -1));
	m_objects.push_back(obj);*/

	InitScene();


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

#if defined ONE_FRAME
		static int s = 0;
		if (s == 1)
		{

			// write image to log
			const unsigned int* f = g_Framebuffer->GetFrameBuffer();
			std::ofstream out("ray_log.txt");

			out << "P3" << std::endl;
			out << g_Framebuffer->GetWidth() << " " << g_Framebuffer->GetHeight() << std::endl;
			out << "255 " << std::endl;

			for (int j = 0; j < g_Framebuffer->GetHeight(); j++)
				for (int i = 0; i < g_Framebuffer->GetWidth(); i++)
				{
					int r = int(f[j* g_Framebuffer->GetWidth() + i]) >> 16;
					int g = (int(f[j* g_Framebuffer->GetWidth() + i]) & 0xff00) >> 8;
					int b = int(f[j* g_Framebuffer->GetWidth() + i]) & 0xff;
					out << r << " " << g << " " << b << std::endl;
				}

			out.close();
			Sleep(1000);
//			should_close = true;
			continue;
		}

		s++;
#endif

		g_Framebuffer->Update(m_octree, s_numberOfSamples);



		FPS++;
		currentTime = GetTickCount();
		elapsedTime = currentTime - lastTime;
		deltaTime = (currentTime - lastframeTime) / 1000;

		if (elapsedTime >= 100)		
		{
			sprintf(szTitle, (LPCSTR)"FPS = %u MS = %.2f", (UINT)(FPS * 1000.0 / elapsedTime), elapsedTime);
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


	 int width = g_Framebuffer->GetWidth() + 20;
	 int height = g_Framebuffer->GetHeight() + 50;
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
		DestroyObjects();

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
