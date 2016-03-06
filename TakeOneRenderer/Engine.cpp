#include "Engine.h"
#include "Log.h"
#include "GL/glew.h"
#include "../../AndroUtils/Introspection/LuaState.h"

bool TakeOne::Engine::Init(int pWidth, int pHeight, std::string pTitle)
{

	mResourceManager.Initialise("TakeOne_ResourceManager");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong.
		LOG_MSG(L"Error: %s\n", glewGetErrorString(err));
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}
TakeOne::Engine::Engine()
	: m_light(nullptr)
	, m_camera(nullptr)
{
}

TakeOne::Engine::~Engine()
{
}

void TakeOne::Engine::Update()
{
	ASSERT(m_light != nullptr);
	ASSERT(m_camera != nullptr);

	for (auto object : m_renderableObjects)
	{
		object->GetRenderObject().GetMaterial().SetShaderParam("light.position", m_light->position);
		object->GetRenderObject().GetMaterial().SetShaderParam("light.intensities", m_light->intensities);
		object->GetRenderObject().GetMaterial().SetShaderParam("light.attenuation", m_light->attenuation);
		object->GetRenderObject().GetMaterial().SetShaderParam("light.ambientCoefficient", m_light->ambientCoefficient);
		object->GetRenderObject().GetMaterial().SetShaderParam("camera", m_camera->GetViewProjectionMatrix());
		object->GetRenderObject().GetMaterial().SetShaderParam("model", object->GetTransform().GetTransformMatrix());

		object->GetRenderObject().Render();
	}

}

void TakeOne::Engine::RegisterRenderObject(TakeOne::RenderNode * pNode)
{
	m_renderableObjects.push_back(pNode);
}

void TakeOne::Engine::RegisterLight(Light * pLight)
{
	m_light = pLight;
}

void TakeOne::Engine::RegisterCamera(TakeOne::CameraNode * pCamera)
{
	m_camera = pCamera;
}


