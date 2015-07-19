#include "ShadowMap.h"
#include "Engine.h"

namespace andro
{

	ShadowMap::ShadowMap(std::string vertexShader, std::string fragmentShader,Vector2 screenRect,Vector3 lightPos, Vector3 lightLookAt)
	: AbstractProgramType(vertexShader, fragmentShader)
	{
		m_screenWidth = screenRect.x;
		m_screenHeight = screenRect.y;
		m_lightPosition = lightPos;
		m_lightLookAt = lightLookAt;
	
	}

	void ShadowMap::UpdateUniforms()
	{
		Camera lightView;
		lightView.LookAt(m_lightPosition, m_lightLookAt);

		//Send the modelview and projection matrices to the shaders
		sendUniform4x4("projectionMatrix", Engine::GetInstance()->GetScene()->GetCamera()->GetProjection().data);
		sendUniform4x4("viewMatrix", lightView.GetTransform().data);

	}

		void ShadowMap::BindAttributeLocations()
	{
		//Bind the attribute locations
		bindAttrib(0, "vVertex");
	}


	void ShadowMap::BindFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,m_frameBufferName);

	}

	void ShadowMap::BindDepthTexture()
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, m_renderTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	}


	void ShadowMap::EnableVertexAttributes()
	{
		glEnableVertexAttribArray(0); //Enable the vertex attribute
	}

	void ShadowMap::DisableVertexAttributes()
	{
		glDisableVertexAttribArray(0); 
	}

	 bssBool ShadowMap::CreateDepthBuffer()
	{
				// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer
		 glGenFramebuffers(1, &m_frameBufferName);
		 glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferName);
 
		 // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		 glGenTextures(1, &m_renderTexture);
		 glBindTexture(GL_TEXTURE_2D, m_renderTexture);
		 glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, (size_t)m_screenWidth, (size_t)m_screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_renderTexture, 0);
		glDrawBuffer(GL_NONE); // No color buffer is drawn to.
		glReadBuffer(GL_NONE);
	
		bssU32 ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		// Always check that our framebuffer is ok
		if(ret != GL_FRAMEBUFFER_COMPLETE)
			return false;

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	return true;
	}
}