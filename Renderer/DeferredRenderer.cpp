#include "DeferredRenderer.h"
#include  "Engine.h"

namespace TakeTwo
{
	GBuffers::GBuffers(int width, int height)
	{
		m_width = width;
		m_height = height;


		m_renderTarget[GBufferType::DIFFUSE]  = new PD_RenderTargetTexture(m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE);
		m_renderTarget[GBufferType::NORMALS]  = new PD_RenderTargetTexture(m_width, m_height, GL_RGBA32F, GL_FLOAT);
		m_renderTarget[GBufferType::SPECULAR] = new PD_RenderTargetTexture(m_width, m_height, GL_RGBA32F, GL_FLOAT);
	}

	GBuffers::~GBuffers()
	{
		for (unsigned int i = 0; i < GBufferType::GBUFFER_CNT; i++)
		{
			delete m_renderTarget[i];
			m_renderTarget[i] = nullptr;
		}
	}

	void GBuffers::Bind(PD_RenderTargetTexture* pDepth)
	{
		PD_CommandBuffer* cb = Engine::GetInstance()->GetCommandBuffer();
		// Render our geometry into the FBO
		cb->BindRenderTargets(m_renderTarget, 3, pDepth);
		// Clear the render targets
		cb->Clear(1.0f, 0.2f, 0.2f, 1.0f);
	}

	DeferredRenderer::DeferredRenderer(int width, int height)
	{
		m_width = width;
		m_height = height;

	}

	DeferredRenderer::~DeferredRenderer()
	{
	}

	void DeferredRenderer::ResolveDeferred(GBuffers * m_gBuffers, PD_RenderTargetTexture* pDepth)
	{
		static bool init_material = false;
		PD_CommandBuffer* cb = Engine::GetInstance()->GetCommandBuffer();

		if (!init_material)
		{
			Program* deferred = EffectLibrary::GetInstance()->GetEffect("resolve_deferred");
			m_deferred_mat.SetProgram(deferred);

			m_deferred_mat.SetShaderParam("u_diffuse", 1);
			m_deferred_mat.SetShaderParam("u_normals", 2);
			m_deferred_mat.SetShaderParam("u_specular", 3);

			init_material = true;
		}

		m_deferred_mat.Use();

		cb->BindTexture(m_gBuffers->getTexture(GBufferType::DIFFUSE),  1);
		cb->BindTexture(m_gBuffers->getTexture(GBufferType::NORMALS),  2);
		cb->BindTexture(m_gBuffers->getTexture(GBufferType::SPECULAR),  3);



		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

}
