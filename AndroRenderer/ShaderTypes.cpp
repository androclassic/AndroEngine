
#include "ShaderTypes.h"
#include "Engine.h"
#include "../AndroUtils/Trace.h"


namespace andro
{
	 GLSLProgram::GLSLProgram(const string& vertexShader, const string& fragmentShader)
		{
			m_vertexShader.filename = vertexShader;
			m_fragmentShader.filename = fragmentShader;
		}



		void GLSLProgram::unload()
		{
			glDetachShader(m_programID, m_vertexShader.id);
			glDetachShader(m_programID, m_fragmentShader.id);
			glDeleteShader(m_vertexShader.id);
			glDeleteShader(m_fragmentShader.id);
			glDeleteShader(m_programID);
		}

		bssBool GLSLProgram::initialize()
		{
			m_programID = glCreateProgram();
			m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
			m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

			m_vertexShader.source = readFile(m_vertexShader.filename);
			m_fragmentShader.source = readFile(m_fragmentShader.filename);

			if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
			{
				return false;
			}

			const GLchar* tmp = static_cast<const GLchar*>(m_vertexShader.source.c_str());
			glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, NULL);

			tmp = static_cast<const GLchar*>(m_fragmentShader.source.c_str());
			glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, NULL);

			if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
			{
				TRACE(L"Could not compile the shaders, they are invalid\n" );
				return false;
			}

			glAttachShader(m_programID, m_vertexShader.id);
			glAttachShader(m_programID, m_fragmentShader.id);

			glLinkProgram(m_programID);
			
			return true;
		}

		void GLSLProgram::linkProgram()
		{
			glLinkProgram(m_programID);
			outputProgramLog(m_programID);
		}

		bssU32 GLSLProgram::getUniformLocation(const string& name)
		{
			map<string, bssU32>::iterator i = m_uniformMap.find(name);
			if (i == m_uniformMap.end())
			{
				bssU32 location = glGetUniformLocation(m_programID, name.c_str());
				m_uniformMap.insert(std::make_pair(name, location));
				return location;
			}

			return (*i).second;
		}

		bssU32 GLSLProgram::getAttribLocation(const string& name)
		{
			map<string, bssU32>::iterator i = m_attribMap.find(name);
			if (i == m_attribMap.end())
			{
				bssU32 location = glGetAttribLocation(m_programID, name.c_str());
				m_attribMap.insert(std::make_pair(name, location));
				return location;
			}

			return (*i).second;
		}

		void GLSLProgram::sendUniform(const string& name, const int id)
		{
			bssU32 location = getUniformLocation(name);
			glUniform1i(location, id);
		}

		void GLSLProgram::sendUniform4x4(const string& name, const bssFloat* matrix, bssBool transpose)
		{
			bssU32 location = getUniformLocation(name);
			glUniformMatrix4fv(location, 1, transpose, matrix);
		}

		void GLSLProgram::sendUniform3x3(const string& name, const bssFloat* matrix, bssBool transpose)
		{
			bssU32 location = getUniformLocation(name);
			glUniformMatrix3fv(location, 1, transpose, matrix);
		}

		void GLSLProgram::sendUniform(const string& name, const bssFloat red, const bssFloat green,
						 const bssFloat blue, const bssFloat alpha)
		{
			bssU32 location = getUniformLocation(name);
			glUniform4f(location, red, green, blue, alpha);
		}

		void GLSLProgram::sendUniform(const string& name, const bssFloat x, const bssFloat y,
						 const bssFloat z)
		{
			bssU32 location = getUniformLocation(name);
			glUniform3f(location, x, y, z);
		}

		void GLSLProgram::sendUniform(const string& name, const bssFloat scalar)
		{
			bssU32 location = getUniformLocation(name);
			glUniform1f(location, scalar);
		}

		void GLSLProgram::bindAttrib(bssU32 index, const string& attribName)
		{
			glBindAttribLocation(m_programID, index, attribName.c_str());
		}

		void GLSLProgram::bindShader()
		{
			glUseProgram(m_programID);
			outputProgramLog(m_programID);
		}

		string GLSLProgram::readFile(const string& filename)
		{
			ifstream fileIn(filename.c_str());

			if (!fileIn.good())
			{
				TRACE(L"Could not load shader : %S \n", filename.c_str());
				return string();
			}

			string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
			return stringBuffer;
		}

		bssBool GLSLProgram::compileShader(const GLSLShader& shader)
		{
			glCompileShader(shader.id);
			GLint result = 0xDEADBEEF;
			glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

			if (!result)
			{
				TRACE(L"Could not compile shader: %d \n", shader.id );
				outputShaderLog(shader.id);
				return false;
			}

			return true;
		}

		void GLSLProgram::outputShaderLog(bssU32 shaderID)
		{
			vector<char> infoLog;
			GLint infoLen;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
			infoLog.resize(infoLen);

			TRACE(L"GLSL Shader : Shader contains errors, please validate this shader!\n");
			glGetShaderInfoLog(shaderID, infoLog.size(), &infoLen, &infoLog[0]);

			TRACE(L"%S \n ", string(infoLog.begin(), infoLog.end()).c_str() );
		}

		void GLSLProgram::outputProgramLog(bssU32 programID)
		{
			vector<char> infoLog;
			GLint infoLen = 0;
			GLint result = GL_FALSE;
			glGetProgramiv(programID, GL_LINK_STATUS, &result);

			if (result == GL_TRUE) {
				return;
			}

			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen == 0) {
				return;
			}

			infoLog.resize(infoLen);

			TRACE(L"GLSL Shader : Shader contains errors, please validate this shader!\n");
			glGetProgramInfoLog(programID, infoLog.size(), &infoLen, &infoLog[0]);

			TRACE(L"%S \n", string(infoLog.begin(), infoLog.end()).c_str());
		}


	ProgramType_MV_PR_TEX::ProgramType_MV_PR_TEX(const string& vertexShader, const string& fragmentShader)
		: AbstractProgramType(vertexShader, fragmentShader)
	{}

	ProgramType_MV_PR_TEX::~ProgramType_MV_PR_TEX()
	{}

	void ProgramType_MV_PR_TEX::BindAttributeLocations()
	{
		//Bind the attribute locations
		bindAttrib(0, "vVertex");
		bindAttrib(1, "vNormal");
		bindAttrib(2, "vColor");
		bindAttrib(3, "vTextCoord");

	}

	void ProgramType_MV_PR_TEX::UpdateUniforms()
	{
		bssFloat viewMatrix[16];
		bssFloat projectionMatrix[16];
		bssFloat lightViewMatrix[16];

		Camera lightView;
		Vector3 lightPosition = Engine::GetInstance()->GetLightPosition();
		lightView.LookAt(lightPosition,Engine::GetInstance()->GetLightLookAt());

		lightView.GetTransform().LoadVectorFromMatrix(lightViewMatrix);
		Engine::GetInstance()->GetScene()->GetCamera()->GetProjection().LoadVectorFromMatrix(projectionMatrix);


		//Get the current matrices from OpenGL

		Engine::GetInstance()->GetScene()->GetCamera()->GetTransform().LoadVectorFromMatrix(viewMatrix);
		

		//Send the modelview and projection matrices to the shaders
		sendUniform("renderTexture", 1);
		sendUniform("image",  0);

		 sendUniform4x4("projectionMatrix", projectionMatrix);
		sendUniform4x4("viewMatrix", viewMatrix);
		sendUniform4x4("lightViewMatrix", lightViewMatrix);
		
		sendUniform("vLightPosition",lightPosition.x,lightPosition.y,lightPosition.z);
		sendUniform("ambientColor", 0.1f, 0.1f, 0.1f, 1.0f);
		sendUniform("diffuseColor", 0.8f, 0.8f, 0.8f, 1.0f);
		sendUniform("specularColor", 0.5f, 0.5f, 0.5f, 1.0f);
	}

	void ProgramType_MV_PR_TEX::EnableVertexAttributes()
	{
		glEnableVertexAttribArray(0); //Enable the vertex attribute
		glEnableVertexAttribArray(1); //Enable the normal attribute
		glEnableVertexAttribArray(2); //Enable the color attribute
		glEnableVertexAttribArray(3); //Enable the texture coordinate attribute
	}

	void ProgramType_MV_PR_TEX::DisableVertexAttributes()
	{
		glDisableVertexAttribArray(0); 
		glDisableVertexAttribArray(1); 
		glDisableVertexAttribArray(2); 
		glDisableVertexAttribArray(3); 
	}
}