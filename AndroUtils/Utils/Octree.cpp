#include "Octree.h"

namespace andro
{


	void SplitToEight(const BoundingBox& box, BoundingBox boxes[8])
	{

		Vector3 halfSize = box.GetHalfSize();
		Vector3 min = box.min;
		Vector3 max = box.max;
		Vector3 mid = (min + max)*0.5f;

		boxes[NEAR_BOTTOM_RIGHT].SetExtents(Vector3(min.x, min.y, min.z), Vector3(min.x, min.y, min.z) + halfSize);
		boxes[NEAR_BOTTOM_LEFT].SetExtents(Vector3(mid.x, min.y, min.z), Vector3(mid.x, min.y, min.z) + halfSize);

		boxes[NEAR_TOP_RIGHT].SetExtents(Vector3(min.x, mid.y, min.z), Vector3(min.x, mid.y, min.z) + halfSize);
		boxes[NEAR_TOP_LEFT].SetExtents(Vector3(mid.x, mid.y, min.z), Vector3(mid.x, mid.y, min.z) + halfSize);

		boxes[FAR_BOTTOM_RIGHT].SetExtents(Vector3(min.x, min.y, mid.z), Vector3(min.x, min.y, mid.z) + halfSize);
		boxes[FAR_BOTTOM_LEFT].SetExtents(Vector3(mid.x, min.y, mid.z), Vector3(mid.x, min.y, mid.z) + halfSize);

		boxes[FAR_TOP_RIGHT].SetExtents(Vector3(min.x, mid.y, mid.z), Vector3(min.x, mid.y, mid.z) + halfSize);
		boxes[FAR_TOP_LEFT].SetExtents(Vector3(mid.x, mid.y, mid.z), Vector3(mid.x, mid.y, mid.z) + halfSize);
	}


}