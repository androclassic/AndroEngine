#pragma once

#include "DynamicObject.h"
#include "Camera.h"

#include <vector>
#include<time.h>
#include<memory>

namespace andro
{

class Scene
{
	public:
		Scene();
		~Scene();
	
		void		Update(bssFloat dt); // update all dynamic objects
		void		Render(GLSLProgram* shader,bssBool cullFront); // render all object and dynamic objects

		void		AddDynamicObject(shared_ptr<DynamicObject> obj);
		bssBool		RemoveObject(shared_ptr<DynamicObject> obj);



		const Camera* GetCamera() const { return m_camera.get(); }
		void SetCamera(std::shared_ptr<Camera> camera);
		

	protected:
		std::vector<shared_ptr<DynamicObject> > 			m_dynamicObjects;
		std::shared_ptr<Camera>             m_camera;

		bssU32*								m_vbo;
		bssU32*								m_indexVbo;
		bssU32*								m_normalVbo;
		bssU32*								m_texIDs;


		bssU32 CreateVertexBuffer(bssU32 index);
		bssU32 CreateIndexBuffer(bssU32 index);
		bssU32 CreateNormalBuffer(bssU32 index);
	
};

}

