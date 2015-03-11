#include "Particle.h"

namespace force
{
	Particle::Particle()
		:damping(0.995)
		,inverseMass(1)
	{

	}

	void Particle::Update(real dt)
	{
		
		assert(dt > 0 );
		if(inverseMass <= 0)
			WARNING("inverseMass must be a positive value");

		position+= velocity*dt;

		Vector3 resultAcc=acceleration;
		resultAcc+= forceAccum * inverseMass;

		velocity+= resultAcc * dt;

		velocity*=real_pow(damping,dt);

		ClearForceAccumulator();
	}

	bool Particle::isMovable()
	{
		return (inverseMass>0);
	}


	void Particle::ClearForceAccumulator()
	{
		forceAccum.x=0;
		forceAccum.y=0;
		forceAccum.z=0;
	}

	Vector3 Particle::GetPosition() const
	{
		return position;
	}

	void Particle::SetPosition(Vector3 pos)
	{
		position=pos;
	}

	void Particle::SetPosition(real x,real y ,real z)
	{
		position=Vector3(x,y,z);
	}

	Vector3 Particle::GetVelocity() const
	{
		return velocity;
	}

	void Particle::SetVelocity(Vector3 vel)
	{
		velocity=vel;
	}

	Vector3	 Particle::GetForce() const
	{
		return forceAccum;
	}

	void Particle::AddForce(Vector3 force)
	{
		forceAccum+=force;
	}

	void Particle::AddVelocity(Vector3 vel)
	{
		velocity+=vel;
	}


	real Particle::GetMass() const
	{
		
		return 1/inverseMass;
	}

	void Particle::SetMass(real mass)
	{
		if(mass<=0)
		{
			WARNING("Mass must be positive value");
			return;
		}
		inverseMass=(real)1/mass;
	}

	void Particle::SetInverseMass(real inv)
	{
		if(inv<=0)
		{
			WARNING("Mass inverse must be positive value");
			return;
		}
		
		inverseMass=inv;
	}
}