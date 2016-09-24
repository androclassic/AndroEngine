#pragma once
#include "GL/glew.h"

namespace TakeTwo
{

	struct PD_Texture
	{
		PD_Texture(int pWidth, int pHeight, GLint pFormat, GLenum pType);
		~PD_Texture();

		unsigned int	m_textureID;
		unsigned int	m_width;
		unsigned int	m_height;
		GLint			m_format;
		GLenum			m_type;
	};


	class PD_RenderTargetTexture
	{
	public:
		PD_RenderTargetTexture(int pWidth, int pHeight, GLint pFormat, GLenum pType); // TODO format general
		~PD_RenderTargetTexture();
		GLuint			m_renderTarget;
		PD_Texture		m_texture;


	};


}