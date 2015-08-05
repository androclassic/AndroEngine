#pragma once

#include<vector>
#include<ctime>

#include"ShaderTypes.h"
#include "MMath.h"
#include "Vertex.h"
#include "../AndroUtils/AndroUtils.h"
using namespace std;

namespace andro
{

	class DynamicObject 
	{
		public:
			DynamicObject();
			DynamicObject(Vector3 position,bssFloat size);
			virtual ~DynamicObject();
			
			virtual void Render(GLSLProgram *shader, bssBool cullFront) const;
			virtual void Update(bssFloat dt);

			void SetPosition(Vector3 position);
			void SetRoll(bssFloat roll);
			void SetPitch(bssFloat pitch);
			void SetScale(bssFloat x, bssFloat y, bssFloat z);
			void SetShapeColor(Vector3 color);
			void SetHeading(bssFloat heading);

			Vector3 GetPosition() const { return m_position; }

			bssBool loadOBJ(const char * path);


			void SetRollSpeed(bssFloat speed);
			void SetPitchSpeed(bssFloat speed);
			void SetHeadingSpeed(bssFloat speed);
			void SetMoveSpeed(const Vector3& speed);
			bssBool LoadTexture(char* filename);
			void SetTexture() const;

	
			Matrix4	m_WorldMatrix;
		protected:
			void CleanBuffers();
			void CleanOpenglBuffers();
			void GenerateOpenglBuffers();
			void UpdateAngle(bssFloat& angle, bssFloat speed);


			Vertex3fPos4fColor2fTex*	m_vertices;
			Vector3*					m_normals;
			bssU32						m_verticesCount;
			bssU32						m_normalsCount;

			vector<bssU32>		m_index;

			bssU32						m_vertexBuffer;
			bssU32						m_indexBuffer;
			bssU32						m_normalBuffer;

			bssFloat						m_roll;
			bssFloat						m_pitch;
			bssFloat						m_heading;

			Vector3						m_position;
			Vector3						m_lastPosition;
			Vector3						m_scale;
			bssFloat						m_size;
			bssBool						m_loadedObj;


			bssFloat						m_rollSpeed;
			bssFloat						m_pitchSpeed;
			bssFloat						m_headingSpeed;
			Vector3						m_moveSpeed;
			bssU32						m_imageID;	

	};


	
	class Line : public DynamicObject 
	{
		public:
			Line();
			Line(Vector3 position, Vector3 orientation,bssFloat width);
			void Render(GLSLProgram *shader, bssBool cullFront) const;
			void UpdateLine(Vector3 position, Vector3 orientation);
			virtual ~Line();
			
			void CreateLine(Vector3 orientation,bssFloat width);
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

