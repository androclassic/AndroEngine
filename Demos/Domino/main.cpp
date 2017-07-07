#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one

#include <windows.h>
#include "Surface.h"
#include "Window.h"
#include "Camera.h"
#include "Spawner.h"


#include "../../AndroRenderer/AndroRenderer.h"
#include "../../EventManager/Event.h"
#include "../../force/core/Force.h"
#include "../../EventManager/EventManager.h"
#include "LightPoint.h"
#include "Rope.h"


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
    const int windowWidth = 1024;
    const int windowHeight = 768;
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
	LightPoint lightPoint(andro::Vector3(30, 20, 110), andro::Vector3(0, -10, 30));
	lightPoint.EnableShadow(true);

	engine->Resize(windowWidth, windowHeight);

	engine->SetClearColor(0.3,0.3,0.3,0.3);
	Camera mycamera;
	mycamera.GetCamera()->LookAt(andro::Vector3(0, 0, 80), andro::Vector3(10, -10, 0));
	mycamera.GetCamera()->setPerspectiveMatrix(45.0f, windowWidth / windowHeight, 1.0f, 1000.0f);
	engine->GetScene()->SetCamera(mycamera.GetCamera());



	Spawner spawner(engine->GetScene()->GetCamera());
	
	Surface bottom(-20,force::Vector3(0,8,0),andro::Vector3(0,0.0,0.0),andro::Vector2(100,100));
	force::World::GetInstance()->AddPlane((force::Plane*)bottom.GetPhysicObject());

	std::unique_ptr<force::ParticleGravity> gravity_generator = make_unique<force::ParticleGravity>(force::Vector3(0, -38, 0));
	std::unique_ptr<force::ParticleGravity> gravity_generator2 = make_unique<force::ParticleGravity>(force::Vector3(0, -9.8, 0));


	Box *box[100];
	int boxnr=60;

	float angle = 2 * PI / boxnr;
	float radius =  25;
	for(int i=0; i<boxnr; i++)
	{
		float x,y;
		x = radius * cosf(angle * i);
		y = radius * sinf(angle * i);

			
			force::Quaternion q(force::Vector3(0,1,0),-angle*i);

			box[i] = new Box(force::Vector3(1.7 ,2.2, 0.3),force::Vector3(x,-20,y),false ,true,q,0.3);
			

			force::World::GetInstance()->registry.add(box[i]->GetPhysicObject()->rigidBody, gravity_generator.get());
			force::World::GetInstance()->AddBox((force::Box*)box[i]->GetPhysicObject());
	}



	//add to boxes connected

			Sphere*	boxA = new Sphere(3,force::Vector3(0,30,40),true,force::Quaternion(),3.3);

			boxA->GetPhysicObject()->rigidBody->SetCanSleep(false);

			force::World::GetInstance()->registry.add(boxA->GetPhysicObject()->rigidBody, gravity_generator2.get());
			force::World::GetInstance()->AddSphere((force::Sphere*)boxA->GetPhysicObject());
	

			
			Rope* ropeB = new Rope(andro::Vector3(0,0,30),7,1.3,1);
	
			force::ParticleCable constraint   =  force::ParticleCable(ropeB->GetLastElement(), boxA->GetPhysicObject()->rigidBody, 4);
			force::ParticleCableAnchor anchor =force::ParticleCableAnchor(ropeB->GetFirstElement(), force::Vector3(0, 10, 24), 2);
			
			force::World::GetInstance()->AddParticleContactGenerators
				(&constraint, 1);
			force::World::GetInstance()->AddParticleContactGenerators
				(&anchor, 1);


			force::World::GetInstance()->registry.add(boxA->GetPhysicObject()->rigidBody, gravity_generator2.get());
			force::World::GetInstance()->AddSphere((force::Sphere*)boxA->GetPhysicObject());




	DWORD FPS=0;
	TCHAR szFPS[32];
    szFPS[0]='\0';

	double currentTime=0;
	double lastTime=0;
	double lastframeTime=GetTickCount();
	double elapsedTime=0;
	double deltaTime=0;
	//This is the mainloop, we render frames until isRunning returns false
    while(!window.ShouldClose())
    {
        
		FPS++;

		
       currentTime=GetTickCount();
	   elapsedTime=currentTime-lastTime;
	
		deltaTime = (currentTime-lastframeTime)/1000;

        engine->Update(deltaTime); //Do any pre-rendering logic
		window.Update(deltaTime);

		if (deltaTime < 0.1 )
		{
			force::World::GetInstance()->runPhysics(deltaTime);

			for(int i=0; i<boxnr; i++)
				{
					box[i]->Update(deltaTime);
				}
			boxA->Update(deltaTime);
			ropeB->Update(deltaTime);
	
	
			andro::EventManager::GetInstance()->Update(deltaTime);
		}
		spawner.Update(deltaTime);
		lightPoint.Update(deltaTime);

	    if( elapsedTime>=1000)
		{
			wsprintf(szFPS, (LPCSTR)"Andro Engine  FPS = %u", (UINT)(FPS * 1000.0 / elapsedTime));
			window.SetTitle(szFPS);
			FPS = 0;
			lastTime=currentTime;
		
		}

		lastframeTime=currentTime;
	
	}

	//release data
	for (int i = 0; i<boxnr; i++)
		delete box[i];

	
	delete boxA;
	delete ropeB;
	
    return 0; //Return success
}
