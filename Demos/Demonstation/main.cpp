#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one
#include "Game.h"

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
	unsigned int FPS = 0;
	char szFPS[32];
	szFPS[0] = '\0';

	double currentTime = 0;
	double lastTime = 0;
	double lastframeTime = 0;
	double elapsedTime = 0;
	double deltaTime = 0;


	Game game;
	game.Initialise();

	TakeTwo::Engine& engine = *TakeTwo::Engine::GetInstance();


	//This is the mainloop, we render frames until isRunning returns false
	while (!game.GetWindow().ShouldClose())
	{
		FPS++;
		currentTime =andro::GetTimeMS();
		elapsedTime = currentTime - lastTime;

		deltaTime = (currentTime - lastframeTime) / 1000.0f;

		game.Update(deltaTime);
		andro::EventManager::GetInstance()->Update(deltaTime);

		if (elapsedTime >= 1000)
		{
		#ifdef _WIN32
			wsprintf(szFPS, (LPCSTR)"Andro Engine  FPS = %u", (UINT)(FPS));
		#endif	
			game.GetWindow().SetTitle(szFPS);
			FPS = 0;
			lastTime = currentTime;

		}

		lastframeTime = currentTime;

	}
	
	game.End();

	return 0; //Return success
}

