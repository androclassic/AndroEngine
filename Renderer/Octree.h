#pragma once
#include <vector>
#include "../AndroUtils/Utils/MMath.h"


namespace TakeTwo
{
	class RenderObject;
}

namespace andro
{

	class OctreeObject
	{
		TakeTwo::RenderObject* handle;
	};
	


	enum NodePosition
	{
		NEAR_BOTTOM_LEFT = 4,
		NEAR_BOTTOM_RIGHT = 0,
		NEAR_TOP_LEFT = 6,
		NEAR_TOP_RIGHT = 2,

		FAR_BOTTOM_LEFT = 5,
		FAR_BOTTOM_RIGHT = 1,
		FAR_TOP_RIGHT = 3,
		FAR_TOP_LEFT = 7,

		MAX_NODE_COUNT = 8 //any defined node should be < 8 && > 0
	};

	class BoundingBox
	{
	public:
		void SplitToEight(BoundingBox boxes[8]);
		inline void SetExtents(Vector3& pMin, Vector3&  pMax) { min = pMin; max = pMax; }
		Vector3 GetHalfSize() const;
		Vector3 GetCenter() const;

		Vector3		min;
		Vector3		max;
	};

	struct OctreeNode
	{
		OctreeNode()
		{
			for (int i = 0; i < 8; i++)
				m_children[i] = NULL;
		}
		float			m_halfWidth;
		Vector3         m_center;
		BoundingBox		m_bounds;
		std::vector<andro::Triangle>  m_objects; 
		OctreeNode*					m_children[8];
	};
	
	OctreeNode* BuildOctreeFromMesh(TakeTwo::RenderObject* object, std::vector<andro::Triangle>& objects, BoundingBox& aabb, int stopDepth);

}

