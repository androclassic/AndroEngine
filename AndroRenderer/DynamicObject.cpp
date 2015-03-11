#include"DynamicObject.h"
#include <iostream>
#include"ShaderTypes.h"

#include "../AndroUtils/Trace.h"
#include "SOIL.h"

namespace andro
{


	DynamicObject::DynamicObject():Object()
	{
		m_rollSpeed=0;
		m_pitchSpeed=0;
		m_headingSpeed=0;

	}

	DynamicObject::DynamicObject(Vector3 position,float size)
	:Object(position,size)
	{
	 m_rollSpeed=0;
	 m_pitchSpeed=0;
	 m_headingSpeed=0;
	 m_moveSpeed = Vector3();

	}

	void DynamicObject::SetPitchSpeed(float speed)
	{
		m_pitchSpeed=speed;
	}
	void DynamicObject::SetRollSpeed(float speed)
	{
		m_rollSpeed=speed;
	}
	void DynamicObject::SetHeadingSpeed(float speed)
	{
		m_headingSpeed=speed;
	}


	void  DynamicObject::SetMoveSpeed(const Vector3& speed)
	{
		m_moveSpeed.x=speed.x;
		m_moveSpeed.y=speed.y;
		m_moveSpeed.z=speed.z;
	}

	void DynamicObject::UpdateAngle(float& angle, float speed)
	{
	  angle += speed;
	  angle = fmod(angle, 360);
	}

	void DynamicObject::Update(float dt)
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

	bool DynamicObject::LoadTexture(char* filename)
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
	Line::Line(Vector3 position, Vector3 orientation,float width)
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
		float width =  m_size;

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

	void Line::CreateLine(Vector3 orientation,float width)
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
	void Line::Render(GLSLProgram* shader, bool cullFront) const
	{

		float worldMatrix[16];

		m_WorldMatrix.LoadVectorFromMatrix( worldMatrix);
	 
		//Send the world_matrix  to the shaders
		shader->sendUniform4x4("worldMatrix", worldMatrix);

		
		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBuffer);

 		glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE,sizeof(Vertex3fPos4fColor2fTex), 0);
 		glVertexAttribPointer((GLint)2, 4, GL_FLOAT, GL_FALSE,sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(float)*3));
		glVertexAttribPointer((GLint)3, 2, GL_FLOAT, GL_FALSE,sizeof(Vertex3fPos4fColor2fTex), (void*)(sizeof(float)*7));
		


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
		float r,g,b;
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

		float r,g,b;
		r =  m_color.x;
		g =  m_color.y;
		b =  m_color.z;
		float a = 0.0f, d = 1.0f;

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