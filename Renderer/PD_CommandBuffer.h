#pragma once
#include "../AndroUtils/Utils/AndroTypes.h"
#include "OGL_Platform.h"
#include "PD_Texture.h"
#include <vector>

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

		void BindVertexBuffer(PDBuffer* pBuffer, const std::vector<unsigned int>&  pAttribsUsed);
		void BindIndexBuffer(PDBuffer* pBuffer);

		void Draw(unsigned int pVertex_count, unsigned int pStart_vertex_location);
		void DrawIndexed(unsigned int pIndex_count, unsigned int pStart_index_location, unsigned int pBase_vertex_location);

		void Clear(afloat r, afloat g, afloat b, afloat a);

	private:
		GLuint			m_fbo; // The FBO ID
		GLint			m_attachment[MAX_RT];

	};



	class Factory
	{
	public:
		Factory();
		~Factory();
		PDBuffer* CreateVertexBuffer(size_t size, void* data);
		PDBuffer* CreateIndexBuffer(size_t size, void* data);

	};


}