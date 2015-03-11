#include "ParticleContactResolver.h"

namespace force
{


	ParticleContactResolver::ParticleContactResolver(unsigned iterations /* = 0 */)
	{
		this->iterations = iterations;
	}

	void ParticleContactResolver::resolveContacts(ParticleContact *contactArray,
											 unsigned numContacts,
											 real duration)
	{

		if(!iterations)
			iterations = numContacts * 2;

		iterationsUsed = 0;
		while(iterationsUsed < iterations)
		{
			// Find the contact with the largest closing velocity;
			real max = 0;
			unsigned maxIndex = numContacts;
			for (unsigned i = 0; i < numContacts; i++)
			{
				real sepVel = contactArray[i].calculateSeparatingVelocity();
				if (sepVel < max)
				{
					max = sepVel;
					maxIndex = i;
				}
			}


			// Do we have anything worth resolving?
			if (maxIndex == numContacts) break;

			// Resolve this contact.
			contactArray[maxIndex].resolve(duration);
			
			// Update the interpenetrations for all contacts
			Vector3 *move = contactArray[maxIndex].particleMovement;
			for (unsigned int i = 0; i < numContacts; i++)
			{
				if (contactArray[i].particle[0] == contactArray[maxIndex].particle[0])
				{
					contactArray[i].penetration -= move[0] * contactArray[i].contactNormal;
				}
				else if (contactArray[i].particle[0] == contactArray[maxIndex].particle[1])
				{
					contactArray[i].penetration -= move[1] * contactArray[i].contactNormal;
				}
				if (contactArray[i].particle[1])
				{
					if (contactArray[i].particle[1] == contactArray[maxIndex].particle[0])
					{
						contactArray[i].penetration += move[0] * contactArray[i].contactNormal;
					}
					else if (contactArray[i].particle[1] == contactArray[maxIndex].particle[1])
					{
						contactArray[i].penetration += move[1] * contactArray[i].contactNormal;
					}
				}
			}

			iterationsUsed++;
		}
	}




}