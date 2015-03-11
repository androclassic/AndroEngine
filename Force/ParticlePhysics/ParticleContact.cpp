#include "ParticleContact.h"

namespace force
{
	void ParticleContact::resolve(real duration)
	{
		resolveVelocity(duration);
		resolveInterpenetration(duration);
	}

	real ParticleContact::calculateSeparatingVelocity() const
	{
		Vector3 relativeVelocity = particle[0]->GetVelocity();
		if (particle[1] != NULL) relativeVelocity -= particle[1]->GetVelocity();
		return relativeVelocity * contactNormal;
	}

	void ParticleContact::resolveVelocity(real duration)
	{
		// Find the velocity in the direction of the contact.
		real separatingVelocity = calculateSeparatingVelocity();
		// Check whether it needs to be resolved.
		if (separatingVelocity > 0)
		{
			// The contact is either separating or stationary - there’s
			// no impulse required.
			return;
		}
		// Calculate the new separating velocity.
		real newSepVelocity = -separatingVelocity * restitution;

		// Check the velocity build-up due to acceleration only.
		Vector3 accCausedVelocity = particle[0]->GetAcceleration();
		if (particle[1]) accCausedVelocity -= particle[1]->GetAcceleration();
		real accCausedSepVelocity = accCausedVelocity *	contactNormal * duration;
		
		// If we’ve got a closing velocity due to acceleration build-up,
		// remove it from the new separating velocity.
		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += restitution * accCausedSepVelocity;
			// Make sure we haven’t removed more than was
			// there to remove.
			if (newSepVelocity < 0) newSepVelocity = 0;
		}

		real deltaVelocity = newSepVelocity - separatingVelocity;

		// We apply the change in velocity to each object in proportion to
		// its inverse mass (i.e., those with lower inverse mass [higher
		// actual mass] get less change in velocity).
		real totalInverseMass = particle[0]->GetInverseMass();
		if (particle[1]) totalInverseMass += particle[1]->GetInverseMass();
		// If all particles have infinite mass, then impulses have no effect.
		if (totalInverseMass <= 0) return;
		// Calculate the impulse to apply.

		real impulse = deltaVelocity / totalInverseMass;

		// Find the amount of impulse per unit of inverse mass.
		Vector3 impulsePerIMass = contactNormal * impulse;

		// Apply impulses: they are applied in the direction of the contact,
		// and are proportional to the inverse mass.
		particle[0]->SetVelocity(particle[0]->GetVelocity() + impulsePerIMass * particle[0]->GetInverseMass());
		if (particle[1])
		{
			// Particle 1 goes in the opposite direction.
			particle[1]->SetVelocity(particle[1]->GetVelocity() + impulsePerIMass * -particle[1]->GetInverseMass());
		}
	}

	void ParticleContact::resolveInterpenetration(real duration)
	{

		// If we don’t have any penetration, skip this step.
		if (penetration <= 0) return;

		// The movement of each object is based on its inverse mass, so
		// total that.
		real totalInverseMass = particle[0]->GetInverseMass();
		if (particle[1]) 
			totalInverseMass += particle[1]->GetInverseMass();

		// If all particles have infinite mass, then we do nothing.
		if (totalInverseMass <= 0) return;

		// Find the amount of penetration resolution per unit of inverse mass.
	  	Vector3 movePerIMass = contactNormal *	(penetration / totalInverseMass);
	
		  particleMovement[0] = movePerIMass * particle[0]->GetInverseMass();

		// Apply the penetration resolution.
		particle[0]->SetPosition(particle[0]->GetPosition() +  particleMovement[0]);
		if (particle[1])
		{
			particle[1]->SetPosition(particle[1]->GetPosition() - (movePerIMass * particle[1]->GetInverseMass()));
			particleMovement[1] = movePerIMass * -particle[1]->GetInverseMass();
		}
	}
}