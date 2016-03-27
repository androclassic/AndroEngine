#pragma once

#include "PD_CommandBuffer.h"
#include "EffectLib.h"
#include "Material.h"
#include "../AndroUtils/Utils/Singleton.h"
#include "../AndroUtils/Utils/ResourceManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------

namespace TakeTwo
{
	enum GBufferType
	{
		DIFFUSE = 0,
		NORMALS,
		SPECULAR,
		GBUFFER_CNT
	};

	class GBuffers
	{
	public:
		// Ctors/Dtors
		GBuffers(int width, int height);
		~GBuffers();

		PD_Texture*	getTexture(const unsigned int index) const { return   &m_renderTarget[index]->m_texture; }

		void Bind(PD_RenderTargetTexture* pDepth);

	private:
		PD_RenderTargetTexture* m_renderTarget[GBufferType::GBUFFER_CNT];

		unsigned int	m_width; 
		unsigned int	m_height;
	};



	class DeferredRenderer
	{
		public:
			DeferredRenderer(int width, int height);
			~DeferredRenderer();
			void ResolveDeferred(GBuffers* m_gBuffers, PD_RenderTargetTexture* pDepth);
	private:
		Material		m_deferred_mat;
		unsigned int	m_width;
		unsigned int	m_height;

	};
}