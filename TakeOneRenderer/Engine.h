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
    class Engine
    {
	public:
		SINGLETON(Engine);

    public:
        bool Init(int pWidth, int pHeight, std::string pTitle);
        void Update();

		andro::ResourceManager& GetResoruceManager()  { return mResourceManager; }

		void RegisterRenderObject(TakeOne::RenderNode* pNode);
		void RegisterLight(Light* pLight);
		void RegisterCamera(TakeOne::CameraNode* pCamera);

    private:
		andro::ResourceManager mResourceManager;
		std::vector<TakeOne::RenderNode*> m_renderableObjects;
		TakeOne::Light* m_light; //embarrassing, temporary
		TakeOne::CameraNode* m_camera; //embarrassing, temporary

    };
}
