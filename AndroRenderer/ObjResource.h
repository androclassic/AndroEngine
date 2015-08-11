#pragma once

#include "../AndroUtils/Resource.h"
#include "../AndroUtils/MMath.h"

namespace andro
{
	class ObjResource : public Resource
	{
	public:

		ObjResource(const std::string &resourcefilenam, void* args = nullptr);
		~ObjResource();

		unsigned int GetVertexCount() const { return m_vertexCount; }
		unsigned int GetVertexBuffer() const { return m_vertexBuffer; }

	private:
		// no copies allowed since classes
		// are referenced
		ObjResource(const ObjResource &foo) : Resource(foo)
		{
		}


		ObjResource &operator =(ObjResource &foo)
		{
			if (this == &foo)
				return *this;
			return *this;
		}


		bool loadOBJ(const char * path);

	private:
		struct buffer_content
		{
			buffer_content() {};
			andro::Vector3 position;
			andro::Vector4 color;
			andro::Vector2 tex;
			andro::Vector3 normal;
		};

		buffer_content* m_data;
		unsigned int	m_vertexCount;
		unsigned int	m_vertexBuffer;

	};
}