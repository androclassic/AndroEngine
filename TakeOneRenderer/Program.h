#pragma once

#include "Shader.h"
#include <string>
#include <memory>
#include "../../AndroUtils/Utils/Resource.h"

namespace TakeOne
{

    class Program : andro::Resource
    {
    public:
        Program(const std::string& pVertexPath, const std::string& pFragmentPath);
        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;
        ~Program();

        void Load(const std::string& pVertexPath, const std::string& pFragmentPath);
        void Reload();
        void Unload();
        void Use() const;
        int GetUniformLocation(const std::string& pName) const;

        static inline unsigned int GetLoadCounter() { return sLoadCounter; }
    private:
		Program(const std::string& pResource_key, void *args);
		
		void Link();

        std::unique_ptr<Shader> mVertex;
        std::unique_ptr<Shader> mFragment;
        unsigned int mProgramId;

        static unsigned int sLoadCounter; //used as unique id to optimise binding

		friend class  andro::ResourceManager;

	public:
		struct ProgramArgs //used for ResourceManager
		{
			ProgramArgs(const std::string& pVertexPath, const std::string& pFragmentPath) : mVertexPath(pVertexPath), mFragmentPath(pFragmentPath)
			{}

			const std::string& mVertexPath;
			const std::string& mFragmentPath;
		};

    };
}
