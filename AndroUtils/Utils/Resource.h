#pragma once
#include "Trace.h"

namespace andro
{
	class Resource
	{
		friend class  ResourceManager;

	public:
		Resource(const std::string& resourcefilename, void *args)
		{
			if (resourcefilename.empty())
				TRACE(L"Empty filename not allowed");


			References = 0;
			ResourceFileName = resourcefilename;
			to_lower(ResourceFileName);

		}

		virtual ~Resource()
		{
		}

		const std::string &GetResourceFileName() const
		{
			return ResourceFileName;
		}

		const int GetReferencesCount() const
		{
			return References;
		}

	private:

		int References;

		void IncReferences() { References++; }
		void DecReferences() { References--; }

	protected:
		Resource(const Resource& object) { }
		Resource& operator=(const Resource& object) { return *this; }

		std::string ResourceFileName;
	};

}
