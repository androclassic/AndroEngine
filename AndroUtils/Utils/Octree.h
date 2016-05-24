#pragma once
#include <vector>
#include <functional>
#include "Shapes.h"
#include "Ray.h"

namespace andro
{

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

		~OctreeNode()
		{
			for (int i = 0; i < 8; i++)
			{
				if (m_children[i] != NULL)
					delete m_children[i];
			}
		}


		BoundingBox		m_bounds;
		std::vector<T>  m_objects;
		OctreeNode<T>*					m_children[8];
	};
	
	// set enclose true when you want to keep objects in the level that enclose the entire obj(pass the correct test func)
	template<typename T>
	OctreeNode<T>* BuildOctree(std::vector<T>& objects, BoundingBox& aabb, int stopDepth, std::function<bool(const BoundingBox& aabb, T object)> functionTest, bool enclose = false)
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

		if (stopDepth == 0)
		{
			node->m_objects = objects; // Warning: copying here best be avoided.
			return node;
		}

		std::vector<T> boxObjects[8];

		bool overall_result = false;

		// for each object text against the octree node
		for (int i = 0; i < objects.size(); ++i)
		{
			overall_result = false;
			for (int boxId = 0; boxId<8; ++boxId)
			{
				bool result = functionTest(boxes[boxId], objects[i]);

				if (result)
				{
					boxObjects[boxId].push_back(objects[i]); // Potential memory allocation here. Could be optimized					
				}
				overall_result = result || overall_result;
			}

			// add to parrent if is not enclosed in any childrens
			if (enclose && !overall_result)
				node->m_objects.push_back(objects[i]);
		}

		//Loop through boxes and divide them more.
		for (int i = 0; i < 8; ++i)
		{
			size_t size = boxObjects[i].size();
			if (size)
			{
				node->m_children[i] = BuildOctree(boxObjects[i], boxes[i], stopDepth - 1, functionTest, enclose);
			}
		}

		return node;

	}



}

