#include "Engine.h"
#include "ShaderTypes.h"
#include "../AndroUtils/Trace.h"


namespace andro
{

	bool Engine::Init(const char* vertexShader,const char* fragmentShader)
	{

			glEnable(GL_DEPTH_TEST); 
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_TEXTURE_2D);
			//glEnable(GL_POLYGON_SMOOTH);
			//glEnable(GL_LINE_SMOOTH);
			glEnable(GL_MULTISAMPLE);
			glFrontFace(GL_CW);
			glEnable(GL_CULL_FACE);

			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			 //Viewport[2] stores the width of the viewport, vieport[3] stores the height
			//We pass these into our font so the ortho mode can set the resolution for the window
			s_ViewPortWidth=viewport[2];
			s_ViewPortHeight=viewport[3];

			glClearColor(0.8f,0.0f,0.3f,0.6f);

			glewInit();

			s_resourceManager.Initialise("RendererResourceManager");

			//initialize shader
			s_GLSLProgram = new ProgramType_MV_PR_TEX(vertexShader, fragmentShader);
			if(!s_GLSLProgram->initialize() )
			{
				TRACE(L"GLSLProgram failed to initialize\n");
				return false;
			}
			s_GLSLProgram->BindAttributeLocations();
			s_GLSLProgram->linkProgram();


			s_mainScene = std::make_shared<Scene>();
			//set MappedFont
			//if (!s_mappedFont->initialize()) {
			//	std::cerr << "Could not initialize the font" << std::endl;
			//	return false;
			//}
			 return true;

	}
	void Engine::SetLightPositionDir(Vector3 pos, Vector3 dir) 
	{
		s_lightPosition = pos; 
		s_lightLookAt = dir;
		if(s_ShadowActivated)
			s_shadowMap->SetLightPositionDir(pos,dir);
	}

	bool Engine::EnableShadow(bool state)
	{
		  s_ShadowActivated = state;
		
		  if(s_shadowMap && state == false)
		  {
			s_shadowMap->BindFrameBuffer();
			glClear(GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		  }

		  if(s_shadowMap  == NULL)
		  {
		  GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			 //Viewport[2] stores the width of the viewport, vieport[3] stores the height
			//We pass these into our font so the ortho mode can set the resolution for the window
			s_ViewPortWidth=viewport[2];
			s_ViewPortHeight=viewport[3];

			s_shadowMap = new ShadowMap("data/shadowmap.vert","data/shadowmap.frag",Vector2(viewport[2],viewport[3]),s_lightPosition,s_lightLookAt);
			if(!s_shadowMap->initialize())
			{
				TRACE(L"ShadowMap failed to initialize\n");
				return false;
			}
			if (!s_shadowMap->CreateDepthBuffer())
			{
				TRACE(L"ShadowMap failed to CreateDepthBuffer\n");
				return false;
			}
			s_shadowMap->BindAttributeLocations();
			s_shadowMap->linkProgram();
			return true;
		  }
		  return false;
	}

	void Engine::Resize(int width, int height)
	{
	  //Prevent a divide by zero error
		if (height <= 0)
		{
			height = 1;
		}

		s_ViewPortHeight=height;
		s_ViewPortWidth=width;
		
		if(s_ShadowActivated)
		{
			s_shadowMap->SetScreenRect(Vector2((float)width, (float)height));
			if (!s_shadowMap->CreateDepthBuffer())
				TRACE(L"ShadowMap failed to CreateDepthBuffer\n");
		}

		//When we resize the window, we tell OpenGL about the new viewport size
		glViewport(0,0, (GLsizei)width, (GLsizei)height);

	}

	void Engine::Update(float dt)
	{
	  s_mainScene->Update(dt);
	  Render();
	}

	void Engine::Render()
	{
		if(s_ShadowActivated)
		{
			s_shadowMap->BindFrameBuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			s_shadowMap->bindShader(); //Enable our shader
		
			s_shadowMap->EnableVertexAttributes();
			s_shadowMap->UpdateUniforms();

			s_mainScene->Render(s_shadowMap, false);
		
			s_shadowMap->DisableVertexAttributes();

			glBindFramebuffer(GL_FRAMEBUFFER , 0 );
		}

		s_GLSLProgram->bindShader(); //Enable our shader
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		if(s_ShadowActivated)
			s_shadowMap->BindDepthTexture();

		s_GLSLProgram->EnableVertexAttributes();
		s_GLSLProgram->UpdateUniforms();

		s_mainScene->Render(s_GLSLProgram, true);

		s_GLSLProgram->DisableVertexAttributes();

	}  


	Engine::Engine() : s_shadowMap(NULL)
	{
		s_ShadowActivated = false;
		s_lightLookAt = Vector3();
		s_lightPosition = Vector3();
	}

	Engine::~Engine()
	{
		if (s_GLSLProgram != NULL)
		{
			delete s_GLSLProgram;
		}
	}
}