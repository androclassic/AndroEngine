#include "Shader.h"
#include "Log.h"
#include "GL/glew.h"

TakeTwo::Shader::Shader(const std::string &pShaderCode, ShaderType pShaderType)
        : mShaderType(pShaderType), mShaderId(0)
{
	Compile(pShaderCode);
}

TakeTwo::Shader::Shader(Shader&& pOther)
        : mShaderType(std::move(pOther.mShaderType)),
          mShaderId(std::move(pOther.mShaderId))
{
    pOther.mShaderId = 0;
}

TakeTwo::Shader& TakeTwo::Shader::operator=(Shader&& pOther)
{
    if(this != &pOther)
    {
        mShaderType = std::move(pOther.mShaderType);
        mShaderId = std::move(pOther.mShaderId);

        pOther.mShaderId = 0;
    }
    return *this;
}

TakeTwo::Shader::~Shader()
{
}

unsigned int TakeTwo::Shader::GetShaderId() const
{
    return mShaderId;
}

TakeTwo::ShaderType TakeTwo::Shader::GetShaderType() const
{
    return mShaderType;
}

void TakeTwo::Shader::Compile(const std::string& pShaderSource)
{
    //Create Shder
    unsigned int type = GL_VERTEX_SHADER;
    if(mShaderType == ShaderType::FRAGMENT)
    {
        type = GL_FRAGMENT_SHADER;
    }
    mShaderId = glCreateShader(type);

    //Compile Shader
    const char* shaderSource = pShaderSource.c_str();
    glShaderSource(mShaderId, 1, &shaderSource, NULL);
    glCompileShader(mShaderId);

    // Check Shader
    int result = GL_FALSE;
    glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &result);

    int infoLogLength;
    glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 1) //Usually returns 1 because the message is '\0'
    {
        std::string errorMessage;
        errorMessage.reserve(static_cast<unsigned long>(infoLogLength));
        glGetShaderInfoLog(mShaderId, infoLogLength, NULL, &errorMessage[0]);
        LOG_MSG(L"%s\n", errorMessage.c_str());
    }
    if(!result)
    {
        LOG_MSG(L"Shader did not compile!");
    }
}

