#include "perlin_noise.h"
#include "MMath.h"

namespace andro
{
	perlin_noise::perlin_noise(int size)
	{
		m_size = size;
		perlin_generate();
		perlin_generate_perm(perm_x);
		perlin_generate_perm(perm_y);
		perlin_generate_perm(perm_z);

	}
	afloat perlin_noise::get_value(const Vector3& p) const
	{
		//afloat u = p.x - floorf(p.x);
		//afloat v = p.y - floorf(p.y);
		//afloat w = p.z - floorf(p.z);

		int i = int(m_size * p.x) & 255;
		int j = int(m_size * p.y) & 255;
		int k = int(m_size * p.z) & 255;

		return ranFloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
	}


	void perlin_noise::perlin_generate()
	{
		for (int i = 0; i < 256; i++)
		{
			ranFloat[i] = random_float();
		}
	}

	void perlin_noise::permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int target = int(random_float() * (i + 1));
			int temp = p[i];
			p[i] = p[target];
			p[target] = temp;
		}
	}


	void perlin_noise::perlin_generate_perm(int* perm)
	{
		for (int i = 0; i < 256; i++)
		{
			perm[i] = i;
		}

		permute(perm, 256);
	}

}