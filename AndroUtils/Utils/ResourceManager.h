#pragma once
#include "Resource.h"

#ifdef _WIN32
	#include <unordered_map>
	using namespace std;
#else
	#include <tr1/unordered_map>
	using namespace std::tr1;
#endif

namespace andro
{
	class ResourceManager
	{
	public:

		template <typename T >
		T *Load(const std::string &filename, void *args)
		{

			if (filename.empty())
				TRACE(L"filename cannot be null");

			std::string FileName = filename;
			to_lower(FileName);

			unordered_map< std::string, Resource* >::iterator it = Map.find(FileName);
			if (it != Map.end())
			{

				(*it).second->IncReferences();

				return (T*)(*it).second;

			}


			T *resource = new T(FileName, args);
			resource->IncReferences();
			Map.insert(std::pair< std::string, Resource* >(FileName, resource));

			return resource;

		}

		bool Unload(const std::string &filename)
		{
			if (filename.empty())
				TRACE(L"Filename cannot be null");

			std::string FileName = filename;
			to_lower(FileName);

			unordered_map< std::string, Resource* >::iterator it = Map.find(FileName);
			if (it != Map.end())
			{
				(*it).second->DecReferences();

				if ((*it).second->GetReferencesCount() == 0)
				{
					// call the destructor 
					delete((*it).second);
					Map.erase(it);
				}
				return true;
			}

			TRACE(L"cannot find %s\n", FileName.c_str());
			return false;
		}

		void Initialise(const std::string &name)
		{
			if (name.empty())
				TRACE(L"Null name is not allowed");

			Name = name;
			to_lower(Name);

		}

		const std::string &GetName() const { return Name; }
		const int Size() const { return Map.size(); }

		ResourceManager()
		{

		}

		~ResourceManager()
		{
			ReleaseAll();
		}
	private:

		unordered_map< std::string, Resource* > Map;
		std::string	Name;
		void ReleaseAll()
		{
			unordered_map< std::string, Resource* >::iterator it = Map.begin();

			while (it != Map.end())
			{
				delete (*it).second;

				it = Map.erase(it);
			}

		}


		ResourceManager(const ResourceManager&) { };
		ResourceManager &operator = (const ResourceManager&) { return *this; }

	};
}
