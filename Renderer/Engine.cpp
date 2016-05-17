#include "Engine.h"
#include "Log.h"
#include "GL/glew.h"
#include "../../AndroUtils/Introspection/LuaState.h"

//-----------------------------------------------------------------------------------------------------------------------

bool TakeTwo::Engine::Init(int pWidth, int pHeight, std::string pTitle)
{

	mResourceManager.Initialise("TakeTwo_ResourceManager");
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

	m_gBuffers = new GBuffers(pWidth, pHeight);
	m_deferred = new DeferredRenderer(pWidth, pHeight);
	m_commandBuffer = new PD_CommandBuffer();
	m_depth =  new PD_RenderTargetTexture(pWidth, pHeight, GL_DEPTH_COMPONENT24, GL_FLOAT);
	return true;
}
TakeTwo::Engine::Engine()
	: m_light(nullptr)
	, m_camera(nullptr)
{
}

TakeTwo::Engine::~Engine()
{
	delete m_commandBuffer;
	delete m_gBuffers;
	delete m_deferred;

}

void TakeTwo::Engine::Update()
{
	ASSERT(m_light != nullptr);
	ASSERT(m_camera != nullptr);


	m_renderableObjects.clear();
	Node* node = &m_SceneRoot;
	while (node)
	{
		if (node->HasRenderObject())
			m_renderableObjects.push_back(node);

		node = node->GetNextInHierarchy();
	}



	m_gBuffers->Bind(m_depth);

	for (auto object : m_renderableObjects)
	{
		object->GetRenderObject().GetMaterial().SetShaderParam("light.position", m_light->position);
		object->GetRenderObject().GetMaterial().SetShaderParam("light.intensities", m_light->intensities);
		object->GetRenderObject().GetMaterial().SetShaderParam("light.attenuation", m_light->attenuation);
		object->GetRenderObject().GetMaterial().SetShaderParam("light.ambientCoefficient", m_light->ambientCoefficient);
		object->GetRenderObject().GetMaterial().SetShaderParam("camera", m_camera->GetViewProjectionMatrix());
		object->GetRenderObject().GetMaterial().SetShaderParam("model", object->GetTransformMatrix());

		object->GetRenderObject().Render();
	}

	m_commandBuffer->BindRenderTarget(nullptr, m_depth);
	m_commandBuffer->Clear(0.0f, 0.2f, 0.2f, 1.0f);

	m_deferred->ResolveDeferred(m_gBuffers, m_depth);

}

void TakeTwo::Engine::RegisterRenderObject(TakeTwo::Node * pNode)
{
	m_SceneRoot.AddChild(pNode);
}

void TakeTwo::Engine::RemoveRenderObject(TakeTwo::Node * pNode)
{
	m_SceneRoot.RemoveChild(pNode);
}

void TakeTwo::Engine::RegisterLight(Light * pLight)
{
	m_light = pLight;
}

void TakeTwo::Engine::RegisterCamera(TakeTwo::CameraNode * pCamera)
{
	m_camera = pCamera;
}


