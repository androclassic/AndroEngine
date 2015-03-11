#ifndef _BOUNDINGVOLUME_
#define _BOUNDINGVOLUME_
#include "Matrix4.h"
#include "Vector3.h"
namespace force
{


class BoundingSphere 
{
	public:
		BoundingSphere();
		BoundingSphere(Vector3 position, real rad);
		//Create a sphare that contain both b1 and b2
		BoundingSphere(const BoundingSphere& b1,const  BoundingSphere& b2);

		real GetGrowth(const BoundingSphere&  other) const	;	
		bool Overlaps(const BoundingSphere&  other)const ;
		real GetSize() const{ return radius; }
		Vector3 GetPosition()const { return position; }
	private:
		real radius;
		Vector3 position;

};
}
#endif