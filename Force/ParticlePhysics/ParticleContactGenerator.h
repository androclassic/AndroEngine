#ifndef _PARTICLE_CONTACT_GENERATOR_
#define _PARTICLE_CONTACT_GENERATOR_

#include "ParticleContact.h"
#include <vector>

namespace force
{
	/**
	 * This is the basic polymorphic interface for contact generators
	 * applying to particles.
	*/
	class ParticleContactGenerator
	{
	public:
		/**
		* Fills the given contact structure with the generated
		* contact. The contact pointer should point to the first
		* available contact in a contact array, where limit is the
		* maximum number of contacts in the array that can be written
		* to. The method returns the number of contacts that have
		* been written.
		*/
		virtual unsigned addContact(ParticleContact *contact,
			unsigned limit, std::vector<Particle*> particles) const = 0;
	};


    /**
      * A contact generator that takes an STL vector of particle pointers and
     * collides them against the ground.
     */
    class GroundContacts : public ParticleContactGenerator
    {
		real m_groundlevel;
    public:
			GroundContacts(real groundlevel=0);
        virtual unsigned addContact(ParticleContact *contact,
            unsigned limit, std::vector<Particle*> particles) const;
    };


}

#endif