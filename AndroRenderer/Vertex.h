#pragma once

namespace andro
{
	class Vertex3fPos4fColor2fTex
	{
		public:

			void Set(float x,float y,float z,float r=0,
					 float g=0,float b=0,float a=0,float s=0,float t=0);
		
			float x;
			float y;
			float z;

			float r;
			float g;
			float b;
			float a;

			float s;
			float t;

	  

	};
}


