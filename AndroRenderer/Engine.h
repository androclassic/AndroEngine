#pragma once

#include "Scene.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "../AndroUtils/Singleton.h"
#include "../AndroUtils/ResourceManager.h"

namespace andro
{
class Engine // same as Example class
{
	SINGLETON(Engine);
	public:
		bool			Init(const char* vertexShader, const char* fragmentShader);
		void			Update(float dt);	// update scene

		void			Resize(int width, int height);	// like Example Resize

		void			SetClearColor(float r, float g, float b, float a){ glClearColor(r, g, b, a); }
		void			SetLightPositionDir(Vector3 pos, Vector3 dir);

		bool			EnableShadow(bool state);
	    Scene*			GetScene() const {return s_mainScene.get();}
		Vector3			GetLightPosition() const {return s_lightPosition;}
		Vector3			GetLightLookAt() const {return s_lightLookAt;}
		
		ResourceManager& GetResoruceManager() { return s_resourceManager; }

	private:
		void					Render();	// clear scene, set camera and render scene

		std::shared_ptr<Scene> s_mainScene;

        ProgramType_MV_PR_TEX*  s_GLSLProgram;
		ShadowMap*			   s_shadowMap;
		int					   s_ViewPortHeight;
	    int					   s_ViewPortWidth;
		bool				   s_ShadowActivated;
		Vector3				   s_lightPosition;
		Vector3				   s_lightLookAt;
		ResourceManager		   s_resourceManager;
	    
};

}

