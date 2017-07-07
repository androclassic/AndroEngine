#pragma once

#include "../EventManager/EventManager.h"
#include "../AndroRenderer/AndroRenderer.h"
#include "../EventManager/EventManager.h"
#include "../force/core/Force.h"

#include "Sphere.h"
#include "Box.h"

#include<memory>
using namespace std;

enum ShapeType
{
	e_Sphere,
	e_Box
};


class Spawner : public andro::EventListener
{
	public:
		Spawner(const andro::Camera* camera ,int maxObjects = 100);
		~Spawner();
		void OnEvent(andro::Event *e);
		void Update(float dt);
		

	private:
		Entity*							m_TemporaryObject;		
		const andro::Camera*			m_camera;
		Entity**						m_objects;
		int								m_count;
		int								m_maxCount;
		ShapeType						m_TemporaryShape;
		bool							m_mousemove;
};
