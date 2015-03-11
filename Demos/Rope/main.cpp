#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#ifdef _WIN32

#include <windows.h>
#include "Entity.h"

#else
#include "glxwindow.h"
#endif

#include "../../AndroRenderer/AndroRenderer.h"
#include "../../EventManager/Event.h"
#include "../../force/core/Force.h"

#include "../Window.h"

using namespace std;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{
	
#else
int main(int argc, char** argv)
{
#endif
    //Set our window settings
    const int windowWidth = 1080;
    const int windowHeight = 720;
    const int windowBPP = 16;
    const int windowFullscreen = 0;
	srand(time(0));


	Window window(windowWidth, windowHeight);


		
	andro::Engine* engine = andro::Engine::GetInstance();
	force::ParticleWorld& physicsEngine = *force::ParticleWorld::GetInstance();

	if (!engine->Init("data/basic-fixed.vert", "data/basic-fixed.frag")) //Initialize our game
		{
			MessageBox(NULL, (LPCSTR)"Could not initialize the application", (LPCSTR)"An error occurred", MB_ICONERROR | MB_OK);
			return 1;
		}
	engine->Resize(windowWidth, windowHeight);
	
	Entity entity(andro::Vector3(20,0,-50),1,0.7);
	 
	 
	shared_ptr<andro::Camera>  camera = make_shared<andro::Camera>();
	camera->SetPosition(0, 0, 20);
	camera->setPerspectiveMatrix(45.0f, windowWidth / windowHeight, 1.0f, 1000.0f);
	engine->GetScene()->SetCamera(camera);


	 
	DWORD FPS=0;
	TCHAR title[32];
	title[0] = '\0';

	double currentTime=0;
	double lastTime=0;
	double lastframeTime=GetTickCount();
	double elapsedTime=0;
	double deltaTime=0;
	int x=1;
	//This is the mainloop, we render frames until isRunning returns false
	while (!window.ShouldClose())
    {
        
		FPS++;

		
	    currentTime=GetTickCount();
	    elapsedTime=currentTime-lastTime;
	
		deltaTime = (currentTime-lastframeTime)/1000;

        engine->Update(deltaTime); //Do any pre-rendering logic
		physicsEngine.runPhysics(deltaTime);
		entity.Update(deltaTime);
		andro::EventManager::GetInstance()->Update(deltaTime);
		window.Update(deltaTime);


		
		if (elapsedTime >= 1000)
		{
			wsprintf(title, (LPCSTR)"Andro Engine  FPS = %u", (UINT)(FPS * 1000.0 / elapsedTime));
			window.SetTitle(title);
			FPS = 0;
			lastTime=currentTime;
		
		}
		
        double realdt=(currentTime-lastframeTime);
		double normaldt= 1000.0f/60.0f;
		if(realdt < normaldt)
			Sleep( normaldt - realdt);//control fps 60/s

		lastframeTime=currentTime;
	
	}

    return 0; //Return success
}
