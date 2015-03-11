#ifndef COLL_DET_H
#define COLL_DET_H

#include "ParticlePhysics/Particle.h"
#include "RigidBodyPhysics/Primitive.h"
#include "RigidBodyPhysics/Contact.h"
#include <vector>


namespace force
{

	class CollisionDetector
	{
		public:
			unsigned sphereAndSphere(const Sphere &one, const Sphere &two, CollisionData *data);
			unsigned sphereAndHalfSpace(const Sphere &sphere, const Plane &plane, CollisionData *data);
			unsigned boxAndHalfSpace(const Box &box, const Plane &plane, CollisionData *data);
			unsigned boxAndSphere(const Box &box, const Sphere &sphere, CollisionData *data);
			unsigned boxAndPoint(const Box &box, const Vector3 &point, CollisionData *data);
			unsigned boxAndBox( const Box &one, const Box &two, CollisionData *data  );
			unsigned CylinderAndSphere(const Cylinder &cylinder, const Sphere &sphere, CollisionData *data);


		
	};

}
#endif