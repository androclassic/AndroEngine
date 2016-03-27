#include "PD_CommandBuffer.h"
#include <exception>

namespace TakeTwo
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
		GLuint slot = (GL_TEXTURE0 + pSlot);
		glActiveTexture(slot);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pTexture->m_textureID);
	}
	void PD_CommandBuffer::BindRenderTargets(const PD_RenderTargetTexture* const* pRenderTargets, size_t pCount, const PD_RenderTargetTexture* pDepth)
	{
		GLint depth = (pDepth != nullptr) ? pDepth->m_texture.m_textureID : 0;

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
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);

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
		GLint depth = (pDepth != nullptr) ? pDepth->m_texture.m_textureID : 0;

		if (pRenderTarget == nullptr)
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
		}
		else
		{
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
			if (m_attachment[0] != pRenderTarget->m_renderTarget)
			{
				m_attachment[0] = pRenderTarget->m_renderTarget;
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_attachment[0]);
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, pRenderTarget->m_texture.m_textureID, 0);

				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);

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
	void PD_CommandBuffer::Clear(float r, float g, float b, float a)
	{
		glClearColor(r,g,b,a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}