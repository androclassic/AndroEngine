#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Singleton.h"
#include "ShadowMap.h"

namespace andro
{
class Engine // same as Example class
{
	SINGLETON(Engine);
	public:
		bssBool			Init(const char* vertexShader, const char* fragmentShader);
		void			Update(bssFloat dt);	// update scene

		void			Resize(int width, int height);	// like Example Resize
		void			AddDynamicModel(shared_ptr<DynamicObject> o, const char* modelname, const char* texturename);

		void			SetClearColor(bssFloat r, bssFloat g, bssFloat b, bssFloat a){ glClearColor(r, g, b, a); }
		void			SetLightPositionDir(Vector3 pos, Vector3 dir);

		bssBool			EnableShadow(bssBool state);
	    Scene*			GetScene() const {return s_mainScene.get();}
		Vector3			GetLightPosition() const {return s_lightPosition;}
		Vector3			GetLightLookAt() const {return s_lightLookAt;}
		


	private:
		void					Render();	// clear scene, set camera and render scene

		std::shared_ptr<Scene> s_mainScene;

        ProgramType_MV_PR_TEX*  s_GLSLProgram;
		ShadowMap*			   s_shadowMap;
		int					   s_ViewPortHeight;
	    int					   s_ViewPortWidth;
		bssBool				   s_ShadowActivated;
		Vector3				   s_lightPosition;
		Vector3				   s_lightLookAt;
	    
};

}

