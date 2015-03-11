#ifndef _PARTICLE_WORLD_
#define _PARTICLE_WORLD_

#include "ParticleContactResolver.h"
#include "ParticleContactGenerator.h"
#include "ParticleForceGenerator.h"
#include <vector>
#include "Singleton.h"

namespace force
{



	/**
	* Keeps track of a set of particles, and provides the means to
	* update them all.
	*/
	class ParticleWorld
	{
	public:
		SINGLETON_(ParticleWorld);

		ParticleForceRegistry registry;
		ParticleContactResolver particle_resolver;

		/**
		* Holds one particle in the linked list of particles.
		*/
		
		std::vector<Particle*> particles;
		
		std::vector<ParticleContactGenerator*> particle_contactGenerators;
	
		ParticleContact *particle_contacts;

		unsigned maxContacts;

		public:

		void AddParticles(Particle* particles, unsigned int count);
		void AddParticleContactGenerators(ParticleContactGenerator* contactgen, unsigned int count);
		void startFrame();

		/**
		* Calls each of the registered contact generators to report
		* their contacts. Returns the number of generated contacts.
		*/
		unsigned generateContacts();

		/**
		* Integrates all the particles in this world forward in time
		* by the given duration.
		*/
		void integrate(real duration);

		/**
		* Processes all the physics for the particle world.
		*/
		void runPhysics(real duration);
	};
}
#endif