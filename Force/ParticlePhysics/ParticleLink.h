#ifndef _PARTICLELINK_
#define _PARTICLELINK_

#include "core.h"
#include "ParticleContactResolver.h"
#include "ParticleContactGenerator.h"
#include <vector>


namespace force
{


	/**
		* Links connect two particles together, generating a contact if
		* they violate the constraints of their link. It is used as a
		* base class for cables and rods, and could be used as a base
		* class for springs with a limit to their extension.
	*/
	class ParticleLink : public ParticleContactGenerator
	{
		
	protected:
		real currentLength() const;
		Particle* particle[2];

	public:
		ParticleLink(Particle* p1, Particle* p2){particle[0]=p1; particle[1]=p2; }	
        virtual unsigned addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const = 0;
	};


	/**
	* Cables link a pair of particles, generating a contact if they
	* stray too far apart.
	*/
	class ParticleCable : public ParticleLink
	{
		real maxLength;

		/**
		* Holds the restitution (bounciness) of the cable.
		*/
		real restitution;

	public:
		ParticleCable():ParticleLink(NULL,NULL)
		{
			maxLength = 0;
			this->restitution =0;
		}
		ParticleCable(Particle* p1, Particle* p2,real maxLen ,real restitution=0.0f):ParticleLink(p1,p2)
		{
			maxLength = maxLen;
			this->restitution = restitution;
		}

        virtual unsigned addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const;
	};



	/**
	* Flexible Rods link a pair of particles, generating a contact if they
	* stray too far apart or too close.
	*/
	class ParticleFlexibleRod : public ParticleLink
	{
		real maxLength;
		real minLength;

		/**
		* Holds the restitution (bounciness) of the cable.
		*/
		real restitution;

	public:
		ParticleFlexibleRod():ParticleLink(NULL,NULL)
		{
			maxLength = 0;
			minLength = 0;
			this->restitution =0;
		}
		ParticleFlexibleRod(Particle* p1, Particle* p2,real maxLen ,real minLen = -1, real restitution=0.0f):ParticleLink(p1,p2)
		{

			maxLength = maxLen;
			if(minLen == -1)
				minLength = maxLength - maxLength/10;
			else
				minLength = minLen;
			this->restitution = restitution;
		}

        virtual unsigned addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const;
	};



	/**
	* Rods link a pair of particles, generating a contact if they
	* stray too far apart or too close.
	*/
	class ParticleRod : public ParticleLink
	{

		real length;
	public:
		ParticleRod():ParticleLink(NULL,NULL)
		{
		  length = 0;
		}
		ParticleRod(Particle* p1, Particle* p2, real len):ParticleLink(p1,p2)
		{
		  length = len;
		}
	
        virtual unsigned addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const;
	};

	class ParticleCableAnchor : public ParticleLink
	{

		real length;
		Vector3 anchor;
	public:
		ParticleCableAnchor(Particle* p1, Vector3 p2, real len):ParticleLink(p1,NULL)
		{
			length = len;
			anchor = p2;
		}
		real currentLength() const;
		virtual unsigned addContact(ParticleContact *contact,
			unsigned limit, std::vector<Particle*> particles) const;
	};

}

#endif 