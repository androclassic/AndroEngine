#include"DynamicObject.h"
#include <iostream>
#include"ShaderTypes.h"

#include "../AndroUtils/Trace.h"
#include "SOIL.h"


#include <iostream>
#include "../AndroUtils/Trace.h"
namespace andro
{


	DynamicObject::DynamicObject()
		:m_rollSpeed(0)
		, m_pitchSpeed(0)
		, m_headingSpeed(0)
		, m_roll(0)
		, m_pitch(0)
		, m_heading(0)
		, m_vertexBuffer(0)
		, m_indexBuffer(0)
		, m_vertices(NULL)
		, m_normals(NULL)
		, m_loadedObj(false)
		, m_size(1)
		, m_position()
	{
		m_WorldMatrix.LoadIdentity();
		m_scale = Vector3(1, 1, 1);
	}

	DynamicObject::DynamicObject(Vector3 position, bssFloat size)
		:m_rollSpeed(0)
		, m_pitchSpeed(0)
		, m_headingSpeed(0)
		, m_roll(0)
		, m_pitch(0)
		, m_heading(0)
		, m_vertexBuffer(0)
		, m_indexBuffer(0)
		, m_vertices(NULL)
		, m_normals(NULL)
		, m_loadedObj(false)
		, m_size(size)
		, m_position(position)
	{
		m_scale = Vector3(1, 1, 1);
		m_WorldMatrix.LoadIdentity();

		Translate(m_WorldMatrix, m_position.x, m_position.y, m_position.z);
		Scale(m_WorldMatrix, m_scale.x, m_scale.y, m_scale.z);
		RotatePitch(m_WorldMatrix, m_pitch);
		RotateYaw(m_WorldMatrix, m_heading);
		RotateRoll(m_WorldMatrix, m_roll);
	}


	DynamicObject::~DynamicObject()
	{
		CleanBuffers();

	}

	void DynamicObject::SetPosition(Vector3 pos)
	{
		m_position.x = pos.x;
		m_position.y = pos.y;
		m_position.z = pos.z;
		SetPos(m_WorldMatrix, m_position.x, m_position.y, m_position.z);
	}

	void DynamicObject::SetScale(bssFloat x, bssFloat y, bssFloat z)
	{
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;

		m_WorldMatrix.LoadIdentity();
		Scale(m_WorldMatrix, m_scale.x, m_scale.y, m_scale.z);
		RotatePitch(m_WorldMatrix, m_pitch);
		RotateYaw(m_WorldMatrix, m_heading);
		RotateRoll(m_WorldMatrix, m_roll);
		SetPos(m_WorldMatrix, m_position.x, m_position.y, m_position.z);

	}


	void DynamicObject::SetRoll(bssFloat roll)
	{
		m_roll = roll;
		m_roll = fmod(m_roll, 360);

		m_WorldMatrix.LoadIdentity();
		Scale(m_WorldMatrix, m_scale.x, m_scale.y, m_scale.z);
		RotateYaw(m_WorldMatrix, m_heading);
		RotatePitch(m_WorldMatrix, m_pitch);
		RotateRoll(m_WorldMatrix, m_roll);
		SetPos(m_WorldMatrix, m_position.x, m_position.y, m_position.z);

	}

	void DynamicObject::SetPitch(bssFloat pitch)
	{
		m_pitch = pitch;
		m_pitch = fmod(m_pitch, 360);

		m_WorldMatrix.LoadIdentity();
		Scale(m_WorldMatrix, m_scale.x, m_scale.y, m_scale.z);
		RotateYaw(m_WorldMatrix, m_heading);
		RotatePitch(m_WorldMatrix, m_pitch);
		RotateRoll(m_WorldMatrix, m_roll);
		SetPos(m_WorldMatrix, m_position.x, m_position.y, m_position.z);

	}
	void DynamicObject::SetHeading(bssFloat heading)
	{
		m_heading = heading;
		m_heading = fmod(m_heading, 360);

		m_WorldMatrix.LoadIdentity();
		Scale(m_WorldMatrix, m_scale.x, m_scale.y, m_scale.z);
		RotateYaw(m_WorldMatrix, m_heading);
		RotatePitch(m_WorldMatrix, m_pitch);
		RotateRoll(m_WorldMatrix, m_roll);
		SetPos(m_WorldMatrix, m_position.x, m_position.y, m_position.z);
	}

	void DynamicObject::Render(GLSLProgram *shader, bssBool cullFront) const
	{
		//Send the world_matrix  to the shaders
		shader->sendUniform4x4("worldMatrix", m_WorldMatrix.data);


		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

		glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3fPos4fColor2fTex), 0);
		glVertexAttribPointer((GLint)2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(bssFloat) * 3));
		glVertexAttribPointer((GLint)3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(bssFloat) * 7));

		glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
		glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);


		if (m_loadedObj)
		{
			if (cullFront)
			{
				glFrontFace(GL_CCW);
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
				glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
				glFrontFace(GL_CW);
			}
			else
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
				glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
			}
		}
		else
		{
			if (cullFront)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
				glDrawElements(GL_TRIANGLE_STRIP, m_index.size(), GL_UNSIGNED_INT, 0);
			}
			else
			{
				glFrontFace(GL_CCW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
				glDrawElements(GL_TRIANGLE_STRIP, m_index.size(), GL_UNSIGNED_INT, 0);
				glFrontFace(GL_CW);
			}
		}

	}


	void DynamicObject::SetShapeColor(Vector3 color)
	{
		for (bssU32 i = 0; i < m_verticesCount; i++)
		{
			m_vertices[i].r = color.x;
			m_vertices[i].g = color.y;
			m_vertices[i].b = color.z;
		}
		CleanOpenglBuffers();
		GenerateOpenglBuffers();
	}

	void Vertex3fPos4fColor2fTex::Set(bssFloat x, bssFloat y, bssFloat z, bssFloat r, bssFloat g, bssFloat b, bssFloat a, bssFloat s, bssFloat t)
	{
		this->x = x;
		this->y = y;
		this->z = z;

		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;

		this->s = s;
		this->t = t;
	}

	void DynamicObject::CleanBuffers()
	{
		if (m_vertices != NULL)
		{
			delete[] m_vertices;
			m_vertices = NULL;

			if (m_normals != NULL)
			{
				delete[] m_normals;
				m_normals = NULL;
			}
		}

		m_verticesCount = m_normalsCount = 0;
		m_index.clear();
		CleanOpenglBuffers();
	}

	void DynamicObject::CleanOpenglBuffers()
	{
		if (m_vertexBuffer)
			glDeleteBuffers(1, &m_vertexBuffer);
		if (m_normalBuffer)
			glDeleteBuffers(1, &m_normalBuffer);
		if (m_indexBuffer)
			glDeleteBuffers(1, &m_indexBuffer);
	}


	void DynamicObject::GenerateOpenglBuffers()
	{
		glGenBuffers(1, &m_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3fPos4fColor2fTex) * m_verticesCount, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

		if (m_normalsCount)
		{
			glGenBuffers(1, &m_normalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer); //Bind the normal buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_normalsCount, &m_normals[0], GL_STATIC_DRAW); //Send the data to OpenGL
		}
		if (m_index.size())
		{
			glGenBuffers(1, &m_indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer); //Bind the index buffer
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bssU32) * m_index.size(), &m_index[0], GL_STATIC_DRAW); //Send the data to OpenGL
		}
	}

	bssBool DynamicObject::loadOBJ(const char * path)
	{

		CleanBuffers();

		std::vector< bssU32 > vertexIndices, uvIndices, normalIndices;
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
				bssU32 vertexIndex[3], uvIndex[3], normalIndex[3];
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
			m_normalsCount = m_verticesCount = vertexIndices.size();
			m_vertices = new andro::Vertex3fPos4fColor2fTex[m_verticesCount];
			m_normals = new Vector3[m_normalsCount];

		}

		fclose(file);

		// For each vertex of each triangle
		for (bssU32 i = 0; i<vertexIndices.size(); i++)
		{
			bssU32 vertexIndex = vertexIndices[i];
			Vector3 vertex = temp_vertices[vertexIndex - 1];

			m_vertices[i].x = vertex.x;
			m_vertices[i].y = vertex.y;
			m_vertices[i].z = vertex.z;

			m_vertices[i].r = 0;
			m_vertices[i].g = 0;
			m_vertices[i].b = 0;


		}
		for (bssU32 i = 0; i < uvIndices.size(); i++)
		{
			bssU32 uvIndex = uvIndices[i];
			Vector2 tex = temp_uvs[uvIndex - 1];

			m_vertices[i].s = tex.x;
			m_vertices[i].t = tex.y;

		}
		for (bssU32 i = 0; i<normalIndices.size(); i++)
		{
			bssU32 vertexIndex = normalIndices[i];
			m_normals[i] = temp_normals[vertexIndex - 1];
		}


		GenerateOpenglBuffers();
		m_loadedObj = true;

		return true;
	}


	void DynamicObject::SetPitchSpeed(bssFloat speed)
	{
		m_pitchSpeed=speed;
	}
	void DynamicObject::SetRollSpeed(bssFloat speed)
	{
		m_rollSpeed=speed;
	}
	void DynamicObject::SetHeadingSpeed(bssFloat speed)
	{
		m_headingSpeed=speed;
	}


	void  DynamicObject::SetMoveSpeed(const Vector3& speed)
	{
		m_moveSpeed.x=speed.x;
		m_moveSpeed.y=speed.y;
		m_moveSpeed.z=speed.z;
	}

	void DynamicObject::UpdateAngle(bssFloat& angle, bssFloat speed)
	{
	  angle += speed;
	  angle = fmod(angle, 360);
	}

	void DynamicObject::Update(bssFloat dt)
	{
		//update angles
		UpdateAngle(m_roll,m_rollSpeed*dt);
		UpdateAngle(m_pitch,m_pitchSpeed*dt);
		UpdateAngle(m_heading,m_headingSpeed*dt);
	   

		//update position
		m_position.x+=m_moveSpeed.x*dt;
		m_position.y+=m_moveSpeed.y*dt;
		m_position.z+=m_moveSpeed.z*dt;

		Scale(m_WorldMatrix, m_scale.x, m_scale.y,m_scale.z );
		RotatePitch(m_WorldMatrix,m_rollSpeed*dt);
		RotateYaw(m_WorldMatrix,m_headingSpeed*dt);
		RotateRoll(m_WorldMatrix,m_pitchSpeed*dt);

		
		SetPos(m_WorldMatrix,m_position.x,m_position.y,m_position.z);
	}

	bssBool DynamicObject::LoadTexture(char* filename)
	{


		/* load an image file directly as a new OpenGL texture */
		m_imageID = SOIL_load_OGL_texture
			(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
			);

		/* check for an error during the load process */
		if (0 == m_imageID)
		{
			TRACE(L"SOIL loading error: '%S'\n", SOIL_last_result());
		}
		return 1;
	}
	void DynamicObject::SetTexture() const
	{
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, m_imageID);
	}



	//---------------------------------------------------------------------------------------------
	Line::Line() : DynamicObject()
	{
		m_size = 1;
		CreateLine(Vector3(1,1,1),1);
  
	}
	Line::Line(Vector3 position, Vector3 orientation,bssFloat width)
		:DynamicObject(position, width)
	{
		m_size =  width;
		SetPosition(position);
		m_orientation=orientation;
		CreateLine(orientation,width);
	}

	Line::~Line(){}
	
	void Line::UpdateLine(Vector3 position, Vector3 orientation)
	{

		m_orientation = orientation;
		m_position	  = position;

		Vector3 target =   m_orientation - m_position;
		bssFloat width =  m_size;

		m_vertices[0].Set( 0				,0			, 0			,0,0,0,0,0,1);
		m_vertices[1].Set( target.x			, target.y	, target.z	,0,0,0,1,1,0);
		m_vertices[2].Set( target.x+width	, target.y	, target.z	,0,0,0,1,1,0);
		m_vertices[3].Set(  width			, 0			, 0			,0,0,0,0,0,1);
		
		m_vertices[4].Set( 0				, width				, 0			,0,0,0,0,0,1);
		m_vertices[5].Set( target.x			, target.y + width	, target.z	,0,0,0,1,1,0);
		m_vertices[6].Set( target.x+width	, target.y + width	, target.z	,0,0,0,1,1,0);
		m_vertices[7].Set(  width			, width				, 0			,0,0,0,0,0,1);
	
		glGenBuffers(1,&m_vertexBuffer); 
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3fPos4fColor2fTex) * m_verticesCount, &m_vertices[0], GL_DYNAMIC_DRAW); //Send the data to OpenGL
	  
	
	}

	void Line::CreateLine(Vector3 orientation,bssFloat width)
	{
	Vector3 target =   m_orientation - m_position;

		CleanBuffers();
		m_size =  width;



		m_vertices=new Vertex3fPos4fColor2fTex[8];
		m_verticesCount=8;

		m_vertices[0].Set( 0				,0			, 0			,0,0,0,0,0,1);
		m_vertices[1].Set( target.x			, target.y	, target.z	,0,0,0,1,1,0);
		m_vertices[2].Set( target.x+width	, target.y	, target.z	,0,0,0,1,1,0);
		m_vertices[3].Set(  width			, 0			, 0			,0,0,0,0,0,1);
		
		m_vertices[4].Set( 0				, width				, 0			,0,0,0,0,0,1);
		m_vertices[5].Set( target.x			, target.y + width	, target.z	,0,0,0,1,1,0);
		m_vertices[6].Set( target.x+width	, target.y + width	, target.z	,0,0,0,1,1,0);
		m_vertices[7].Set(  width			, width				, 0			,0,0,0,0,0,1);
		

	   m_index.push_back(0);
	   m_index.push_back(1);
	   m_index.push_back(3);
	   m_index.push_back(2);
	   m_index.push_back(7);
	   m_index.push_back(6);
	   m_index.push_back(4);
	   m_index.push_back(5);
	   m_index.push_back(0);
	   m_index.push_back(1);
	   m_index.push_back(1);
	   m_index.push_back(2);
	   m_index.push_back(5);
	   m_index.push_back(6);
	   m_index.push_back(6);
	   m_index.push_back(7);
	   m_index.push_back(7);
	   m_index.push_back(4);
	   m_index.push_back(3);
	   m_index.push_back(0);


		GenerateOpenglBuffers();


	}
	void Line::Render(GLSLProgram* shader, bssBool cullFront) const
	{
		//Send the world_matrix  to the shaders
		shader->sendUniform4x4("worldMatrix", m_WorldMatrix.data);

		
		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);

		glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3fPos4fColor2fTex), 0);
		glVertexAttribPointer((GLint)2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(bssFloat) * 3));
		glVertexAttribPointer((GLint)3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(bssFloat) * 7));
		


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBuffer);	

		glDrawElements(GL_TRIANGLE_STRIP, m_index.size(), GL_UNSIGNED_INT, 0);

	}
	

	
	//----------------------------------------------------------------------------------------------


	Cube::Cube() : DynamicObject()
	{
		CreateCube(Vector3(1,1,1),Vector3());
		m_imageID=0;
		m_size = 1;
		m_color = Vector3();
  
	}
	Cube::Cube(Vector3 position,Vector3 size,Vector3 color)  : DynamicObject(position, size.Lenght())
	{

		m_imageID=0;
		m_size =  size.Lenght();
		SetPosition(position);
		m_color = color;
		CreateCube(size,color);
		
	}
	Cube::~Cube()  
	{
		m_imageID=0;
  
	}

	void Cube::CreateCube(Vector3 size,Vector3 color)
	{
		CleanBuffers();

		m_size =  size.Lenght();

		m_vertices=new Vertex3fPos4fColor2fTex[24];
		m_verticesCount=24;
		bssFloat r,g,b;
		m_color = color;
		r =  m_color.x;
		g =  m_color.y;
		b =  m_color.z;

		//front
/*1*/		m_vertices[0].Set(-size.x, size.y, size.z,r,g,b,0, 0, 0);
/*2*/		m_vertices[1].Set( size.x, size.y, size.z,r,g,b,0, 0, 1);
/*3*/		m_vertices[2].Set(-size.x,-size.y, size.z,r,g,b,0, 1, 0);
/*4*/		m_vertices[3].Set( size.x,-size.y, size.z,r,g,b,0, 1, 1);

		//back
/*5*/		m_vertices[4].Set(-size.x, size.y,-size.z,r,g,b,0, 0, 0);
/*6*/		m_vertices[5].Set( size.x, size.y,-size.z,r,g,b,0, 0, 1);
/*7*/		m_vertices[6].Set(-size.x,-size.y,-size.z,r,g,b,0, 1, 0);
/*8*/		m_vertices[7].Set( size.x,-size.y,-size.z,r,g,b,0, 1, 1);

		//left
/*1*/		m_vertices[8].Set( -size.x, size.y, size.z,r,g,b,0, 0, 0);
/*5*/		m_vertices[9].Set( -size.x, size.y,-size.z,r,g,b,0, 1, 0);
/*3*/		m_vertices[10].Set(-size.x,-size.y, size.z,r,g,b,0, 0, 1);
/*7*/		m_vertices[11].Set(-size.x,-size.y,-size.z,r,g,b,0, 1, 1);

		//right
/*2*/		m_vertices[12].Set( size.x, size.y, size.z,r,g,b,0, 0, 0);
/*6*/		m_vertices[13].Set( size.x, size.y,-size.z,r,g,b,0, 0, 1);
/*8*/		m_vertices[14].Set( size.x,-size.y,-size.z,r,g,b,0, 1, 1);
/*4*/		m_vertices[15].Set( size.x,-size.y, size.z,r,g,b,0, 1, 0);

		//up
/*1*/		m_vertices[16].Set(-size.x, size.y, size.z,r,g,b,0, 0, 0);
/*2*/		m_vertices[17].Set( size.x, size.y, size.z,r,g,b,0, 0, 1);
/*6*/		m_vertices[18].Set( size.x, size.y,-size.z,r,g,b,0, 1, 1);
/*5*/		m_vertices[19].Set(-size.x, size.y,-size.z,r,g,b,0, 1, 0);
		//bottom
/*3*/		m_vertices[20].Set(-size.x,-size.y, size.z,r,g,b,0, 0, 0);
/*4*/		m_vertices[21].Set( size.x,-size.y, size.z,r,g,b,0, 0, 1);
/*8*/		m_vertices[22].Set( size.x,-size.y,-size.z,r,g,b,0, 1, 1);
/*7*/		m_vertices[23].Set(-size.x,-size.y,-size.z,r,g,b,0, 1, 0);

	m_normalsCount = 22;
	m_normals = new Vector3[m_normalsCount];
	//front
	m_index.push_back(0);
	m_index.push_back(1);
	m_index.push_back(2);
	m_index.push_back(3);
	m_normals[0] = Vector3(0,0,1);
	m_normals[1] = Vector3(0,0,1);
	m_normals[2] = Vector3(0,0,0);
	m_normals[3] = Vector3(0,0,0);

	//back
	m_index.push_back(5);
	m_index.push_back(4);
	m_index.push_back(7);
	m_index.push_back(6);
	m_normals[4] = Vector3(0,0,-1);
	m_normals[5] = Vector3(0,0,-1);
	m_normals[6] = Vector3(0,0,0);
	m_normals[7] = Vector3(0,0,0);

	   
	//left
	m_index.push_back(9);
	m_index.push_back(8);
	m_index.push_back(11);
	m_index.push_back(10);
	m_normals[8] = Vector3(-1,0,0);
	m_normals[9] = Vector3(-1,0,0);
	m_normals[10] = Vector3(0,0,0);
	m_normals[11] = Vector3(0,0,0);


	//right
	m_index.push_back(12);
	m_index.push_back(13);
	m_index.push_back(15);
	m_index.push_back(14);
	m_normals[12] = Vector3(1,0,0);
	m_normals[13] = Vector3(1,0,0);
	m_normals[14] = Vector3(0,0,0);
	m_normals[15] = Vector3(0,0,0);

	//up
	m_index.push_back(19);
	m_index.push_back(18);
	m_index.push_back(16);
	m_index.push_back(17);
	m_normals[16] = Vector3(0,1,0);
	m_normals[17] = Vector3(0,1,0);
	m_normals[18] = Vector3(0,0,0);
	m_normals[19] = Vector3(0,0,0);

	//bottom
	m_index.push_back(20);
	m_index.push_back(21);
	m_index.push_back(23);
	m_index.push_back(22);
	m_normals[20] = Vector3(0,-1,0);
	m_normals[21] = Vector3(0,-1,0);


	GenerateOpenglBuffers();

	}


//--------------------------------------------------------------------------------------------------------------------
	Quad::Quad() : DynamicObject()
	{
		CreateQuad(Vector2(1,1));
		m_imageID=0;
		m_size = 1;
		m_color = Vector3();
 
	}
	Quad::Quad(Vector3 position,Vector2  size, Vector3 color)  : DynamicObject(position, size.Lenght())
	{
		SetPosition(position);
		m_imageID=0;
		m_size =size.Lenght();
		m_color = color;
  
		CreateQuad(size);
		
	}

	Quad::~Quad()  
	{
		m_imageID=0;
  
	}


	void Quad::CreateQuad(Vector2 size)
	{
		CleanBuffers();

		m_size =  size.Lenght();

		m_vertices=new Vertex3fPos4fColor2fTex[8];
		m_verticesCount=8;

		bssFloat r,g,b;
		r =  m_color.x;
		g =  m_color.y;
		b =  m_color.z;
		bssFloat a = 0.0f, d = 1.0f;

		m_vertices[0].Set(-size.x, 0,-size.y,r,g,b,0,a,a);
		m_vertices[1].Set( size.x, 0,-size.y,r,g,b,0,d,a);
		m_vertices[2].Set( size.x, 0, size.y,r,g,b,0,d,d);
		m_vertices[3].Set(-size.x, 0, size.y,r,g,b,0,a,d);

		m_vertices[4].Set(-size.x, -0.5,-size.y,r,g,b,0,a,a);
		m_vertices[5].Set( size.x, -0.5,-size.y,r,g,b,0,d,a);
		m_vertices[6].Set( size.x, -0.5, size.y,r,g,b,0,d,d);
		m_vertices[7].Set(-size.x, -0.5, size.y,r,g,b,0,a,d);


		
		m_index.clear();

	   m_index.push_back(0);
	   m_index.push_back(1);
	   m_index.push_back(3);
	   m_index.push_back(2);
	   //backface
	   m_index.push_back(7);
	   m_index.push_back(6);
	   m_index.push_back(4);
	   m_index.push_back(5);


	   m_normalsCount =2;
		m_normals = new Vector3[m_normalsCount];
		m_normals[0] = Vector3(0,1,0);
		m_normals[1] = Vector3(0,1,0);


		GenerateOpenglBuffers();


		//set world state object
		m_WorldMatrix.LoadIdentity();
		Translate(m_WorldMatrix,m_position.x,m_position.y,m_position.z);
		RotatePitch(m_WorldMatrix,m_pitch);
		RotateYaw(m_WorldMatrix,m_heading);
		RotateRoll(m_WorldMatrix,m_roll);
	}

}