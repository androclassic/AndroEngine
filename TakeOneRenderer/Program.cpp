#include "Program.h"
#include "Log.h"
#include "GL/glew.h"

unsigned int TakeOne::Program::sLoadCounter = 0;



TakeOne::Program::Program(const std::string& pVertexPath, const std::string& pFragmentPath)
	:Resource(std::string(pVertexPath + pFragmentPath).c_str(), NULL)
{
    Load(pVertexPath, pFragmentPath);
}

TakeOne::Program::Program(const std::string& pResource_key, void *pArgs)
	: Resource(pResource_key.c_str(), NULL)
{
	ProgramArgs* args = (ProgramArgs*)pArgs;
	Load(args->mVertexPath, args->mFragmentPath);
}


TakeOne::Program::~Program()
{
    Unload();
}

void TakeOne::Program::Load(const std::string& pVertexPath, const std::string& pFragmentPath)
{
    Unload();
    //Load shaders
    mFragment = std::make_unique<Shader>(pVertexPath, ShaderType::VERTEX);
    mVertex = std::make_unique<Shader>(pFragmentPath, ShaderType::FRAGMENT);

    //Create and link program
    Link();
}

void TakeOne::Program::Reload()
{
    Unload();
    mVertex->Reload();
    mFragment->Reload();
    Link();
}

void TakeOne::Program::Unload()
{
    if(mProgramId)
    {
        glDeleteProgram(mProgramId);
        mProgramId = 0;
    }
}

void TakeOne::Program::Use() const
{
	static unsigned int sLastProgramId = -1;

	if (sLastProgramId != mProgramId)
	{
		glUseProgram(mProgramId);
		sLastProgramId = mProgramId;
	}
}

int TakeOne::Program::GetUniformLocation(const std::string& pName) const
{
    return glGetUniformLocation(mProgramId, pName.c_str());
}

void TakeOne::Program::Link()
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

