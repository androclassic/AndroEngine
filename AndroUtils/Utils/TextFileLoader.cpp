#include "TextFileLoader.h"
#include <fstream>
#include <sstream>

andro::TextFile::TextFile(const std::string & resourcefilename)
	: andro::Resource(resourcefilename, NULL)
{
	m_text = LoadTextFile(resourcefilename);
}

andro::TextFile::TextFile(const std::string & resourcefilename, void * args)
	: andro::Resource(resourcefilename, NULL)
{
	m_text = LoadTextFile(resourcefilename);
}

std::string andro::TextFile::LoadTextFile(const std::string& filePath)
{
    std::ifstream fileStream(filePath);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}
