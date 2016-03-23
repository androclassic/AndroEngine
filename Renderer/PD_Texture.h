#pragma once
#include "GL\glew.h"

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



	class FBORenderTexture
	{
	public:
		// Ctors/Dtors
		FBORenderTexture(int width, int height);
		~FBORenderTexture();

		// Methods
		void	start();

		PD_Texture*	getDiffuseTexture() const { return   &m_rt[0]->m_texture; }
		PD_Texture*	getPositionTexture() const { return   &m_rt[1]->m_texture; }
		PD_Texture*	getNormalsTexture() const { return   &m_rt[2]->m_texture; }

		PD_RenderTargetTexture* m_rt[4];
	private:

		unsigned int	m_width; // FBO width
		unsigned int	m_height; // FBO height
	};

}