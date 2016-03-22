#include "Engine.h"
#include "Log.h"
#include "GL/glew.h"
#include "../../AndroUtils/Introspection/LuaState.h"


namespace TakeOne
{

	PD_CommandBuffer::PD_CommandBuffer()
	{
		// Generate the OGL resources for what we need
		glGenFramebuffersEXT(1, &m_fbo);

	}

	PD_CommandBuffer::~PD_CommandBuffer()
	{
	}

	void PD_CommandBuffer::BindTexture(PD_Texture* pTexture, unsigned int pSlot)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB + pSlot);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pTexture->m_textureID);
	}
	void PD_CommandBuffer::BindRenderTargets(const PD_RenderTargetTexture* const* pRenderTargets, size_t pCount, const PD_RenderTargetTexture* pDepth)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

		bool invalid = false;
		for (size_t i = 0; i < pCount; i++)
		{
			if (m_attachment[i] != pRenderTargets[i]->m_renderTarget)
			{
				invalid = true;
				m_attachment[i] = pRenderTargets[i]->m_renderTarget;
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_RENDERBUFFER_EXT, m_attachment[i]);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, pRenderTargets[i]->m_texture.m_textureID, 0);
			}
		}
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, pDepth->m_texture.m_textureID);

		if (invalid)
		{
			// Check if all worked fine and unbind the FBO
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
				throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");
		}
		// Specify what to render an start acquiring
		static GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT };
		glDrawBuffers(pCount, buffers);

	}
	void PD_CommandBuffer::BindRenderTarget(const PD_RenderTargetTexture* pRenderTarget, const  PD_RenderTargetTexture* pDepth)
	{

		if (pRenderTarget == nullptr)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, pDepth->m_texture.m_textureID);
		}
		else
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
			if (m_attachment[0] != pRenderTarget->m_renderTarget)
			{
				m_attachment[0] = pRenderTarget->m_renderTarget;
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_attachment[0]);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pRenderTarget->m_texture.m_textureID, 0);

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, pDepth->m_texture.m_textureID);

				// Check if all worked fine and unbind the FBO
				GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
				if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
					throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");

			}

			// Specify what to render an start acquiring
			static GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT };
			glDrawBuffers(1, buffers);
		}



	}



	PD_Texture::PD_Texture(int pWidth, int pHeight, GLint pFormat, GLenum pType)
	{
		m_width = pWidth;
		m_height = pHeight;
		m_format = pFormat;
		m_type = pType;

		// Generate  m_renderTargetTexture
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGBA, m_type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	};
	PD_Texture::~PD_Texture()
	{
		glDeleteTextures(1, &m_textureID);
	}


	PD_RenderTargetTexture::PD_RenderTargetTexture(int pWidth, int pHeight, GLint pFormat, GLenum pType)
		: m_texture(pWidth, pHeight, pFormat, pType)
	{
		// Generate  m_renderTarget
		glGenRenderbuffersEXT(1, &m_renderTarget);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_renderTarget);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, pFormat, pWidth, pHeight);
	}

	PD_RenderTargetTexture::~PD_RenderTargetTexture()
	{
		glDeleteRenderbuffersEXT(1, &m_renderTarget);
	}

	/**
	*	Create the FBO render texture initializing all the stuff that we need
	*/
	FBORenderTexture::FBORenderTexture(int _dWidth, int _dHeight)
	{
		// Save extensions
		m_width = _dWidth;
		m_height = _dHeight;


		m_rt[0] = new PD_RenderTargetTexture(m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE);
		m_rt[1] = new PD_RenderTargetTexture(m_width, m_height, GL_RGBA32F_ARB, GL_FLOAT);
		m_rt[2] = new PD_RenderTargetTexture(m_width, m_height, GL_RGBA16F_ARB, GL_FLOAT);
		m_rt[3] = new PD_RenderTargetTexture(m_width, m_height, GL_DEPTH_COMPONENT24, GL_FLOAT);

	}

	/**
	*	Destructor
	*/
	FBORenderTexture::~FBORenderTexture() {

		delete m_rt[0];
		delete m_rt[1];
		delete m_rt[2];
		delete m_rt[3];
	}

	/**
	*	Start rendering to the texture
	*	Both color and depth buffers are cleared.
	*/
	void FBORenderTexture::start() {
		// Clear the render targets
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.2f, 0.2f, 1.0f);

	}



}

//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

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

	m_multipleRenderTarget = new FBORenderTexture(pWidth, pHeight);
	m_commandBuffer = new PD_CommandBuffer();

	return true;
}
TakeOne::Engine::Engine()
	: m_light(nullptr)
	, m_camera(nullptr)
{
}

TakeOne::Engine::~Engine()
{
	delete m_commandBuffer;
	delete m_multipleRenderTarget;

}

void TakeOne::Engine::Update()
{
	ASSERT(m_light != nullptr);
	ASSERT(m_camera != nullptr);


	// Render our geometry into the FBO
	m_commandBuffer->BindRenderTargets(m_multipleRenderTarget->m_rt, 3, m_multipleRenderTarget->m_rt[3]);
	m_multipleRenderTarget->start();
		
		for (auto object : m_renderableObjects)
		{
			if (strcmp(object->GetRenderObject().GetResourceFileName().c_str(), "data/sphere.obj_simpletexturemap_dwarf.jpg") == 0)
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


	for (auto object : m_renderableObjects)
	{
		if (strcmp(object->GetRenderObject().GetResourceFileName().c_str(), "data/sphere.obj_simpletexturemap_dwarf.jpg") == 0)
		{
			object->GetRenderObject().GetMaterial().SetShaderParam("light.position", m_light->position);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.intensities", m_light->intensities);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.attenuation", m_light->attenuation);
			object->GetRenderObject().GetMaterial().SetShaderParam("light.ambientCoefficient", m_light->ambientCoefficient);
			object->GetRenderObject().GetMaterial().SetShaderParam("camera", m_camera->GetViewProjectionMatrix());
			object->GetRenderObject().GetMaterial().SetShaderParam("model", object->GetTransform().GetTransformMatrix());

			TakeOne::Texture texture; //hack
			texture.mTextureId = m_multipleRenderTarget->m_rt[0]->m_texture.m_textureID;
			object->GetRenderObject().GetMaterial().SetTexture(std::move(texture));
			object->GetRenderObject().Render();
		}
	}

}

void TakeOne::Engine::RegisterRenderObject(TakeOne::RenderNode * pNode)
{
	m_renderableObjects.push_back(pNode);
}

void TakeOne::Engine::RemoveRenderObject(TakeOne::RenderNode * pNode)
{
	for (std::vector<TakeOne::RenderNode*>::iterator it = m_renderableObjects.begin(); it != m_renderableObjects.end(); it++)
	{
		if (*it == pNode)
		{
			m_renderableObjects.erase(it);
			return;
		}
	}
}

void TakeOne::Engine::RegisterLight(Light * pLight)
{
	m_light = pLight;
}

void TakeOne::Engine::RegisterCamera(TakeOne::CameraNode * pCamera)
{
	m_camera = pCamera;
}


