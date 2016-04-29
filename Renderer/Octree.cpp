#include "Octree.h"
#include "../../Renderer/RenderObject.h"

namespace andro
{
	Vector3 BoundingBox::GetHalfSize() const
	{
		Vector3 halfSize = (min + max)*0.5f - min;
		return halfSize;
	}

	Vector3 BoundingBox::GetCenter() const
	{
		return (min + max)*0.5f;
	}


	void BoundingBox::SplitToEight(BoundingBox boxes[8])
	{
		Vector3 mid = (min + max)*0.5f;
		Vector3 halfSize = GetHalfSize();

		boxes[NEAR_BOTTOM_RIGHT].SetExtents(Vector3(min.x, min.y, min.z), Vector3(min.x, min.y, min.z) + halfSize);
		boxes[NEAR_BOTTOM_LEFT].SetExtents(Vector3(mid.x, min.y, min.z), Vector3(mid.x, min.y, min.z) + halfSize);

		boxes[NEAR_TOP_RIGHT].SetExtents(Vector3(min.x, mid.y, min.z), Vector3(min.x, mid.y, min.z) + halfSize);
		boxes[NEAR_TOP_LEFT].SetExtents(Vector3(mid.x, mid.y, min.z), Vector3(mid.x, mid.y, min.z) + halfSize);

		boxes[FAR_BOTTOM_RIGHT].SetExtents(Vector3(min.x, min.y, mid.z), Vector3(min.x, min.y, mid.z) + halfSize);
		boxes[FAR_BOTTOM_LEFT].SetExtents(Vector3(mid.x, min.y, mid.z), Vector3(mid.x, min.y, mid.z) + halfSize);

		boxes[FAR_TOP_RIGHT].SetExtents(Vector3(min.x, mid.y, mid.z), Vector3(min.x, mid.y, mid.z) + halfSize);
		boxes[FAR_TOP_LEFT].SetExtents(Vector3(mid.x, mid.y, mid.z), Vector3(mid.x, mid.y, mid.z) + halfSize);
	}


	OctreeNode* BuildOctreeFromMesh(TakeTwo::RenderObject* object, std::vector<andro::Triangle>& objects, BoundingBox& aabb, int stopDepth)
	{
		// stop if we reached certain depth or certain number of objects
		if (stopDepth < 0 || objects.size() < 3)
			return NULL;

		// Split to 8 cubes
		BoundingBox boxes[8];
		aabb.SplitToEight(boxes);

		// Create a node and attach the objects
		OctreeNode* node = new OctreeNode;
		node->m_bounds = aabb;
		node->m_objects = objects; // Warning: copying here best be avoided.

		std::vector<andro::Triangle> boxObjects[8];

		TakeTwo::Mesh& mesh = object->GetMesh();

		//for each triangle check intersection with 8 boxes
		for (int i = 0; i < objects.size(); ++i)
		{
			Triangle& tri = objects[i];
			for (int boxId = 0; boxId<8; ++boxId)
			{
				Vector3 center = boxes[boxId].GetCenter();
				Vector3 halfsize = boxes[boxId].GetHalfSize();
				bool overlaps = TriangleBoxOverlap(center, halfsize, tri);

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
				node->m_children[i] = BuildOctreeFromMesh(object, boxObjects[i], boxes[i], stopDepth - 1);
			}
		}

		return node;
	}
}