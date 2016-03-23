#pragma once

#include <string>
#include <vector>

#include "Light.h"
#include "RenderNode.h"
#include "CameraNode.h"
#include "PD_CommandBuffer.h"
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

		void RegisterRenderObject(TakeTwo::RenderNode* pNode);
		void RemoveRenderObject(TakeTwo::RenderNode* pNode);
		void RegisterLight(Light* pLight);
		void RegisterCamera(TakeTwo::CameraNode* pCamera);

    private:
		andro::ResourceManager mResourceManager;
		std::vector<TakeTwo::RenderNode*> m_renderableObjects;
		TakeTwo::Light* m_light; //embarrassing, temporary
		TakeTwo::CameraNode* m_camera; //embarrassing, temporary
		PD_CommandBuffer*	m_commandBuffer;
		FBORenderTexture*	m_multipleRenderTarget;

    };
}
