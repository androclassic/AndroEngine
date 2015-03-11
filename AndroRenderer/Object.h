#pragma once

#include<vector>
#include<ctime>

#include"ShaderTypes.h"
#include "MMath.h"
#include "Vertex.h"
using namespace std;

namespace andro
{
	class Object 
	{
		public:
		
    		virtual void Render(GLSLProgram *shader,bool cullFront) const;
			virtual	void SetTexture() const =0;

			void SetPosition(Vector3 position);
			void SetRoll(float roll);
			void SetPitch(float pitch);
			void SetScale(float x, float y, float z);
			void SetShapeColor(Vector3 color);
			void SetHeading(float heading);
			void SetMatrixData(float data[]);

			Vector3 GetPosition() const { return m_position;}
			void GetMatrixData(float data[]) const;

			Vector3 Look() const;
			Vector3 Right() const;
			Vector3 Up() const;

			bool loadOBJ(const char * path );

	
			 virtual ~Object();	
			Matrix4				            m_WorldMatrix;

		protected:
			Object();
			Object(Vector3 pos,float size);
			void CleanBuffers();
			void CleanOpenglBuffers();
			void GenerateOpenglBuffers();
			
		protected:

			
			Vertex3fPos4fColor2fTex*		m_vertices;
			Vector3*						m_normals;

			GLuint							m_verticesCount;
			GLuint							m_normalsCount;
			
			vector<unsigned int>			m_index;
	    
			GLuint                  m_vertexBuffer;
			GLuint                  m_indexBuffer;
			GLuint                  m_normalBuffer;


			float					m_roll;
			float					m_pitch;
			float					m_heading;
	        
			

			Vector3					m_position;
			Vector3					m_lastPosition;
			Vector3					m_scale;
			float					m_size;   
			bool					m_loadedObj;
	};
}


