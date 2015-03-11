#pragma once

#include"ShaderTypes.h"
#include "MMath.h"

namespace andro
{

class ShadowMap: public AbstractProgramType
{
public:
	ShadowMap(std::string vertexShader, std::string fragmentShader,Vector2 screenRect,Vector3 lightPos, Vector3 lightLookAt);

	void BindAttributeLocations();
	void UpdateUniforms();
	void EnableVertexAttributes();
	void DisableVertexAttributes();
	bool CreateDepthBuffer();
	void BindFrameBuffer();
	void BindDepthTexture();
	
	void SetScreenRect(Vector2 size){ m_screenWidth = size.x; m_screenHeight = size.y;}
	Vector2 GetScreenRect() const{ return Vector2(m_screenWidth,m_screenHeight );}
	void SetLightPositionDir(Vector3 pos, Vector3 dir) { m_lightPosition = pos; m_lightLookAt = dir;}


private:

	Vector3				m_lightPosition;
	Vector3				m_lightLookAt;

	float				m_screenWidth;
	float				m_screenHeight;
	GLuint				m_renderTexture;
	GLuint				m_frameBufferName;

};


}
