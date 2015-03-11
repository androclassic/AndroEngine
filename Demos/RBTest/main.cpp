#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#ifdef _WIN32

#include <windows.h>
#include "Box.h"
#include "Sphere.h"
#include "Surface.h"
#include "LightPoint.h"
#include "Window.h"
#include "Camera.h"

#else
#include "glxwindow.h"
#endif

#include "../../AndroRenderer/AndroRenderer.h"
#include "../../EventManager/Event.h"
#include "../../force/core/Force.h"
#include "../../EventManager/EventManager.h"



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
	if (!engine->Init("data/ASD.vert", "data/ASD.frag")) //Initialize our game
		{
			MessageBox(NULL, (LPCSTR)"Could not initialize the application", (LPCSTR)"An error occurred", MB_ICONERROR | MB_OK);
			return 1;
		}
	
	force::World* physicsEngine = force::World::GetInstance();


	engine->Resize(windowWidth, windowHeight);
	engine->SetClearColor(0.3,0.6,0.3,0.3);

	Camera mycamera;
	mycamera.GetCamera()->SetPosition(0, 0, 80);
	mycamera.GetCamera()->setPerspectiveMatrix(45.0f, windowWidth / windowHeight, 1.0f, 1000.0f);

	engine->GetScene()->SetCamera(mycamera.GetCamera());
	
	LightPoint lightPoint(andro::Vector3(0,10,70),andro::Vector3(0,0,0));
	lightPoint.EnableShadow(true);

	
	Surface bottom(-20,force::Vector3(0,8,0),andro::Vector3(0,0.0,0.0),andro::Vector2(20,20));
	physicsEngine->AddPlane((force::Plane*)bottom.GetPhysicObject());


	Surface left(-20,force::Vector3(1,0,0),andro::Vector3(0,0.5,0.5),andro::Vector2(20,20));
	physicsEngine->AddPlane((force::Plane*)left.GetPhysicObject());

	Surface right(-20,force::Vector3(-1,0,0),andro::Vector3(0,0.5,0.5),andro::Vector2(20,20));
	physicsEngine->AddPlane((force::Plane*)right.GetPhysicObject());

	Surface back(-20,force::Vector3(0,0,1),andro::Vector3(0.3,0.5,0.5),andro::Vector2(20,20));
	physicsEngine->AddPlane((force::Plane*)back.GetPhysicObject());

	Sphere *sphere[100];
	int spherenr=20;

	std::unique_ptr<force::ParticleGravity> gravity_generator = make_unique<force::ParticleGravity>();

	for(int i=0; i<spherenr; i++)
	{

			sphere[i] = new Sphere(2,force::Vector3((rand() % 200) /10 -10 ,20+i*8, (rand() % 200) /10 -10), true, force::Quaternion(),2);
			physicsEngine->registry.add(sphere[i]->GetPhysicObject()->rigidBody, gravity_generator.get());
			physicsEngine->AddSphere((force::Sphere*)sphere[i]->GetPhysicObject());
	}


	Box *box[100];
	int boxnr=60;
	for(int i=0; i<boxnr; i++)
	{
			force::Quaternion q;
			q.addScaledVector(force::Vector3(rand() % 2,rand() % 2,rand() % 2),DegToRad(rand() %  361));


			box[i] = new Box(force::Vector3(0.5 ,0.5, 0.5)*3.5,force::Vector3((rand() % 200) /10 -10 ,10+i*8,((rand() % 200) /10 -10)),true,true,q,0.3);
			

			physicsEngine->registry.add(box[i]->GetPhysicObject()->rigidBody, gravity_generator.get());
			physicsEngine->AddBox((force::Box*)box[i]->GetPhysicObject());
	}




	DWORD FPS=0;
	TCHAR szFPS[32];
    szFPS[0]='\0';

	double currentTime=0;
	double lastTime=0;
	double lastframeTime=GetTickCount();
	double elapsedTime=0;
	double deltaTime=0;
	//This is the mainloop, we render frames until isRunning returns false
	while (!window.ShouldClose())
    {
        
		FPS++;

		
       currentTime=GetTickCount();
	   elapsedTime=currentTime-lastTime;
	
		deltaTime = (currentTime-lastframeTime)/1000;

        engine->Update(deltaTime); //Do any pre-rendering logic
		window.Update(deltaTime);
		physicsEngine->runPhysics(deltaTime);
//		floor->Update(deltaTime);

	for(int i=0; i<boxnr; i++)
		{
			box[i]->Update(deltaTime);
		}
	
		for(int i=0; i<spherenr; i++)
		{
			sphere[i]->Update(deltaTime);
	
			if(sphere[i]->GetPhysicObject()->rigidBody->GetPosition().z > 60)
			{
				sphere[i]->GetPhysicObject()->rigidBody->SetPosition(force::Vector3((rand() % 200) /10 -10 ,30 + 10+i*8,-4));
				sphere[i]->GetPhysicObject()->rigidBody->SetVelocity(force::Vector3::Z);
			}
		}
	
		andro::EventManager::GetInstance()->Update(deltaTime);
	    if( elapsedTime>=1000)
		{
			
			wsprintf(szFPS, (LPCSTR)"Andro Engine  FPS = %u", (UINT)(FPS * 1000.0 / elapsedTime));
			window.SetTitle(szFPS);
			FPS = 0;
			lastTime=currentTime;
		
		}
		
        double realdt=(currentTime-lastframeTime);
		double normaldt= 1000.0f/60.0f;
		if(realdt < normaldt)
			Sleep( normaldt - realdt);//control fps 60/s

		lastframeTime=currentTime;
	
	}


	//release data

	for (int i = 0; i<boxnr; i++)
		delete box[i];

	for (int i = 0; i < spherenr; i++)
		delete sphere[i];

    return 0; //Return success
}
