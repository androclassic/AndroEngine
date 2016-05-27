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

	int first_node(double tx0, double ty0, double tz0, double txm, double tym, double tzm) {
		unsigned char answer = 0;   // initialize to 00000000
									// select the entry plane and set bits
		if (tx0 > ty0) {
			if (tx0 > tz0) { // PLANE YZ
				if (tym < tx0) answer |= 2;    // set bit at position 1
				if (tzm < tx0) answer |= 1;    // set bit at position 0
				return (int)answer;
			}
		}
		else {
			if (ty0 > tz0) { // PLANE XZ
				if (txm < ty0) answer |= 4;    // set bit at position 2
				if (tzm < ty0) answer |= 1;    // set bit at position 0
				return (int)answer;
			}
		}
		// PLANE XY
		if (txm < tz0) answer |= 4;    // set bit at position 2
		if (tym < tz0) answer |= 2;    // set bit at position 1
		return (int)answer;
	}

	int new_node(double txm, int x, double tym, int y, double tzm, int z) {
		if (txm < tym) {
			if (txm < tzm) { return x; }  // YZ plane
		}
		else {
			if (tym < tzm) { return y; } // XZ plane
		}
		return z; // XY plane;
	}

}