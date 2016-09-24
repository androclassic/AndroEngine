#pragma once
#include "Resource.h"
#include "ResourceManager.h"
#include <string>

namespace andro
{
		class TextFile : public andro::Resource
		{
			public:
				TextFile(const std::string& resourcefilename);

				const std::string& GetContent() const { return m_text;  }
		private:
				TextFile(const std::string& resourcefilename, void *args);
				std::string LoadTextFile(const std::string& filePath);
				std::string m_text;

				friend class andro::ResourceManager;
		};
}
