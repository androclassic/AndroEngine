#pragma once

#include "Object.h"



namespace andro
{

	class DynamicObject :public Object
	{
		public:
			DynamicObject();
			DynamicObject(Vector3 position,float size);
		    virtual void Update(float dt); 

			void SetRollSpeed(float speed);
			void SetPitchSpeed(float speed);
			void SetHeadingSpeed(float speed);
			void SetMoveSpeed(const Vector3& speed);
			bool LoadTexture(char* filename);
			void SetTexture() const;

			
		protected:
			void UpdateAngle(float& angle, float speed);

			float				m_rollSpeed;
			float				m_pitchSpeed;
			float				m_headingSpeed;
			Vector3				m_moveSpeed;
			GLuint				m_imageID;	

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

