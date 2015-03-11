#include "ParticleLink.h"

namespace force
{
	real ParticleLink::currentLength() const
	{
		Vector3 relativePos = particle[0]->GetPosition() -
			particle[1]->GetPosition();
		return relativePos.magnitude();
	}

	unsigned ParticleCable::addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const
	{
		real length = currentLength();

		if (length < maxLength)
		{
			return 0;
		}
		// Otherwise return the contact.
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];
		// Calculate the normal.
		Vector3 normal = particle[1]->GetPosition() - particle[0]->GetPosition();
		normal.normalise();

		contact->contactNormal = normal;
		contact->penetration = length - maxLength;
		contact->restitution = restitution;

		return 1;
	}

	unsigned ParticleFlexibleRod::addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const
	{
		real length = currentLength();


		if (length < maxLength && length > minLength)
		{
			return 0;
		}

		// Otherwise return the contact.
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];
		// Calculate the normal.
		Vector3 normal = particle[1]->GetPosition() - particle[0]->GetPosition();
		normal.normalise();

		if(length > minLength)
		{
			contact->contactNormal = normal;
			contact->penetration = length - maxLength;
			contact->restitution = restitution;
		}
		else
		{
			contact->contactNormal = normal * -1;
			contact->penetration = minLength- length  ;
			contact->restitution =2* restitution;
		}
		
		return 1;
	}


	unsigned ParticleRod::addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const
	{
		// Find the length of the rod.
		real currentLen = currentLength();

		// Check whether we’re overextended.
		if (currentLen == length)
		{
			return 0;
		}

		// Otherwise return the contact.
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];
		// Calculate the normal.
		Vector3 normal = particle[1]->GetPosition() - particle[0]
		->GetPosition();
		normal.normalise();
		// The contact normal depends on whether we’re extending
		// or compressing.
		if (currentLen > length) {
			contact->contactNormal = normal;
			contact->penetration = currentLen - length;
		} else {
			contact->contactNormal = normal * -1;
			contact->penetration = length - currentLen;
		}
		// Always use zero restitution (no bounciness).
		contact->restitution = 0;
		return 1;
}

	real ParticleCableAnchor::currentLength() const
	{
		Vector3 relativePos = particle[0]->GetPosition() -	anchor;
		return relativePos.magnitude();
	}


	unsigned ParticleCableAnchor::addContact(ParticleContact *contact,
		unsigned limit, std::vector<Particle*> particles) const
	{
		// Find the length of the rod.
		real currentLen = currentLength();
		// Check whether we’re overextended.
		if (currentLen == length)
		{
			return 0;
		}

		// Otherwise return the contact.
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];
		// Calculate the normal.
		Vector3 normal = anchor - particle[0]->GetPosition();
		normal.normalise();
		// The contact normal depends on whether we’re extending
		// or compressing.
		if (currentLen > length) {
			contact->contactNormal = normal;
			contact->penetration = currentLen - length;
		} else {
			contact->contactNormal = normal * -1;
			contact->penetration = length - currentLen;
		}
		// Always use zero restitution (no bounciness).
		contact->restitution = 0;
		return 1;
	}

}
