#pragma once

#include <string>
#include <vector>

#include "Light.h"
#include "RenderNode.h"
#include "CameraNode.h"
#include "../AndroUtils/Utils/Singleton.h"
#include "../AndroUtils/Utils/ResourceManager.h"

namespace TakeOne
{


	#define MAX_RT 5

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

	class PD_CommandBuffer
	{
	public:
		PD_CommandBuffer();
		~PD_CommandBuffer();

		void BindTexture(PD_Texture* pTexture, unsigned int pSlot);
		void BindRenderTargets(const PD_RenderTargetTexture* const* pRenderTargets, size_t pCount, const PD_RenderTargetTexture* pDepth);
		void BindRenderTarget(const PD_RenderTargetTexture* pRenderTarget, const PD_RenderTargetTexture* pDepth);

	private:
		GLuint			m_fbo; // The FBO ID
		GLuint			m_attachment[MAX_RT];

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
//--------------------------------------------------------------------------------------------------------------------------------------

namespace TakeOne
{
    class Engine
    {
	public:
		SINGLETON(Engine);

    public:
        bool Init(int pWidth, int pHeight, std::string pTitle);
        void Update();

		andro::ResourceManager& GetResoruceManager()  { return mResourceManager; }

		void RegisterRenderObject(TakeOne::RenderNode* pNode);
		void RemoveRenderObject(TakeOne::RenderNode* pNode);
		void RegisterLight(Light* pLight);
		void RegisterCamera(TakeOne::CameraNode* pCamera);

    private:
		andro::ResourceManager mResourceManager;
		std::vector<TakeOne::RenderNode*> m_renderableObjects;
		TakeOne::Light* m_light; //embarrassing, temporary
		TakeOne::CameraNode* m_camera; //embarrassing, temporary
		PD_CommandBuffer*	m_commandBuffer;
		FBORenderTexture*	m_multipleRenderTarget;

    };
}
