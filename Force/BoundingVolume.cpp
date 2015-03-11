#include "BoundingVolume.h"

namespace force
{
	BoundingSphere::BoundingSphere()
	{}
	
	BoundingSphere::BoundingSphere(force::Vector3 position, real rad)
		:position()
		,radius(rad)
	{
	}

	BoundingSphere::BoundingSphere(const BoundingSphere& b1,const  BoundingSphere& b2)	
	{
		Vector3 centreOffset = b2.position - b1.position;
		real distance = centreOffset.squareMagnitude();
		real radiusDiff = b2.radius - b1.radius;

		// Check if the larger sphere encloses the small one
		if (radiusDiff*radiusDiff >= distance)
		{
			if (b1.radius > b2.radius)
			{
				position = b1.position;
				radius = b1.radius;
			}
			else
			{
				position = b2.position;
				radius = b2.radius;
			}
		}

		// Otherwise we need to work with partially
		// overlapping spheres
		else
		{
			distance = real_sqrt(distance);
			radius = (distance + b1.radius + b2.radius) * ((real)0.5);

			// The new centre is based on one's centre, moved towards
			// two's centre by an ammount proportional to the spheres'
			// radii.
			position = b1.position;
			if (distance > 0)
			{
				position += centreOffset * ((radius - b1.radius)/distance);
			}
    }
		
	}

	bool BoundingSphere::Overlaps(const BoundingSphere& other) const
	{

			Vector3 difference = other.GetPosition() - position;
			if(difference.squareMagnitude() <= (other.GetSize() + radius) * (other.GetSize() + radius))
				return true; 
			else
				return false;
	}
	
	real BoundingSphere::GetGrowth(const BoundingSphere& other) const
	{
			BoundingSphere newSphere = BoundingSphere(*this, other);
		
			// We return a value proportional to the change in surface
		    // area of the sphere.
			return newSphere.radius*newSphere.radius - radius * radius; 
							
	}

}