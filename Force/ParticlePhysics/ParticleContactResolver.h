#ifndef _PCONTACT_RESOLVER_
#define _PCONTACT_RESOLVER_

#include "ParticlePhysics/ParticleContact.h"

namespace force
{

	/**
	* The contact resolution routine for particle contacts. One
	* resolver instance can be shared for the whole simulation.
	*/
	class ParticleContactResolver
	{
	protected:
		unsigned iterations;
		unsigned iterationsUsed;
	public:
		ParticleContactResolver(unsigned iterations = 0);
		void setIterations(unsigned iterations){this->iterations = iterations; }
		void resolveContacts(ParticleContact *contactArray,	unsigned numContacts,	real duration);
	};




}
#endif