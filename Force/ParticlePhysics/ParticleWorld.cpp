#include "ParticleWorld.h"

namespace force
{
	ParticleWorld::ParticleWorld()
	{
		maxContacts = 200;
		particle_contacts = new ParticleContact[maxContacts];
	}

	ParticleWorld::~ParticleWorld()
	{
	}

	void ParticleWorld::startFrame()
	{
		for(unsigned int i = 0; i < particles.size(); i++)
		{
			// Remove all forces from the accumulator.
			particles[i]->ClearForceAccumulator();
		}
	}

	
	void ParticleWorld::AddParticles(Particle* particles, unsigned int count)
	{
		for(unsigned int i = 0 ; i < count;i++)
		{
			if(particles==NULL)
				break;

			ParticleWorld::particles.push_back(particles);
			particles++;
		}
	}
	
	void ParticleWorld::AddParticleContactGenerators(ParticleContactGenerator* contactgen, unsigned int count)
	{
		for(unsigned int i = 0 ; i < count;i++)
		{
			if(contactgen==NULL)
				break;

			particle_contactGenerators.push_back(contactgen);
			contactgen++;
		}
	}

	unsigned ParticleWorld::generateContacts()
	{
		unsigned limit = maxContacts;
		ParticleContact* nextContact =  particle_contacts;
		for(unsigned int i = 0; i < particle_contactGenerators.size(); i++)
		{
			unsigned used = particle_contactGenerators[i]->addContact(nextContact, limit, this->particles);
			limit -= used;
			nextContact += used;
			// We’ve run out of contacts to fill. This means we’re missing
			// contacts.
			if (limit <= 0) break;
		}
		// Return the number of contacts used.
		return maxContacts - limit;
	}
	
	void ParticleWorld::integrate(real duration)
	{

		for(unsigned int i = 0; i < particles.size(); i++)
		{
			// Remove all forces from the accumulator.
			particles[i]->Update(duration);
		}
	}


	void ParticleWorld::runPhysics(real duration)
	{
		if(duration > 0)
		{
			// First apply the force generators.
			registry.updateForces(duration);
			// Then integrate the objects.
			ParticleWorld::integrate(duration);
			// Generate contacts.
			unsigned usedContacts = ParticleWorld::generateContacts();
			// And process them.
		//	if (calculateIterations)
				particle_resolver.setIterations(usedContacts * 2);

			particle_resolver.resolveContacts(particle_contacts, usedContacts, duration);
		}
	}

}