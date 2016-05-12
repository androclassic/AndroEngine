#pragma once


#include "../GameObject.h"



class Game
{
public:
	Game();

	void Initialise();
	void Update(float dt);
	void End();

	inline Window& GetWindow() { return *mWindow.get(); }

private:
	std::unique_ptr<Window> mWindow;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<TakeTwo::Light> mMainLight;

};





