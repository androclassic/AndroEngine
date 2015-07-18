#pragma once

namespace andro
{
	class Vertex3fPos4fColor2fTex
	{
		public:

			void Set(bssFloat x,bssFloat y,bssFloat z,bssFloat r=0,
					 bssFloat g=0,bssFloat b=0,bssFloat a=0,bssFloat s=0,bssFloat t=0);
		
			bssFloat x;
			bssFloat y;
			bssFloat z;

			bssFloat r;
			bssFloat g;
			bssFloat b;
			bssFloat a;

			bssFloat s;
			bssFloat t;

	  

	};
}


