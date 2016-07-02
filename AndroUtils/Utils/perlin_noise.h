#pragma once
#include "AndroTypes.h"

namespace andro
{

	class Vector3;

	class perlin_noise
	{
	public:
		 perlin_noise(int size = 4);
		 afloat get_value(const Vector3& p) const;

	private:
		void perlin_generate();
		void permute(int* p, int n);
		void perlin_generate_perm( int* perm );

		 afloat ranFloat[256];
		 int perm_x[256];
		 int perm_y[256];
		 int perm_z[256];
		 int m_size;
	};


}