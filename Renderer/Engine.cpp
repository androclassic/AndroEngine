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

	m_multipleRenderTarget = new FBORenderTexture(pWidth, pHeight);
	m_commandBuffer = new PD_CommandBuffer();

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
	delete m_multipleRenderTarget;

}

void TakeTwo::Engine::Update()
{
	ASSERT(m_light != nullptr);
	ASSERT(m_camera != nullptr);


	// Render our geometry into the FBO
	m_commandBuffer->BindRenderTargets(m_multipleRenderTarget->m_rt, 3, m_multipleRenderTarget->m_rt[3]);
	m_multipleRenderTarget->start();
		
		for (auto object : m_renderableObjects)
		{
			if (strcmp(object->GetRenderObject().GetResourceFileName().c_str(), "data/cube2.obj_simpletexturemap_dwarf.jpg") == 0)
				continue;

			object->GetRenderObject().GetMaterial().SetShaderParam("light.position", m_light->position);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.intensities", m_light->intensities);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.attenuation", m_light->attenuation);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.ambientCoefficient", m_light->ambientCoefficient);
			object->GetRenderObject().GetMaterial().SetShaderParam("camera", m_camera->GetViewProjectionMatrix());
			object->GetRenderObject().GetMaterial().SetShaderParam("model", object->GetTransform().GetTransformMatrix());

			object->GetRenderObject().Render();
		}


	m_commandBuffer->BindRenderTarget(nullptr, m_multipleRenderTarget->m_rt[3]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//hacky hack temp don`t look
	unsigned index = 0;
	for (auto object : m_renderableObjects)
	{
		if (strcmp(object->GetRenderObject().GetResourceFileName().c_str(), "data/cube2.obj_simpletexturemap_dwarf.jpg") == 0)
		{
			object->GetRenderObject().GetMaterial().SetShaderParam("light.position", m_light->position);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.intensities", m_light->intensities);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.attenuation", m_light->attenuation);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.ambientCoefficient", m_light->ambientCoefficient);
			object->GetRenderObject().GetMaterial().SetShaderParam("camera", m_camera->GetViewProjectionMatrix());
			object->GetRenderObject().GetMaterial().SetShaderParam("model", object->GetTransform().GetTransformMatrix());

			TakeTwo::Texture texture; //hack
			texture.mTextureId = m_multipleRenderTarget->m_rt[index++]->m_texture.m_textureID;
			object->GetRenderObject().GetMaterial().SetTexture(std::move(texture));
			object->GetRenderObject().Render();
		}
	}

}

void TakeTwo::Engine::RegisterRenderObject(TakeTwo::RenderNode * pNode)
{
	m_renderableObjects.push_back(pNode);
}

void TakeTwo::Engine::RemoveRenderObject(TakeTwo::RenderNode * pNode)
{
	for (std::vector<TakeTwo::RenderNode*>::iterator it = m_renderableObjects.begin(); it != m_renderableObjects.end(); it++)
	{
		if (*it == pNode)
		{
			m_renderableObjects.erase(it);
			return;
		}
	}
}

void TakeTwo::Engine::RegisterLight(Light * pLight)
{
	m_light = pLight;
}

void TakeTwo::Engine::RegisterCamera(TakeTwo::CameraNode * pCamera)
{
	m_camera = pCamera;
}


