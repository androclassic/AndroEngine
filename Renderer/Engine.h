#pragma once

#include <string>
#include <vector>

#include "Light.h"
#include "Node.h"
#include "CameraNode.h"
#include "PD_CommandBuffer.h"
#include "EffectLib.h"
#include "DeferredRenderer.h"
#include "../AndroUtils/Utils/Singleton.h"
#include "../AndroUtils/Utils/ResourceManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------

namespace TakeTwo
{
    class Engine
    {
	public:
		SINGLETON(Engine);

    public:
        bool Init(int pWidth, int pHeight, std::string pTitle);
        void Update();

		andro::ResourceManager& GetResoruceManager()  { return mResourceManager; }

		void RegisterRenderObject(TakeTwo::Node* pNode);
		void RemoveRenderObject(TakeTwo::Node* pNode);
		void RegisterLight(Light* pLight);
		void RegisterCamera(TakeTwo::CameraNode* pCamera);
		PD_CommandBuffer* GetCommandBuffer() { return m_commandBuffer; }
		Factory*		  GetFactory() { return &m_factory; }

    private:
		andro::ResourceManager			  mResourceManager;
		std::vector<TakeTwo::Node*> m_renderableObjects;

		TakeTwo::Light*					m_light; //embarrassing, temporary
		TakeTwo::CameraNode*			m_camera; //embarrassing, temporary
		PD_CommandBuffer*				m_commandBuffer;
		Factory							m_factory;
		GBuffers*						m_gBuffers;
		DeferredRenderer*				m_deferred;
		PD_RenderTargetTexture*			m_depth;

		TakeTwo::Node					m_SceneRoot;

    };
}
