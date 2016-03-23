#include "Program.h"
#include "Log.h"
#include "GL/glew.h"

unsigned int TakeTwo::Program::sLoadCounter = 0;



TakeTwo::Program::Program(const std::string& pVertexCode, const std::string& pFragmentCode)
{
    Load(pVertexCode, pFragmentCode);
}

TakeTwo::Program::~Program()
{
    Unload();
}

void TakeTwo::Program::Load(const std::string& pVertexCode, const std::string& pFragmentCode)
{
    Unload();
    //Load shaders
    mFragment = std::make_unique<Shader>(pVertexCode, ShaderType::VERTEX);
    mVertex = std::make_unique<Shader>(pFragmentCode, ShaderType::FRAGMENT);

    //Create and link program
    Link();
}



void TakeTwo::Program::Unload()
{
    if(mProgramId)
    {
        glDeleteProgram(mProgramId);
        mProgramId = 0;
    }
}

void TakeTwo::Program::Use() const
{
	static unsigned int sLastProgramId = -1;

	if (sLastProgramId != mProgramId)
	{
		glUseProgram(mProgramId);
		sLastProgramId = mProgramId;
	}
}

int TakeTwo::Program::GetUniformLocation(const std::string& pName) const
{
    return glGetUniformLocation(mProgramId, pName.c_str());
}

void TakeTwo::Program::Link()
{
    sLoadCounter++;

    //Create the program
    mProgramId = glCreateProgram();

    //Attach shaders and link the program
    glAttachShader(mProgramId, mVertex->GetShaderId());
    glAttachShader(mProgramId, mFragment->GetShaderId());
    glLinkProgram(mProgramId);

    // Check Shader
    int result = GL_FALSE;
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &result);

    int infoLogLength;
    glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 1)//Usually returns 1 because the message is '\0'
    {
        std::string errorMessage;
        errorMessage.reserve(static_cast<unsigned long>(infoLogLength));
        glGetProgramInfoLog(mProgramId, infoLogLength, NULL, &errorMessage[0]);
        LOG_MSG(L"%s\n", errorMessage.c_str());
    }
    if(!result)
    {
        LOG_MSG(L"Program did not link!");
    }
}

