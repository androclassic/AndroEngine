#include "Game.h"




void Print_C(const char* msg)
{
	TRACE(msg);
}



void Print_ERROR(const char* msg)
{
	TRACE("[LUA_ERROR] %s \n", msg);
	ASSERT(FALSE);
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
	mMainLight.reset(new TakeTwo::Light);
	mCamera.reset(new Camera());

	TakeTwo::Engine& engine = *TakeTwo::Engine::GetInstance();
	engine.Init(1024, 768, "AndroEngine");

	mCamera->GetCamera()->SetCameraType(TakeTwo::CameraType::PERSPECTIVE);
	mCamera->GetCamera()->SetClearColor(glm::vec4(0.1,0.2,0.1, 1.0));
	mCamera->GetCamera()->SetPerspectiveFOV(45.0f, windowWidth, windowHeight, 1.0f, 1000.0f);
	mCamera->GetCamera()->SetPosition(glm::vec3(0, 3 , -40));
	mCamera->GetCamera()->LookAt(glm::vec3(0, -10, 0));

	mMainLight->position = glm::vec3(0.0f, 100.0f, -10.0f);
	mMainLight->intensities = glm::vec3(0.8f, 0.8f, 0.8f) * 50.0f;
	mMainLight->attenuation = 0.01f;
	mMainLight->ambientCoefficient = 0.01f;

	engine.RegisterLight(mMainLight.get());
	engine.RegisterCamera(mCamera->GetCamera().get());
//-----------------------------------------------------------
//------ render types
//-----------------------------------------------------------

	REGISTER_TYPE_EXPLCIT(TakeTwo::Material::MaterialFormat, MaterialFormat, TakeTwo::Material::MaterialFormat::MaterialToLua, TakeTwo::Material::MaterialFormat::MaterialFromLua);
	REGISTER_TYPE_EXPLCIT(TakeTwo::EffectDesc, EffectDesc, TakeTwo::EffectDesc::ToLua, TakeTwo::EffectDesc::FromLua);
	REGISTER_TYPE_EXPLCIT(TakeTwo::BlendDesc, BlendDesc, TakeTwo::BlendDesc::ToLua, TakeTwo::BlendDesc::FromLua);
	REGISTER_TYPE_EXPLCIT(TakeTwo::DepthStencilDesc, DepthStencilDesc, TakeTwo::DepthStencilDesc::ToLua, TakeTwo::DepthStencilDesc::FromLua);

//-----------------------------------------------------------
//------ physcis types
//-----------------------------------------------------------

//-----------------------------------------------------------
//------ game types
//-----------------------------------------------------------
	REGISTER_USER_TYPE(GameObject);
//-----------------------------------------------------------


	//initialise Lua
	Lua_State::GetInstance()->Init();
	lua_State* L = *Lua_State::GetInstance();
	
	//game bindings
	lua_bind_member(L, GameObject, CreateGameObject);
	lua_bind_member(L, GameObject, DestroyGameObject);
	lua_bind_member(L, GameObject, NativeUpdate);
	lua_bind_member(L, GameObject, SetPosition);
	lua_bind_member(L, GameObject, SetScale);

	// render bindings
	lua_bind_explicit(L, TakeTwo::EffectLibrary::AddEffect, AddEffectLib);

	//debug bindings
	lua_bind(L, Print_C);
	lua_bind(L, Print_ERROR);


	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptInit.lua");

	//temporary here
	static shared_ptr<force::Plane> ground;
	ground = make_shared<force::Plane>();
	ground->rigidBody = new force::RigidBody();
	ground->rigidBody->SetPosition(0, -10, 0);
	ground->rigidBody->SetMass(MAX_MASS);
	((force::Plane*)ground.get())->normal = force::Vector3(0,1,0);
	((force::Plane*)ground.get())->offset = -10;

	force::World::GetInstance()->AddPlane(ground.get());

}




void Game::Update(afloat deltaTime)
{

	TakeTwo::Engine::GetInstance()->Update();
	mWindow->Update(deltaTime);
	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptUpdate.lua");
	force::World::GetInstance()->runPhysics(deltaTime);
}

void Game::End()
{
	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptDestroy.lua");
}
