#ifndef _PARTICLE_CONTACT_
#define _PARTICLE_CONTACT_

#include "core.h"
#include "Particle.h"
/**
* A contact represents two objects in contact (in this case
* ParticleContact representing two particles). Resolving a
* contact removes their interpenetration, and applies sufficient
* impulse to keep them apart. Colliding bodies may also rebound.
*
* The contact has no callable functions, it just holds the
* contact details. To resolve a set of contacts, use the particle
* contact resolver class.
*/

namespace force
{
	class ParticleContact
	{
		public:
			Particle* particle[2];
			
	   /**
		 * Holds the amount each particle is moved by during interpenetration
		 * resolution.
		 */
			Vector3 particleMovement[2];
		/**
		 * Holds restitution coeficient
		 */
			real restitution; 
		/**
		 * Holds interpenetration value
		 */
			real penetration;
			Vector3 contactNormal;

			void resolve(real duration);
			real calculateSeparatingVelocity() const;
		private:
			void resolveVelocity(real duration);
			void resolveInterpenetration(real duration);
	};

}
#endif