#include "Game.h"




void Print_C(const char* msg)
{
	printf(msg);
}


Game::Game()
	: mWindow( nullptr )
	, mCamera( nullptr )
	, mMainLight( nullptr )
{
}




void Game::Initialise()
{
	const int windowWidth = 1024;
	const int windowHeight = 768;

	mWindow.reset(new Window(windowWidth, windowHeight));
	mMainLight.reset(new TakeOne::Light);
	mCamera.reset(new Camera());

	TakeOne::Engine& engine = *TakeOne::Engine::GetInstance();
	engine.Init(1024, 768, "AndroEngine");

	mCamera->GetCamera()->SetCameraType(TakeOne::CameraType::PERSPECTIVE);
	mCamera->GetCamera()->SetClearColor(glm::vec4(0.1,0.2,0.1, 1.0));
	mCamera->GetCamera()->SetPerspectiveFOV(45.0f, windowWidth, windowHeight, 1.0f, 1000.0f);
	mCamera->GetCamera()->GetTransform().SetPosition(glm::vec3(0, 3 , -40));
	mCamera->GetCamera()->LookAt(glm::vec3(0, -10, 0));

	mMainLight->position = glm::vec3(0.0f, 100.0f, -10.0f);
	mMainLight->intensities = glm::vec3(0.8f, 0.8f, 0.8f) * 50.0f;
	mMainLight->attenuation = 0.01f;
	mMainLight->ambientCoefficient = 0.01f;

	engine.RegisterLight(mMainLight.get());
	engine.RegisterCamera(mCamera->GetCamera().get());



	REGISTER_USER_TYPE(GameObject);
	REGISTER_USER_TYPE_REF(GameObject);
	REGISTER_TYPE_EXPLCIT(TakeOne::Material::MaterialFormat, MaterialFormat, TakeOne::Material::MaterialFormat::MaterialToLua, TakeOne::Material::MaterialFormat::MaterialFromLua);

	//initialise Lua
	Lua_State::GetInstance()->Init();
	lua_State* L = *Lua_State::GetInstance();

	static Function CreateGameObject = make_gen(GameObject::CreateGameObject);
	BindFunctionToLua(L, &CreateGameObject, "CreateGameObject");

	static Function DestroyGameObject = make_gen(GameObject::DestroyGameObject);
	BindFunctionToLua(L, &DestroyGameObject, "DestroyGameObject");


	lua_bind_member(L, GameObject, SetPosition);
	lua_bind_member(L, GameObject, SetScale);
	lua_bind(L, Print_C);

	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptInit.lua");

}




void Game::Update(float deltaTime)
{

	TakeOne::Engine::GetInstance()->Update();
	mWindow->Update(deltaTime);
	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptUpdate.lua");


}

void Game::End()
{
	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptDestroy.lua");
}