#pragma once

#include "../../AndroUtils/EventManager/EventManager.h"
#include "../../TakeOneRenderer/CameraNode.h"

#include <memory>
#include "GLFW\glfw3.h"

class Camera : public andro::EventListener
{
private:
	bool m_mousemove;
	std::shared_ptr<TakeOne::CameraNode> m_camera;

public:
	Camera();
	void OnEvent(andro::Event *e);
	std::shared_ptr<TakeOne::CameraNode> GetCamera() const { return m_camera; }
	void Move(float dx, float dz);

};


