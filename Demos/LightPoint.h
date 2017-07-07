#pragma once

#include "../EventManager/EventManager.h"
#include "../AndroRenderer/AndroRenderer.h"
#include "../EventManager/EventManager.h"
#include "../force/core/Force.h"

#include "Sphere.h"



class LightPoint : public andro::EventListener
{
	public:
		LightPoint(andro::Vector3 pos, andro::Vector3 lookAt);
		~LightPoint();

		void SetPositionAndLook(andro::Vector3 pos, andro::Vector3 lookAt);
		void EnableShadow(bool state);
		void EnableLight(bool state);

		void OnEvent(andro::Event *e);
		void Update(float dt);

		

	private:
		Entity*				m_LightObject;	
		andro::Vector3      m_position;
		andro::Vector3		m_lookAt;
		bool				m_light;
		bool				m_shadow;
};