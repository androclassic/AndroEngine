#pragma once

#include "Shader.h"
#include <string>
#include <memory>
#include "../../AndroUtils/Utils/Resource.h"
#include "../AndroUtils/Utils/TextFileLoader.h"

namespace TakeTwo
{

    class Program 
    {
    public:
        Program(const std::string& pVertexCode, const std::string& pFragmentCode);
        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;
        ~Program();

        void Load(const std::string& pVertexCode, const std::string& pFragmentCode);
        void Unload();
        void Use() const;
        int GetUniformLocation(const std::string& pName) const;

        static inline unsigned int GetLoadCounter() { return sLoadCounter; }
    private:
		void Link();

        std::unique_ptr<Shader> mVertex;
        std::unique_ptr<Shader> mFragment;
        unsigned int mProgramId;

        static unsigned int sLoadCounter; //used as unique id to optimise binding
    };
}
