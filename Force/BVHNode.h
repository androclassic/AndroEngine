#ifndef BVHNODE
#define BVHNODE
#include "BoundingVolume.h"
#include "RigidBody.h"

namespace force
{
	struct PotentialContacts
	{
		RigidBody* bodys[2];
	};
	
	template<class BoundingVolume>
	class BVHNode
	{
		public:
			~BVHNode();
			bool isLeaf(){ return rigidBody != NULL; }
			bool Overlaps(BVHNode* other){ return boundingVolume->Overlaps(other->boundingVolume); }
			void Insert(RigidBody *rb, const BoundingVolume& bv);

			unsigned int getPotentialContacts(PotentialContacts* contacts, unsigned int limit); 
			unsigned int getPotentialContactsWith(PotentialContacts* contacts, BVHNode* other, unsigned int limit); 
			void recalculateBoundingVolume();

			RigidBody*		rigidBody;
			BoundingVolume* boundingVolume;
			BVHNode*		childrens[2];
			BVHNode*		parent;
	};

}
#endif