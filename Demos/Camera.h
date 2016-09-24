#pragma once

#include "../../AndroUtils/EventManager/EventManager.h"
#include "../../Renderer/CameraNode.h"
#include "../../AndroUtils/Utils/AndroTypes.h"
#include <memory>

#include "GLFW/glfw3.h"

class Camera : public andro::EventListener
{
private:
	bool m_mousemove;
	std::shared_ptr<TakeTwo::CameraNode> m_camera;

public:
	Camera();
	void OnEvent(andro::Event *e);
	std::shared_ptr<TakeTwo::CameraNode> GetCamera() const { return m_camera; }
	void Move(afloat dx, afloat dz);

};


