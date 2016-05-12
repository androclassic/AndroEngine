#pragma once
#include <string>

namespace TakeTwo
{
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };

    class Shader
    {
    public:
        Shader(const std::string &pShaderCode, ShaderType pShaderType);
		Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&&);
        Shader& operator=(Shader&&);
        ~Shader();

        unsigned int GetShaderId() const;
        ShaderType GetShaderType() const;

    private:
        void Compile(const std::string& pShaderSource);

        ShaderType mShaderType;
        unsigned int mShaderId;
    };
}
