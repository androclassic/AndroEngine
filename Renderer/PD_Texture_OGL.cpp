#include "PD_Texture.h"

namespace TakeTwo
{
	PD_Texture::PD_Texture(int pWidth, int pHeight, GLint pFormat, GLenum pType)
	{
		m_width = pWidth;
		m_height = pHeight;
		m_format = pFormat;
		m_type = pType;

		// Generate  m_renderTargetTexture
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, GL_RGBA, m_type, nullptr);
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