#include "BVHNode.h"

namespace force
{
	template<class BoundingVolume>
	BVHNode<BoundingVolume>::~BVHNode()
	{
		/*
		// If we don’t have a parent, then we ignore the sibling processing.
		if (parent)
		{
		// Find our sibling.
		BVHNode<BoundingVolume> *sibling;
		if (parent->children[0] == this) sibling = parent->children[1];
		else sibling = parent->children[0];
		
		// Write its data to our parent.
		parent->volume = sibling->volume;
		parent->body = sibling->body;
		parent->children[0] = sibling->children[0];
		parent->children[1] = sibling->children[1];
		
		// Delete the sibling (we blank its parent and
		// children to avoid processing/deleting them).
		sibling->parent = NULL;
		sibling->body = NULL;
		sibling->children[0] = NULL;
		sibling->children[1] = NULL;
		delete sibling;
		
		// Recalculate the parent’s bounding volume.
		parent->recalculateBoundingVolume()	
		}

		// Delete our children (again we remove their parent data so
		// we don’t try to process their siblings as they are deleted).
		if (children[0])
		{
			children[0]->parent = NULL;
			delete children[0];
		}
		if (children[1]) 
		{
			children[1]->parent = NULL;
			delete children[0];
		}
		*/
	}

	template<class BoundingVolume>
    void BVHNode<BoundingVolume>::recalculateBoundingVolume()
    {
    	/*
        if (isLeaf()) return;

        // Use the bounding volume combining constructor.
        boundingVolume = new BoundingVolume(*children[0]->volume, *children[1]->volume  );

        // Recurse up the tree
        if (parent) parent->recalculateBoundingVolume();
		*/
	}
	template<class BoundingVolume>
	unsigned int BVHNode<BoundingVolume>::getPotentialContacts(PotentialContacts *contacts, unsigned int limit)
	{
		return 0;

    	/*
		if(isLeaf() || limit == 0)
			return 0;

		return childrens[0]->getPotentialContactsWith(contacts, childrens[1], limit);
		*/
	}

	template<class BoundingVolume>
	unsigned int BVHNode<BoundingVolume>::getPotentialContactsWith(PotentialContacts *contacts,BVHNode* other, unsigned int limit)
	{
		return 0;

    	/*
		if(!Overlaps(other) || limit == 0)
			return 0;

		if(isLeaf() && other->isLeaf())
		{
			contacts->bodys[0] = rigidBody;
			contacts->bodys[1] = other->rigidBody;
			return 1;
		}

		//if other  is leaf or first volume is bigger than other we descend in first
		if(other->isLeaf() || boundingVolume->GetSize() > other->boundingVolume->GetSize() )
		{
			unsigned int count  = childrens[0]->getPotentialContactsWith(contacts, other, limit);

			if(limit > count)
					return count + childrens[1]->getPotentialContactsWith(contacts + count, other, limit);
			else
				return count;
		}
		else
		{
			unsigned int count  = other->childrens[0]->getPotentialContactsWith(contacts, this, limit);

			if(limit > count)
					return count + other->childrens[1]->getPotentialContactsWith(contacts + count, this, limit);
			else
				return count;
		}
		*/
	}

	template<class BoundingVolume>
	void BVHNode<BoundingVolume>::Insert(RigidBody *rb, const BoundingVolume& bv)
	{
		/*
		if(isLeaf())
		{

				childrens[0] = new BVHNode<BoundingVolume>();
				childrens[0]->boundingVolume = new BoundingVolume this->boundingVolume;
				childrens[0]->rigidBody = this->rigidBody;
				childrens[0]->parrent= this;

				childrens[1] = new BVHNode<BoundingVolume>();
				childrens[1]->boundingVolume = bv;
				childrens[1]->rigidBody = rb;
				childrens[1]->parrent= this;

				this->rigidBody = NULL;
				this->boundingVolume = recalculateBoundingVolume();
		
		}
		else if(childrens[0]->boundingVolume->GetGrowth(bv) < childrens[1]->boundingVolume->GetGrowth(bv))
			childrens[0]->Insert(rb, bv);
		else
			childrens[1]->Insert(rb, bv);
	*/
		
	}

}