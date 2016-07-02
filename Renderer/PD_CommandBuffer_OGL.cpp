#include "PD_CommandBuffer.h"
#include "../AndroUtils/Utils/Trace.h"
#include <exception>
#include <vector>

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
	void PD_CommandBuffer::Draw(unsigned int pVertex_count, unsigned int pStart_vertex_location)
	{
		glDrawArrays(GL_TRIANGLES, pStart_vertex_location, pVertex_count);
	}
	void PD_CommandBuffer::DrawIndexed(unsigned int pIndex_count, unsigned int pStart_index_location, unsigned int pBase_vertex_location)
	{
		glDrawElements(
			GL_TRIANGLES,
			pIndex_count,
			GL_UNSIGNED_INT,
			reinterpret_cast<const void*>(0)
			);
	}

	void PD_CommandBuffer::Clear(afloat r, afloat g, afloat b, afloat a)
	{
		glClearColor(r,g,b,a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PD_CommandBuffer::BindVertexBuffer(PDBuffer* pBuffer, const std::vector<unsigned int>&  pAttribsUsed)
	{
		GLuint buffer_id = pBuffer ? pBuffer->m_buffer : 0;
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id);


		if(pBuffer)
		{

			if (pAttribsUsed[Vertex::POSITION] )
			{
				glEnableVertexAttribArray(Vertex::POSITION);
				glVertexAttribPointer(
					Vertex::POSITION,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					reinterpret_cast<const void*>(offsetof(Vertex, position))
					);
			}

			if (pAttribsUsed[Vertex::NORMAL])
			{
				glEnableVertexAttribArray(Vertex::NORMAL);
				glVertexAttribPointer(
					Vertex::NORMAL,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					reinterpret_cast<const void*>(offsetof(Vertex, normal))
					);
			}

			if (pAttribsUsed[Vertex::COLOR])
			{
				glEnableVertexAttribArray(Vertex::COLOR);
				glVertexAttribPointer(
					Vertex::COLOR,
					3,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					reinterpret_cast<const void*>(offsetof(Vertex, color))
					);
			}

			if (pAttribsUsed[Vertex::TEXCOORD])
			{
				glEnableVertexAttribArray(Vertex::TEXCOORD);
				glVertexAttribPointer(
					Vertex::TEXCOORD,
					2,
					GL_FLOAT,
					GL_FALSE,
					sizeof(Vertex),
					reinterpret_cast<const void*>(offsetof(Vertex, texCoord))
					);
			}
		}
	}
	void PD_CommandBuffer::BindIndexBuffer(PDBuffer* pBuffer)
	{
		GLuint buffer_id = pBuffer ? pBuffer->m_buffer : 0;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);

	}



	//-------------------------------------------------------------------------------------------
	Factory::Factory()
	{
	}

	Factory::~Factory()
	{

	}

	PDBuffer* Factory::CreateVertexBuffer(size_t size, void * data)
	{
		PDBuffer* vbo = new PDBuffer();
		glGenBuffers(1, &vbo->m_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vbo->m_buffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		return vbo;
	}

	PDBuffer* Factory::CreateIndexBuffer(size_t size, void * data)
	{
		PDBuffer* ibo = new PDBuffer();
		glGenBuffers(1, &ibo->m_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->m_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		return ibo;
	}





}