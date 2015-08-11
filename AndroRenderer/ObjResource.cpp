#include <windows.h>
#include <GL\glew.h>


#include "ObjResource.h"
#include "../AndroUtils/MMath.h"

#include <vector>

namespace andro
{

ObjResource::ObjResource(const std::string & resourcefilename, void* args)
	: Resource(resourcefilename, args)
	, m_data(nullptr)
	, m_vertexCount(0)
{
	VERIFY( loadOBJ(resourcefilename.c_str()) );
}

ObjResource::~ObjResource()
{
	if (m_data != nullptr)
		delete m_data;

	if (m_vertexBuffer)
		glDeleteBuffers(1, &m_vertexBuffer);

}

bool ObjResource::loadOBJ(const char * path)
{


	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< Vector3> temp_vertices;
	std::vector< Vector2 > temp_uvs;
	std::vector< Vector3 > temp_normals;

	FILE * file = NULL;
	VERIFY(fopen_s(&file, path, "r") == 0);

	if (file == NULL)
	{
		TRACE(L"Impossible to open the file !\n");
		return false;
	}

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf_s(file, "%s", lineHeader, 128);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0)
		{
			Vector3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			Vector2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			Vector3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				TRACE(L"File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}

	}

	 m_vertexCount = vertexIndices.size();
	 m_data = new buffer_content[m_vertexCount];


	fclose(file);

	// For each vertex of each triangle
	for (unsigned int i = 0; i < m_vertexCount; i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		Vector3 vertex = temp_vertices[vertexIndex - 1];

		m_data[i].position.x = vertex.x;
		m_data[i].position.y = vertex.y;
		m_data[i].position.z = vertex.z;

		m_data[i].color = Vector3(0.8, 0, 0);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		unsigned int uvIndex = uvIndices[i];
		Vector2 tex = temp_uvs[uvIndex - 1];

		m_data[i].tex.x = tex.x;
		m_data[i].tex.y = tex.y;

	}
	for (unsigned int i = 0; i<normalIndices.size(); i++)
	{
		unsigned int vertexIndex = normalIndices[i];
		m_data[i].normal.x = temp_normals[vertexIndex - 1].x;
		m_data[i].normal.y = temp_normals[vertexIndex - 1].y;
		m_data[i].normal.z = temp_normals[vertexIndex - 1].z;
	}

	//GenerateOpenglBuffers();
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12 * m_vertexCount, m_data, GL_STATIC_DRAW); //Send the data to OpenGL

	return true;
}
}
