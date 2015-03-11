#include"Scene.h"

#include "../AndroUtils/Trace.h"

namespace andro
{
	Scene::Scene()
	{
		m_objects.clear();
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

	void Scene::AddObject(shared_ptr<Object> obj)
	{
	   m_objects.push_back(obj);
	}
	void Scene::AddDynamicObject(shared_ptr<DynamicObject>obj)
	{
	   m_dynamicObjects.push_back(obj);
	}

	bool Scene::RemoveObject(shared_ptr<Object> obj)
	{
		int index = -1;
		for (unsigned int i = 0; i < m_objects.size(); i++)
			if (m_objects[i].get() == obj.get())
				{
					m_objects.erase(m_objects.begin() + i);
					return true;
				}
		for(unsigned int i=0; i < m_dynamicObjects.size(); i++)
			if (m_dynamicObjects[i].get() == obj.get())
				{
					m_dynamicObjects.erase(m_dynamicObjects.begin() + i);
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
			for(unsigned int i=0;i<m_objects.size();i++)
		{
				m_objects[i]->SetTexture();
	    		m_objects[i]->Render(shader, cullFront);
		}
			

	}


}