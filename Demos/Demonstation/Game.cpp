#include "Game.h"
#include "PrimitivesFormat.h"



void Print_C(const char* msg)
{
	TRACE("[LUA_Print] %s \n", msg);
}

void Print_ERROR(const char* msg)
{
	TRACE("[LUA_ERROR] %s \n", msg);
	ASSERT(FALSE);
}


void glmVec3FromLua(lua_State * L, int index, Variable * ref)
{
	ASSERT(lua_istable(L, index));

	glm::vec3 *ref_vec = new ((glm::vec3*)ref->GetVoidPtr())glm::vec3();

	lua_getfield(L, index, "x");
	lua_getfield(L, index, "y");
	lua_getfield(L, index, "z");

	ref_vec->x = lua_tonumber(L, -3);
	ref_vec->y = lua_tonumber(L, -2);
	ref_vec->z = lua_tonumber(L, -1);
}
void glmVec3ToLua(lua_State * L, Variable & var)
{
	glm::vec3 vec = var.GetValue<glm::vec3 >();

	lua_newtable(L); //{*table is now in - 1 * }
	lua_pushnumber(L, vec.x); //{*table is now in - 2 * }
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, vec.y); //{*table is now in - 2 * }
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, vec.z); //{*table is now in - 2 * }
	lua_setfield(L, -2, "z");
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

	REGISTER_TYPE_EXPLCIT(glm::vec3, nativeVector3, glmVec3ToLua, glmVec3FromLua);

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
	REGISTER_USER_TYPE(force::Primitive);


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

	// physics bindings
	lua_bind_explicit(L, Primitives::Box, Box);
	lua_bind_explicit(L, Primitives::Plane, Plane);
	lua_bind_explicit(L, Primitives::Cylinder, Cylinder);
	lua_bind_explicit(L, Primitives::Sphere, Sphere);

	//debug bindings
	lua_bind(L, Print_C);
	lua_bind(L, Print_ERROR);


	Lua_State::GetInstance()->execute_program("data/lua_src/gameScriptInit.lua");

	//temporary here
	static shared_ptr<force::Plane> ground;
	ground = make_shared<force::Plane>(force::Vector3(0, 1, 0), -10);
	ground->rigidBody->SetPosition(0, -10, 0);
	ground->rigidBody->SetMass(MAX_MASS);

	force::World::GetInstance()->AddPrimitive(ground.get());

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
