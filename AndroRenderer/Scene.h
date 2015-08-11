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
	
		void		Update(float dt); // update all dynamic objects
		void		Render(GLSLProgram* shader,bool cullFront); // render all object and dynamic objects

		void		AddDynamicObject(shared_ptr<DynamicObject> obj);
		bool		RemoveObject(shared_ptr<DynamicObject> obj);



		const Camera* GetCamera() const { return m_camera.get(); }
		void SetCamera(std::shared_ptr<Camera> camera);
		

	protected:
		std::vector<shared_ptr<DynamicObject> > 			m_dynamicObjects;
		std::shared_ptr<Camera>             m_camera;

		unsigned int*								m_vbo;
		unsigned int*								m_indexVbo;
		unsigned int*								m_normalVbo;
		unsigned int*								m_texIDs;


		unsigned int CreateVertexBuffer(unsigned int index);
		unsigned int CreateIndexBuffer(unsigned int index);
		unsigned int CreateNormalBuffer(unsigned int index);
	
};

}

