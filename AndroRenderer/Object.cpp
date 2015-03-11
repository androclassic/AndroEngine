#include"Object.h"
#include <iostream>


#include "../AndroUtils/Trace.h"


namespace andro
	{
	Object::Object() 
	{
		
		m_roll=0; 
		m_pitch=0; 
		m_heading=0; 
		m_vertexBuffer=0;
		m_indexBuffer=0;
		m_normalBuffer=0;
		m_normalsCount = 0;
		m_vertices=NULL;
		m_normals = NULL;
		m_WorldMatrix.LoadIdentity();
		m_loadedObj = false;
		m_scale = Vector3(1,1,1);
	  
	}
	Object::Object(Vector3 position,float size) 
	{  
		m_roll=0; 
		m_pitch=0; 
		m_heading=0; 
		m_vertexBuffer=0;
		m_indexBuffer=0;
		m_vertices=NULL;
		m_size=size;
		m_WorldMatrix.LoadIdentity();
		m_scale = Vector3(1,1,1);
		m_position = position;
		m_loadedObj = false;
		
		Translate(m_WorldMatrix,m_position.x,m_position.y,m_position.z);
		Scale(m_WorldMatrix,m_scale.x,m_scale.y,m_scale.z);
		RotatePitch(m_WorldMatrix,m_pitch);
		RotateYaw(m_WorldMatrix,m_heading);
		RotateRoll(m_WorldMatrix,m_roll);
		
	}

	

	Object::~Object()
	{
	  if(m_vertices!=NULL)
	  {
		  delete[] m_vertices;
		  m_vertices=NULL;
	  }
		
	}

	void Object::SetPosition(Vector3 pos)
	{
		m_position.x = pos.x;
		m_position.y = pos.y;
		m_position.z = pos.z;
		SetPos(m_WorldMatrix,m_position.x,m_position.y,m_position.z);
	}

	void Object::SetScale(float x, float y, float z)
	{
		m_scale.x=x;
		m_scale.y=y;
		m_scale.z=z;

		m_WorldMatrix.LoadIdentity();
		Scale(m_WorldMatrix,m_scale.x,m_scale.y,m_scale.z);
		RotatePitch(m_WorldMatrix,m_pitch);
		RotateYaw(m_WorldMatrix,m_heading);
		RotateRoll(m_WorldMatrix,m_roll);
		SetPos(m_WorldMatrix,m_position.x,m_position.y,m_position.z);

	}


	void Object::SetRoll(float roll)
	{
	  m_roll=roll; 
	  m_roll = fmod(m_roll, 360);

	  m_WorldMatrix.LoadIdentity(); 
 	  Scale(m_WorldMatrix,m_scale.x,m_scale.y,m_scale.z);
	  RotateYaw(m_WorldMatrix,m_heading);
	  RotatePitch(m_WorldMatrix,m_pitch);
	  RotateRoll(m_WorldMatrix,m_roll);
	  SetPos(m_WorldMatrix,m_position.x,m_position.y,m_position.z);

	}

	void Object::SetPitch(float pitch)
	{
	  m_pitch=pitch; 
	  m_pitch = fmod(m_pitch, 360);

	  m_WorldMatrix.LoadIdentity(); 
 	  Scale(m_WorldMatrix,m_scale.x,m_scale.y,m_scale.z);
	  RotateYaw(m_WorldMatrix,m_heading);
	  RotatePitch(m_WorldMatrix,m_pitch);
	  RotateRoll(m_WorldMatrix,m_roll);
	  SetPos(m_WorldMatrix,m_position.x,m_position.y,m_position.z);

	}
	void Object::SetHeading(float heading)
	{
	  m_heading=heading; 
	  m_heading = fmod(m_heading, 360);

	  m_WorldMatrix.LoadIdentity(); 
 	  Scale(m_WorldMatrix,m_scale.x,m_scale.y,m_scale.z);
	  RotateYaw(m_WorldMatrix,m_heading);
	  RotatePitch(m_WorldMatrix,m_pitch);
	  RotateRoll(m_WorldMatrix,m_roll);
	  SetPos(m_WorldMatrix,m_position.x,m_position.y,m_position.z);
	}

	void Object::Render(GLSLProgram *shader, bool cullFront) const
	{

		float worldMatrix[16];

		
		m_WorldMatrix.LoadVectorFromMatrix( worldMatrix);
	 
		//Send the world_matrix  to the shaders
		shader->sendUniform4x4("worldMatrix", worldMatrix);

		
		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);

 		glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex3fPos4fColor2fTex), 0);
 		glVertexAttribPointer((GLint)2, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(float)*3));
		glVertexAttribPointer((GLint)3, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(float)*7));

		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE,sizeof(Vector3), 0);
		

		if(m_loadedObj)
		{
			if(cullFront)
			{
				glFrontFace(GL_CCW);
				glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
				glDrawArrays(GL_TRIANGLES,0,m_verticesCount);
				glFrontFace(GL_CW);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);
				glDrawArrays(GL_TRIANGLES,0,m_verticesCount);
			}
		}
		else
		{
			if(cullFront)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);	
				glDrawElements(GL_TRIANGLE_STRIP, m_index.size(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glFrontFace(GL_CCW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);	
				glDrawElements(GL_TRIANGLE_STRIP, m_index.size(), GL_UNSIGNED_INT, 0);
				glFrontFace(GL_CW);
			}
		}

	}


	Vector3 Object::Look() const
	{
		return this->m_WorldMatrix.CLook;
	}

	Vector3 Object::Right() const
	{
		return this->m_WorldMatrix.CRight;
	}

	Vector3 Object::Up() const
	{
		return this->m_WorldMatrix.CUp;
	}
	
	void Object::SetShapeColor(Vector3 color)
	{
		for(unsigned int i=0; i < m_verticesCount; i++)
		{
			m_vertices[i].r = color.x;
			m_vertices[i].g = color.y;
			m_vertices[i].b = color.z;
		}
		CleanOpenglBuffers();
		GenerateOpenglBuffers();
	}

	void Vertex3fPos4fColor2fTex::Set(float x, float y, float z, float r , float g , float b, float a, float s, float t)
	{
		this->x=x;
		this->y=y;
		this->z=z;

		this->r=r;
		this->g=g;
		this->b=b;
		this->a=a;

		this->s=s;
		this->t=t;
	}

	void Object::SetMatrixData(float data[])
	{
		this->m_WorldMatrix.LoadMatrixFromVector(data);
	}
	void Object::GetMatrixData(float data[]) const 
	{
		this->m_WorldMatrix.LoadVectorFromMatrix(data);
	}

	void Object::CleanBuffers()
	{
		if(m_vertices!=NULL)
		  {
			  delete[] m_vertices;
			  m_vertices=NULL;

			  if(m_normals != NULL)
				{
					delete[] m_normals;
					m_normals=NULL;
				}
		  }

		m_verticesCount = m_normalsCount = 0;
		m_index.clear();
		CleanOpenglBuffers();
	}

	void Object::CleanOpenglBuffers()
	{
		if (m_vertexBuffer)
			glDeleteBuffers(1, &m_vertexBuffer);
		if (m_normalBuffer)
			glDeleteBuffers(1, &m_normalBuffer);
		if (m_indexBuffer)
			glDeleteBuffers(1, &m_indexBuffer);
	}


	void Object::GenerateOpenglBuffers()
	{
	    glGenBuffers(1,&m_vertexBuffer); 
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3fPos4fColor2fTex) * m_verticesCount, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

		if(m_normalsCount)
		{
			glGenBuffers(1,&m_normalBuffer); 
			glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer); //Bind the normal buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_normalsCount, &m_normals[0], GL_STATIC_DRAW); //Send the data to OpenGL
		}
		if(m_index.size())
		{
			glGenBuffers(1,&m_indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer); //Bind the index buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_index.size(), &m_index[0], GL_STATIC_DRAW); //Send the data to OpenGL
		}
	}

	bool Object::loadOBJ(const char * path )
	{

		CleanBuffers();

		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< Vector3> temp_vertices;
		std::vector< Vector2 > temp_uvs;
		std::vector< Vector3 > temp_normals;

		FILE * file = NULL;
		VERIFY(fopen_s(&file, path, "r") == 0);
		
		if( file == NULL )
		{
			TRACE(L"Impossible to open the file !\n");
			return false;
		}

		while( 1 )
		{
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf_s(file, "%s", lineHeader,128);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.
 
			// else : parse lineHeader
			if ( strcmp( lineHeader, "v" ) == 0 )
			{
				Vector3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if ( strcmp( lineHeader, "vt" ) == 0 )
			{
				Vector2 uv;
				fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if ( strcmp( lineHeader, "vn" ) == 0 )
			{
				Vector3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if ( strcmp( lineHeader, "f" ) == 0 )
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
				uvIndices    .push_back(uvIndex[0]);
				uvIndices    .push_back(uvIndex[1]);
				uvIndices    .push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
			m_normalsCount = m_verticesCount =  vertexIndices.size();
			m_vertices = new andro::Vertex3fPos4fColor2fTex[ m_verticesCount ];
			m_normals = new Vector3[m_normalsCount];

		}
		
			fclose(file);

		// For each vertex of each triangle
			for( unsigned int i=0; i<vertexIndices.size(); i++ )
			{
				unsigned int vertexIndex = vertexIndices[i];
				Vector3 vertex = temp_vertices[ vertexIndex-1 ];
	
				m_vertices[i].x = vertex.x;
				m_vertices[i].y = vertex.y;
				m_vertices[i].z = vertex.z;

				m_vertices[i].r = 0;
				m_vertices[i].g = 0;
				m_vertices[i].b = 0;

	
			}
			for( unsigned int i=0; i < uvIndices.size(); i++ )
			{ 
				unsigned int uvIndex = uvIndices[i];
				Vector2 tex = temp_uvs[ uvIndex-1 ];

				m_vertices[i].s = tex.x;
				m_vertices[i].t = tex.y;

			}
			for( unsigned int i=0; i<normalIndices.size(); i++ )
			{
				unsigned int vertexIndex = normalIndices[i];
				m_normals[i] = temp_normals[ vertexIndex-1 ];
			}
		

		GenerateOpenglBuffers();
		m_loadedObj = true;

		return true;
	}
		
}