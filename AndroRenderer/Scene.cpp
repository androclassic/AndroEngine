#include"Scene.h"

#include "../AndroUtils/Trace.h"

namespace andro
{
	Scene::Scene()
	{
		m_dynamicObjects.clear();
		m_vbo		= NULL;
		m_indexVbo	= NULL;
		m_texIDs	= NULL;
		m_normalVbo = NULL;

		m_camera = std::make_shared<Camera>();
	}



	Scene::~Scene()
	{
	}

	void Scene::SetCamera(std::shared_ptr<Camera> camera)
	{	
		m_camera = camera;
	}

	void Scene::AddDynamicObject(shared_ptr<DynamicObject>obj)
	{
	   m_dynamicObjects.push_back(obj);
	}

	bool Scene::RemoveObject(shared_ptr<DynamicObject> obj)
	{
		int index = -1;
		for(unsigned int i=0; i < m_dynamicObjects.size(); i++)
			if (m_dynamicObjects[i].get() == obj.get())
				{
					m_dynamicObjects.erase(m_dynamicObjects.begin() + i);
					return true;
				}
		return false;
	}

	void Scene::Update(float dt)
	{
		 
		for(unsigned int i=0;i<m_dynamicObjects.size();i++)
		{
				  
			m_dynamicObjects[i]->Update(dt);
		}
	}

	void Scene::Render(GLSLProgram* shader,bool cullFront) 
	{
		m_camera->Set();

		for(unsigned int i=0;i<m_dynamicObjects.size();i++)
		{
			m_dynamicObjects[i]->SetTexture();
			m_dynamicObjects[i]->Render(shader, cullFront);
		}

	}


}