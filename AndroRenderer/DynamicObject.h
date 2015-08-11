#pragma once

#include<vector>
#include<ctime>

#include"ShaderTypes.h"
#include "Vertex.h"
#include "../AndroUtils/AndroUtils.h"
#include "../AndroUtils/MMath.h"

using namespace std;

namespace andro
{

	class DynamicObject 
	{
		public:
			DynamicObject();
			DynamicObject(Vector3 position,float size);
			virtual ~DynamicObject();
			
			virtual void Render(GLSLProgram *shader, bool cullFront) const;
			virtual void Update(float dt);

			void SetPosition(Vector3 position);
			void SetRoll(float roll);
			void SetPitch(float pitch);
			void SetScale(float x, float y, float z);
			void SetShapeColor(Vector3 color);
			void SetHeading(float heading);

			Vector3 GetPosition() const { return m_position; }

			bool loadOBJ(const char* filename);


			void SetRollSpeed(float speed);
			void SetPitchSpeed(float speed);
			void SetHeadingSpeed(float speed);
			void SetMoveSpeed(const Vector3& speed);
			bool LoadTexture(char* filename);
			void SetTexture() const;

	
			Matrix4	m_WorldMatrix;
		protected:
			void CleanBuffers();
			void CleanOpenglBuffers();
			void GenerateOpenglBuffers();
			void UpdateAngle(float& angle, float speed);


			Vertex3fPos4fColor2fTex*	m_vertices;
			Vector3*					m_normals;
			unsigned int						m_verticesCount;
			unsigned int						m_normalsCount;

			vector<unsigned int>		m_index;

			unsigned int						m_vertexBuffer;
			unsigned int						m_indexBuffer;
			unsigned int						m_normalBuffer;

			float						m_roll;
			float						m_pitch;
			float						m_heading;

			Vector3						m_position;
			Vector3						m_lastPosition;
			Vector3						m_scale;
			float						m_size;
			bool						m_loadedObj;


			float						m_rollSpeed;
			float						m_pitchSpeed;
			float						m_headingSpeed;
			Vector3						m_moveSpeed;
			unsigned int						m_imageID;	

			const void*					m_buffer;

	};


	
	class Line : public DynamicObject 
	{
		public:
			Line();
			Line(Vector3 position, Vector3 orientation,float width);
			void Render(GLSLProgram *shader, bool cullFront) const;
			void UpdateLine(Vector3 position, Vector3 orientation);
			virtual ~Line();
			
			void CreateLine(Vector3 orientation,float width);
			void SetOrientation(Vector3 orientation) {m_orientation =  orientation;}
		
		protected:
			Vector3				    m_orientation;

	};

	
	class Cube : public DynamicObject 
	{
		public:
			Cube();
			Cube(Vector3 position,Vector3 size,Vector3 color);
			virtual ~Cube();
		

			void CreateCube(Vector3 size,Vector3 color); // fill vertices info
		protected:
			Vector3					m_color;

	};

	class Quad : public DynamicObject
	{
		public:
			Quad();
			Quad(Vector3 position,Vector2 size,Vector3 color);
			virtual ~Quad();

			void CreateQuad(Vector2 size);
		protected:
			Vector3					m_color;
			
	};


}

