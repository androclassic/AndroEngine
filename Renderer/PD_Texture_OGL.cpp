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


}