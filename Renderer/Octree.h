#pragma once
#include <vector>
#include <functional>
#include "../AndroUtils/Utils/MMath.h"
#include "OGL_Platform.h"


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


	void SplitToEight(const BoundingBox& box, BoundingBox boxes[8]);

	template<typename T>
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

		std::vector<T>  m_objects;
		OctreeNode<T>*					m_children[8];
	};
	

	template<typename T>
	OctreeNode<T>* BuildOctree(std::vector<T>& objects, BoundingBox& aabb, int stopDepth, std::function<bool(const BoundingBox& aabb, T object)> functionTest)
	{
		// stop if we reached certain depth or certain number of objects
		if (stopDepth < 0 )
			return NULL;

		// Split to 8 cubes
		BoundingBox boxes[8];
		SplitToEight(aabb, boxes);

		// Create a node and attach the objects
		OctreeNode<T>* node = new OctreeNode<T>;
		node->m_bounds = aabb;

		if (stopDepth ==  0)
			node->m_objects = objects; // Warning: copying here best be avoided.

		std::vector<T> boxObjects[8];


		// for each object text against the octree node
		for (int i = 0; i < objects.size(); ++i)
		{
			for (int boxId = 0; boxId<8; ++boxId)
			{
				bool overlaps = functionTest(boxes[boxId], objects[i]);

				if (overlaps)
				{
					boxObjects[boxId].push_back(objects[i]); // Potential memory allocation here. Could be optimized					
				}
			}
		}

		//Loop through boxes and divide them more.
		for (int i = 0; i < 8; ++i)
		{
			size_t size = boxObjects[i].size();
			if (size)
			{
				node->m_children[i] = BuildOctree(boxObjects[i], boxes[i], stopDepth - 1, functionTest);
			}
		}

		return node;

	}



}

