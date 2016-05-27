#pragma once
#include <vector>
#include <functional>
#include "Shapes.h"
#include "Ray.h"
#include "Trace.h"

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
			m_terminal = FALSE;
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
		bool			m_terminal;
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
			node->m_terminal = true;
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
		bool is_terminal = true;
		for (int i = 0; i < 8; ++i)
		{
			size_t size = boxObjects[i].size();
			if (size)
			{
				is_terminal = false;
				node->m_children[i] = BuildOctree(boxObjects[i], boxes[i], stopDepth - 1, functionTest, enclose);
			}
		}
		node->m_terminal = is_terminal;

		return node;

	}

	static unsigned char a; // because an unsigned char is 8 bits

	int first_node(double tx0, double ty0, double tz0, double txm, double tym, double tzm);
	int new_node(double txm, int x, double tym, int y, double tzm, int z);

	template<typename T>
	void proc_subtree(double tx0, double ty0, double tz0, double tx1, double ty1, double tz1, OctreeNode<T>* node, std::vector<T>* objects_list)
	{
		if (node == nullptr)//TODO
			return;

		float txm, tym, tzm;
		int currNode;

		if (tx1 < 0 || ty1 < 0 || tz1 < 0) return;
		if (node->m_terminal) {
			TRACE(L"Reached leaf \n ");
			return;
		}
		else 
		{
			TRACE(L"Reached node \n");
		}

		if (node->m_objects.size())
		{
			TRACE(L"Adding %d objects \n", node->m_objects.size());
			objects_list->insert(objects_list->end(), node->m_objects.begin(), node->m_objects.end());
		}


		txm = 0.5*(tx0 + tx1);
		tym = 0.5*(ty0 + ty1);
		tzm = 0.5*(tz0 + tz1);

		currNode = first_node(tx0, ty0, tz0, txm, tym, tzm);
		do {
			switch (currNode)
			{
			case 0: {
				proc_subtree(tx0, ty0, tz0, txm, tym, tzm, node->m_children[a], objects_list);
				currNode = new_node(txm, 4, tym, 2, tzm, 1);
				break; }
			case 1: {
				proc_subtree(tx0, ty0, tzm, txm, tym, tz1, node->m_children[1 ^ a], objects_list);
				currNode = new_node(txm, 5, tym, 3, tz1, 8);
				break; }
			case 2: {
				proc_subtree(tx0, tym, tz0, txm, ty1, tzm, node->m_children[2 ^ a], objects_list);
				currNode = new_node(txm, 6, ty1, 8, tzm, 3);
				break; }
			case 3: {
				proc_subtree(tx0, tym, tzm, txm, ty1, tz1, node->m_children[3 ^ a], objects_list);
				currNode = new_node(txm, 7, ty1, 8, tz1, 8);
				break; }
			case 4: {
				proc_subtree(txm, ty0, tz0, tx1, tym, tzm, node->m_children[4 ^ a], objects_list);
				currNode = new_node(tx1, 8, tym, 6, tzm, 5);
				break; }
			case 5: {
				proc_subtree(txm, ty0, tzm, tx1, tym, tz1, node->m_children[5 ^ a], objects_list);
				currNode = new_node(tx1, 8, tym, 7, tz1, 8);
				break; }
			case 6: {
				proc_subtree(txm, tym, tz0, tx1, ty1, tzm, node->m_children[6 ^ a], objects_list);
				currNode = new_node(tx1, 8, ty1, 8, tzm, 7);
				break; }
			case 7: {
				proc_subtree(txm, tym, tzm, tx1, ty1, tz1, node->m_children[7 ^ a], objects_list);
				currNode = 8;
				break; }
			}
		} while (currNode<8);
	}

	template<typename T>
	void ray_octree_traversal(OctreeNode<T>* octree, andro::ray& ray, std::vector<T>* objects_list) {
		a = 0;
		andro::Vector3 size = octree->m_bounds.GetHalfSize();

		// fixes for rays with negative direction
		if (ray.dir[0] < 0) {
			ray.origin[0] = size[0] - ray.origin[0];
			ray.dir[0] = size[0] -ray.dir[0];
			a |= 4; //bitwise OR (latest bits are XYZ)
		}
		if (ray.dir[1] < 0) {
			ray.origin[1] = size[1] - ray.origin[1];
			ray.dir[1] = -ray.dir[1];
			a |= 2;
		}
		if (ray.dir[2] < 0) {
			ray.origin[2] = size[2] - ray.origin[2];
			ray.dir[2] = -ray.dir[2];
			a |= 1;
		}

		double divx = 1 / ray.dir[0]; // IEEE stability fix
		double divy = 1 / ray.dir[1];
		double divz = 1 / ray.dir[2];

		double tx0 = (octree->m_bounds.min[0] - ray.origin[0]) * divx;
		double tx1 = (octree->m_bounds.max[0] - ray.origin[0]) * divx;
		double ty0 = (octree->m_bounds.min[1] - ray.origin[1]) * divy;
		double ty1 = (octree->m_bounds.max[1] - ray.origin[1]) * divy;
		double tz0 = (octree->m_bounds.min[2] - ray.origin[2]) * divz;
		double tz1 = (octree->m_bounds.max[2] - ray.origin[2]) * divz;

		if (max(max(tx0, ty0), tz0) < min(min(tx1, ty1), tz1)) {
			proc_subtree(tx0, ty0, tz0, tx1, ty1, tz1, octree, objects_list);
		}
	}

}

