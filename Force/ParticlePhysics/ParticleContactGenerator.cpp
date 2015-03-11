#include "ParticleContactGenerator.h"


namespace force
{
	GroundContacts::GroundContacts(real groundlevel)
	{
		m_groundlevel = groundlevel;
	}

	unsigned GroundContacts::addContact(ParticleContact *contact,
										unsigned limit, std::vector<Particle*> particles) const
	{
		unsigned count = 0;
		std::vector<Particle*>::const_iterator p;
		for(p = particles.begin() ; p!=particles.end(); p++)
		{
			real y = (*p)->GetPosition().y;
			if (y < m_groundlevel)
			{
				contact->contactNormal = Vector3(0, 1, 0);
				contact->particle[0] = *p;
				contact->particle[1] = NULL;
				contact->penetration = m_groundlevel - y;
				contact->restitution = 0.1f;
				contact++;
				count++;
			}

			if (count >= limit) return count;
		}
		return count;
	}

}