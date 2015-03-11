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

		void		AddObject(shared_ptr<Object> obj);
		void		AddDynamicObject(shared_ptr<DynamicObject> obj);
		bool		RemoveObject(shared_ptr<Object> obj);



		const Camera* GetCamera() const { return m_camera.get(); }
		void SetCamera(std::shared_ptr<Camera> camera);
		

	protected:
		std::vector<shared_ptr<Object> >		    		m_objects;
		std::vector<shared_ptr<DynamicObject> > 			m_dynamicObjects;

		
		std::shared_ptr<Camera>             m_camera;

		GLuint*								m_vbo;
		GLuint*								m_indexVbo;
		GLuint*								m_normalVbo;
		GLuint*								m_texIDs;


		GLuint CreateVertexBuffer(unsigned int index);
		GLuint CreateIndexBuffer(unsigned int index);
		GLuint CreateNormalBuffer(unsigned int index);
	
};

}

