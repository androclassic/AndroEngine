#pragma once
#include "PD_Texture.h"

#define MAX_RT 5

namespace TakeTwo
{
	class PD_CommandBuffer
	{
	public:
		PD_CommandBuffer();
		~PD_CommandBuffer();

		void BindTexture(PD_Texture* pTexture, unsigned int pSlot);
		void BindRenderTargets(const PD_RenderTargetTexture* const* pRenderTargets, size_t pCount, const PD_RenderTargetTexture* pDepth);
		void BindRenderTarget(const PD_RenderTargetTexture* pRenderTarget, const PD_RenderTargetTexture* pDepth);
		void Clear(float r, float g, float b, float a);

	private:
		GLuint			m_fbo; // The FBO ID
		GLint			m_attachment[MAX_RT];

	};
}