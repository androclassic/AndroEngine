   /*Georgescu Andrei	*	
	*		 Force		*
	*		 2012		*/ 

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "core.h"
#include "Matrix3.h"
#include "Matrix4.h"

#ifndef MAX_MASS
#define MAX_MASS 1000
#endif

namespace force
{
class Particle
	{
	public:
		Particle();

virtual	void		Update(real dt);

		void		SetPosition(Vector3 pos);
		void		SetPosition(real x,real y ,real z);
		Vector3		GetPosition() const;
		
		void		SetVelocity(Vector3 vel);
		void        AddVelocity(Vector3 vel);
		Vector3		GetVelocity() const;
		
		void		SetMass(real mass);
		void		SetInverseMass(real inv);
		real		GetInverseMass() const { return inverseMass;}
		Vector3		GetAcceleration()const { return acceleration;}

		real		GetMass() const;
		
		void		AddForce(Vector3 force);
		Vector3		GetForce() const;

		void		ClearForceAccumulator();
		void		SetDamping(real	damping) {this->damping =  damping;} 
		bool		isMovable();
	protected:
		Vector3		position;
		Vector3		velocity;
		Vector3		acceleration;
		Vector3		forceAccum;

		real		inverseMass;
		real		damping;

	};

}
#endif //FORCE_PARTICLE_H